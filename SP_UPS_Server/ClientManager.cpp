//
// Created by David Kuta on 27.11.2021.
//

#include "ClientManager.h"

map<int, Player *> ClientManager::unlogged_players;
map<string, Player *> ClientManager::logged_players;

void ClientManager::create_unlogged_player(char *ip, int client_socket) {
    string string_ip = ip;
    cout << "Connected new player on server with IP: " + string_ip + " on socket: " + to_string(client_socket) << endl;

    Player *pl = new Player(ip, client_socket);

    //add new connected player to unlloged players
    ClientManager::unlogged_players.insert(make_pair(client_socket, pl));

}

void ClientManager::log_player(int client_socket, string name) {
    cout << "Creating new player with name:" + name + " on socket: " + to_string(client_socket) << endl;

    auto *pl = ClientManager::get_unlogged_player(client_socket);
    ClientManager::unlogged_players.erase(client_socket);

    pl->set_name(name.data());
    pl->state = "LOBBY";
    pl->connected = 1;
    ClientManager::logged_players.insert(make_pair(name, pl));

    string message = "LOGIN|OK";
    Response::sendToPlayer(pl, message);
}

Player *ClientManager::get_unlogged_player(int id) {
    auto search = ClientManager::unlogged_players.find(id);
    if (search != ClientManager::unlogged_players.end()) {
        return search->second;
    }

    return nullptr;
}

Player *ClientManager::get_logged_player_by_socket(int id) {
    map<string, Player *>::iterator it = ClientManager::logged_players.begin();
    while (it != ClientManager::logged_players.end()) {
        auto *pl = it->second;

        if (pl->socket == id) {
            return pl;
        }

        it++;
    }

    return nullptr;
}

Player *ClientManager::get_logged_player_by_name(string name) {
    auto it = ClientManager::logged_players.begin();
    while (it != ClientManager::logged_players.end()) {
        auto *pl = it->second;

        if (pl->name == name) {
            return pl;
        }

        it++;
    }

    return nullptr;
}

void ClientManager::handleNewConnection(int sock_fd, string name) {

    cout << "Attempt to login with sock=" + to_string(sock_fd) + ", name=" + name << endl;

    if (!name.empty() && name.length() <= 10) {
        Player *pl = get_logged_player_by_name(name);

        //check if player is logged
        if (pl != NULL) {
            //if player is logged and playing
            if (pl->connected == 1 || name.empty()) {
                cout << to_string(pl->connected) << endl;
                Response::sendToClient(sock_fd, "LOGIN|ERR|NAME_USED");

            } else {

                ClientManager::reconnect(pl, sock_fd);

            }
        } else {
            //register new player
            ClientManager::log_player(sock_fd, name);


        }
    } else {
        Response::sendToClient(sock_fd, "LOGIN|ERR|INVALID_NAME");
    }

}

void ClientManager::reconnect(Player *pl, int newSockFd) {

    pl->socket = newSockFd;
    auto game = GameManager::gameExists(pl->game_id);
    if (game == nullptr) {
        pl->state = "LOBBY";
        pl->connected = 1;
        pl->ping_counter = 0;
        string message = "LOGIN|OK";
        Response::sendToPlayer(pl, message);
    } else {
        auto gamePtr = GameManager::gameExists(pl->game_id);
        pl->state1 = "CONN";

        string serializedData;
        for (auto & i : game->arr) {
            for (auto & j : i) {
                serializedData += j + ";";
            }
        }
        cout << serializedData << endl;
        if(game->gameState == "RUNNING"){
            pl->state = "IN_GAME";
            for (auto &it: gamePtr->getPlayers()) {
                if (it->name != pl->name) {
                    Response::sendToPlayer(pl, "RECONNECT|OK|" + pl->mark + "|" + it->name + "|" + it->mark + "|" + it->state1 + "|" +
                            to_string(pl->prioriti) + "|" + serializedData);
                }
            }
        }
        else{
            pl->state = "WAITING";
            for (auto &it: gamePtr->getPlayers()) {
                if (it->name != pl->name) {
                    Response::sendToPlayer(pl, "RECONNECT|OK|GAME|" + to_string(game->getGameId()) + "|" + pl->mark + "|" + it->name + "|" + it->mark + "|" +
                            to_string(pl->prioriti) + "|" + it->state1);
                }
            }
        }

        pl->connected = 1;
        GameManager::notifyOponent(pl, "CONN");

    }
}

void ClientManager::destroyPlayer(int sock_fd) {

    Player *pl = get_logged_player_by_socket(sock_fd);

    if (pl != nullptr) {
        Response::sendToPlayer(pl, "LOGOUT|OK");
        pl->connected = -1;
        ClientManager::logged_players.erase(pl->name);

        cout << pl->state << endl;

        if (pl->state == "WAITING") {
            cout << "degueue" << endl;
            GameManager::playerDequeue(pl);
        }

        else if (pl->state == "IN_GAME" || pl->state == "END") {
            cout << "leave" << endl;
            GameManager::playerLeaveGame(pl);
        }

    }

}

void ClientManager::logoutPlayer(int sock_fd) {

    Player *pl = get_logged_player_by_socket(sock_fd);

    if (pl != nullptr) {
        Response::sendToPlayer(pl, "LOGOUT|OK");
        pl->connected = -1;
        ClientManager::logged_players.erase(pl->name);
        Server::closeSocket(sock_fd);


        if (pl->state == "WAITING") {
            cout << "degueue" << endl;
            GameManager::playerDequeue(pl);
        }

        else if (pl->state == "IN_GAME" || pl->state == "END") {
            cout << "leave" << endl;
            GameManager::playerLeaveGame(pl);
        }

    }

}




