//
// Created by david on 13.12.2021.
//

#include "GameManager.h"

map<int, Game *> GameManager::runningGames;
queue<Game *> GameManager::waitingGames;
int GameManager::idOfNextGame = 1;
string name_opponent;
string mark;
string state1;


void GameManager::playerEnqueue(Player *pl) {
    if (pl == nullptr) {
        return;
    }

    if (pl->state != "LOBBY") {
        return;
    }
    queue < Game * > temp = waitingGames;

    while (!temp.empty()) {
        if (temp.front()->isFull()) {
            temp.pop();
            continue;
        }
        pl->state = "WAITING";
        pl->mark = "O";
        pl->prioriti = 0;
        pl->end = 0;
        temp.front()->addPlayer(pl);
        pl->game_id = temp.front()->getGameId();
        cout << "Player " + pl->name + " join to game " + to_string(temp.front()->getGameId()) << endl;

        string secMessage1 = "FIND_GAME|OK|GAME|" + to_string(temp.front()->getGameId()) + "|" + pl->mark;
        Response::sendToPlayer(pl, secMessage1);


        for (auto &it: temp.front()->getPlayers()) {
            if (it->name != pl->name) {
                name_opponent = it->name;
                mark = it->mark;
                state1 = it->state1;
            }
        }

        string Message = "OPPONENT|" + name_opponent;
        string Message1 = Message + "|";
        string Message3 = Message1 + mark;
        string Message4 = Message3 + "|";
        string Message5 = Message4 + state1;

        Response::sendToPlayer(pl, Message5);


        string Message2 = "OPPONENT|" + pl->name + "|"+ pl->mark + "|" + pl->state1;
        vector<Player *> vectPl = temp.front()->getPlayers();
        for (auto &it: vectPl) {
            if (it->name != pl->name) {
                Response::sendToPlayer(it, Message2);
            }
        }
        return;
    }
    //if queue is empty
    Game *newGame = new Game(idOfNextGame);
    idOfNextGame++;
    pl->game_id = newGame->getGameId();
    newGame->addPlayer(pl);
    pl->prioriti = 1;
    pl->end = 0;
    pl->mark = "X";
    waitingGames.push(newGame);
    pl->state = "WAITING";

    string secMessage = "FIND_GAME|OK|GAME|" + to_string(newGame->getGameId()) + "|" + pl->mark;
    Response::sendToPlayer(pl, secMessage);


}

void GameManager::playerDequeue(Player *pl) {
    if (pl == nullptr) {
        return;
    }

    if (pl->state != "WAITING") {
        return;
    }

    int gameId = pl->game_id;
    queue < Game * > temp = waitingGames;
    while (!temp.empty()) {
        if (temp.front()->getGameId() == gameId) {
            temp.front()->removePlayer(pl);
            vector<Player *> vectPl = temp.front()->getPlayers();
            for (auto &it: vectPl) {
                if (it->name != pl->name) {
                    it->prioriti = 1;
                    it->mark = "X";
                }
            }
            string secMessage = "DEQUEUE|X";
            Response::sendToPlayer(pl, secMessage);
            notifyOponent(pl, secMessage);

        }

        temp.pop();
    }


    if (waitingGames.size() == 1 && waitingGames.front()->isEmpty()) {
        waitingGames.pop();
    }

    pl->state = "LOBBY";
    pl->game_id = 0;
    string message = "LEAVE_GAME|OK";
    Response::sendToPlayer(pl, message);
}

void GameManager::startGame(Player *pl) {
    if (pl->state != "WAITING") {
        cout << "Error: PLAYER IS NOT WAITING " + pl->name << endl;
        return;
    }

    if (pl->game_id <= 0) {
        cout << "Error: PLAYER IS NOT IN GAME " + pl->name << endl;
        return;
    }

    queue < Game * > temp = waitingGames;
    Game *gamePtr = nullptr;

    while (!temp.empty()) {
        if (temp.front()->getGameId() == pl->game_id) {
            gamePtr = temp.front();
            auto vectPl = gamePtr->getPlayers();

            if (vectPl.size() != 2) {
                cout << "CANNOT START GAME" << endl;
                Response::sendErrorToPlayer(pl,"INVALID_COMMAND|CANNOT_START_GAME_ALONE");
                return;
            }
            for (auto &it: vectPl) {
                it->state = "IN_GAME";
                Response::sendToPlayer(it, "START_GAME|OK");
            }
            gamePtr->gameState = "RUNNING";
            for (auto &it: vectPl) {
                if(it->prioriti == 1)
                    Response::sendToPlayer(it, "IT_IS_YOUR_TURN|" + to_string(it->prioriti));
            }
            runningGames.insert(make_pair(gamePtr->getGameId(), gamePtr));
            temp.pop();
            waitingGames = temp;
            return;
        }

    }


}

void GameManager::turn(Player *pl, int option) {

    if (option > 7 || option < 1) {
        return;
    }

    if (pl->state != "IN_GAME") {
        return;
    }
    auto gamePtr = GameManager::runningGames.at(pl->game_id);
    if (gamePtr->gameState != "RUNNING") {
        return;
    }
    vector<Player *> vectPl = gamePtr->getPlayers();
    pl->game_move = option;
    int result = GameLogic::Connect4(gamePtr, pl);
    if(result == 0){
        Response::sendErrorToPlayer(pl,"INVALID_MOVE_ROW_IS_FULL");
        return;
    }
    string game_move = to_string(pl->game_move);
    Response::sendToPlayer(pl, "GAME_MOVE|OK|" + game_move);
    pl->prioriti--;

    for (auto &it: vectPl) {
        if( it->name != pl->name) {
            it->prioriti++;
            Response::sendToPlayer(it, "IT_IS_YOUR_TURN|OPPONENT_TURN|" + game_move);
        }
    }

    if(result == 1){
        Response::sendToPlayer(pl, "GAME_END|YOU_WIN");
        pl->end = 1;
        pl->state = "END";
        for (auto &it: vectPl) {
            if( it->name != pl->name) {
                it->end = 1;
                it->state= "END";
                Response::sendToPlayer(it, "GAME_END|YOU_LOSE");
            }
        }
        return;
    }
    if(result == 2){
        for (auto &it: vectPl) {
            it->end = 1;
            Response::sendToPlayer(it, "GAME_END|DRAW");
        }
        return;
    }

}

void GameManager::playerLeaveGame(Player *pl) {

    if (pl->state != "IN_GAME" && pl->state != "END") {
        return;
    }
    int gameId = pl->game_id;
    auto gamePtr = runningGames.at(gameId);
    vector<Player *> vectPl = gamePtr->getPlayers();
    auto gamePlayers = gamePtr->getPlayers();
    gamePtr->removePlayer(pl);
    pl->game_id = 0;
    pl->game_move = -1;
    pl->state = "LOBBY";
    Response::sendToPlayer(pl, "LEAVE_GAME|OK");
    notifyOponent(pl, "LEAVE_GAME");
    for (auto &it: vectPl) {
        if( it->name != pl->name) {
            if(it->end == 0)
                Response::sendToPlayer(it, "GAME_END|YOU_WIN");
        }
    }

}

Game *GameManager::gameExists(int gameId) {

    queue < Game * > temp = waitingGames;
    Game *gamePtr = nullptr;
    while (!temp.empty()) {
        if (temp.front()->getGameId() == gameId) {
            gamePtr = temp.front();
            return gamePtr;
        }
        temp.pop();
    }

    map<int, Game *>::iterator it;

    for (it = runningGames.begin(); it != runningGames.end(); it++) {
        if (it->first == gameId) {
            return it->second;
        }
    }

    return nullptr;

}

void GameManager::notifyOponent(Player *pl, string message) {
    if (pl == nullptr) {
        return;
    }

    auto gamePtr = GameManager::gameExists(pl->game_id);

    if (gamePtr == nullptr) {
        return;
    }

    for (auto &it: gamePtr->getPlayers()) {
        if (it->name != pl->name) {
            Response::sendToPlayer(it, "INFO|" + pl->name + "|" + message);
        }
    }

}
