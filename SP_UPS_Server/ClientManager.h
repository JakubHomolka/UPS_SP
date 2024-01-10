//
// Created by David Kuta on 27.11.2021.
//

#ifndef RPS_SERVER_CLIENTMANAGER_H
#define RPS_SERVER_CLIENTMANAGER_H

#include <cstdio>
#include <map>
#include <vector>
#include <stack>
#include <memory>
#include <string>
#include "Player.h"
#include "Server.h"
#include "Response.h"
#include "GameManager.h"

/**
 * Manipulates with clients (players)
 */
class ClientManager {
public:
    /**
     * Used for handling unlogged players(token login not send yet)
     */
    static map<int, Player *> unlogged_players;
    /**
     * Used for handling logged players(token login send yet)
     */
    static map<string, Player *> logged_players;

    /**
     * gets logged player from map
     * @param socket_id socket id of player
     * @return Player object
     */
    static Player *get_logged_player_by_socket(int socket_id);

    /**
     * gets unlogged player from map
     * @param id socket id of player
     * @return Player object
     */
    static Player *get_unlogged_player(int id);

    /**
     * creates unlogged player in the map of unlogged players
     * @param ip client ip
     * @param client_socket
     */
    static void create_unlogged_player(char *ip, int client_socket);

    /**
     * returns player from logged players map by name
     * @param name name of player
     * @return Player object
     */
    static Player *get_logged_player_by_name(string name);

    /**
     * Used for log player, basicaly remove from unlogged players and move to logged players map
     * @param client_socket
     * @param name
     */
    static void log_player(int client_socket, string name);

    /**
     * Handles new connection of player. if player is logged and disconnected, then reconnect.
     * If client is unlogged, then log
     * @param sock_fd
     * @param name
     */
    static void handleNewConnection(int sock_fd, string name);

    /**
     * Reconnects player, with connected status -1
     * @param pl
     * @param newSockFd
     */
    static void reconnect(Player *pl, int newSockFd);

    /**
     * removes player and all of its information from the server
     * @param sock_fd
     */
    static void logoutPlayer(int sock_fd);

    static void destroyPlayer(int sock_fd);

};


#endif //RPS_SERVER_CLIENTMANAGER_H
