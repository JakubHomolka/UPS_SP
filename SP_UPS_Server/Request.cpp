
#include "Request.h"

/**
 *  Resolve incoming message
 */
void Request::resolve(Player *pl, std::string msg) {
    if(pl == nullptr){
        return;
    }

    msg = Utils::trim(msg);


    pl->msg_in = msg;


    vector<string> msg_parts = Utils::split(msg, "|");

    if (msg_parts.empty()) {


        cout << "Invalid message to resolve. Player:" + pl->name + " socket:" + to_string(pl->socket) +
                ". Invalid message counter: " + to_string(pl->invalid_message_counter) << endl;
        Response::sendErrorToPlayer(pl,"INVALID_MESSAGE_FORMAT");
    } else {
        //type of message
        string type = msg_parts[0];
        string name_fm = msg_parts.back();
        cout << "From: " + pl->name + " message:" + msg << endl;

        if (type == "LOGIN") {
            if (pl->state != "NEW") {
                cout << "LOGIN: Invalid operation. Player:" + pl->name + " is in state: " + pl->state +
                        ". Can not login twice." << endl;

                Response::sendErrorToPlayer1(pl,"CANNOT_LOGIN_TWICE");
            } else {
                pl->invalid_message_counter = 0;

                string name = "";
                if (msg_parts.size() > 1 && !msg_parts[1].empty()) {
                    name = msg_parts[1];

                }else{
                    Response::sendErrorToPlayer1(pl,"NAME_EMPTY");
                }

                if(!Utils::isAlpha(name)){
                    Response::sendErrorToPlayer1(pl,"INVALID_NAME");
                }
                else {

                    ClientManager::handleNewConnection(pl->socket, name);
                }
            }
        } else if (type.compare("PONG") == 0) {
            pl->ping_counter = 0;

        } else if (type.compare("FIND_GAME") == 0 && name_fm == pl->name) {

            if (pl->state.compare("LOBBY") == 0) {
                pl->invalid_message_counter = 0;
                GameManager::playerEnqueue(pl);

            } else {
                cout << "FIND_GAME: Invalid operation. Player:" + pl->name + " is in state: " + pl->state +
                        ". Can not do operation find game" << endl;

                Response::sendErrorToPlayer(pl,"CANNOT_FIND_GAME_IN_STATE_"+pl->state);
            }
        } else if (type.compare("LEAVE_GAME") == 0 && name_fm == pl->name) {


            if (pl->state.compare("WAITING") == 0) {
                pl->invalid_message_counter = 0;
                GameManager::playerDequeue(pl);

            } else if (pl->state.compare("IN_GAME") == 0) {
                pl->invalid_message_counter = 0;
                GameManager::playerLeaveGame(pl);

            } else if (pl->state.compare("END") == 0) {
                pl->invalid_message_counter = 0;
                GameManager::playerLeaveGame(pl);
            } else {
                cout << "LEAVE_GAME: Invalid operation. Player:" + pl->name + " is in state: " + pl->state +
                        ". Can not leave game" << endl;

                Response::sendErrorToPlayer(pl,"CANNOT_LEAVE_GAME_IN_STATE_"+pl->state);
            }
        } else if (type.compare("GAME_MOVE") == 0 && name_fm == pl->name) {

            if (pl->state.compare("IN_GAME") == 0) {
                if(pl->prioriti != 1){
                    cout << "It's not " + pl->name + "'s turn" << endl;
                    Response::sendErrorToPlayer(pl,"NOT_YOUR_TURN");
                    return;
                }else{
                    pl->invalid_message_counter = 0;
                }
                try {
                    int option = stoi(msg_parts[1]);
                    if (option < 1 || option > 7) {
                        throw ::exception();
                    }
                    pl->invalid_message_counter = 0;
                    GameManager::turn(pl, option);

                } catch (const exception &exc) {
                    cout << "Invalid input from player: " + pl->name + ". Msg: " + msg + ". Err: " + exc.what() << endl;

                    Response::sendErrorToPlayer(pl,"WRONG_VALUE_OF_MOVE(correct_is<1,6>)");
                }
            } else {
                cout << "GAME_MOVE: Invalid operation. Player:" + pl->name + " is in state: " + pl->state +
                        ". Can not do this operation" << endl;

                Response::sendErrorToPlayer(pl,"CANNOT_PERFORM_GAME_MOVE_IN_STATE_"+pl->state);
            }
        } else if (type.compare("START_GAME") == 0 && name_fm == pl->name) {


            if (pl->state.compare("WAITING") == 0) {
                pl->invalid_message_counter = 0;
                GameManager::startGame(pl);
            } else {
                cout << " START_GAME: Invalid operation. Player:" + pl->name + " state: " + pl->state +
                        ". Can not start game"
                     << endl;

                Response::sendErrorToPlayer(pl,"CANNOT_START_GAME_IN_STATE_"+pl->state);

            }
        } else if (type.compare("LOGOUT") == 0 && name_fm == pl->name) {


            if (pl->state.compare("LOBBY") == 0 || pl->state.compare("WAITING") == 0 || pl->state.compare("IN_GAME") == 0 || pl->state.compare("END")) {
                pl->invalid_message_counter = 0;
                ClientManager::logoutPlayer(pl->socket);
            } else {
                cout << "LOGOUT: Invalid operation. Player:" + pl->name + " state: " + pl->state + ". Can not logout"
                     << endl;

                Response::sendErrorToPlayer(pl,"CANNOT_LOGOUT_IN_STATE_"+pl->state);
            }

        } else {


            cout << "Player: " + pl->name + " unknown message type recieved or wrong name. Type:" + type +
                    ". Invalid message counter: " + to_string(pl->invalid_message_counter) << endl;
            Response::sendErrorToPlayer1(pl,"MESSAGE_FORMAT_OR_WRONG_NAME");
        }
    }
}