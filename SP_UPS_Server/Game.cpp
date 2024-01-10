//
// Created by david on 13.12.2021.
//

#include "Game.h"

Game::Game(int gameId) {
    this->gameId = gameId;
    this->gameState = "WAITING";
    this->counter = 0;
    this->c1=6;
    this->c2=6;
    this->c3=6;
    this->c4=6;
    this->c5=6;
    this->c6=6;
    this->c7=6;

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 7; j++) {
            this->arr[i][j] = " ";
        }
    }

}

int Game::getGameId() {
    return this->gameId;
}

void Game::addPlayer(Player *pl) {
    if (pl == nullptr) {
        return;
    }

    this->players.push_back(pl);
}

bool Game::isFull() {
    if (this->players.size() == MAX_PLAYERS) {
        return true;
    }
    return false;
}

void Game::removePlayer(Player *pl) {

    if (pl == nullptr) {
        return;
    }
    int position = 0;
    for (auto &it: players) {
        if (it->name == pl->name) {
            cout << "Player with name " + it->name + " is remove" << endl;
            players.erase(players.begin() + position);

        }
        position++;
    }

}


bool Game::isEmpty() {
    return getPlayers().empty();
}

vector<Player *> Game::getPlayers() {
    return this->players;
}


