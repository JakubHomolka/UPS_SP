//
// Created by David Kuta on 08.12.2021.
//

#ifndef RPS_SERVER_RESPONSE_H
#define RPS_SERVER_RESPONSE_H

#include "Player.h"
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
/**
 * Used as response to the request
 */
class Response {

public:
    /**
     * sends a message to the client by its socket file descriptor
     * @param sockFd
     * @param message
     */
    static void sendToClient(int sockFd, string message);
    /**
     * sends a message to the player
     * @param sockFd
     * @param message
     */
    static void sendToPlayer(Player *player, string message);

    /**
     * Sends specified error message to the player with error message counter
     * @param player
     * @param errorMessage
     */
    static void sendErrorToPlayer(Player *player, string errorMessage);

    /**
     * Sends specified error message to the player with error message counter
     * @param player
     * @param errorMessage
     */
    static void sendErrorToPlayer1(Player *player, string errorMessage);

};


#endif //RPS_SERVER_RESPONSE_H
