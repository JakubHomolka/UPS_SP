//
// Created by David Kuta on 27.11.2021.
//

#ifndef RPS_SERVER_REQUEST_H
#define RPS_SERVER_REQUEST_H


#include <cstdio>
#include <vector>
#include "Player.h"
#include "ClientManager.h"
#include "GameManager.h"
#include "Utils.h"
/**
 * Used to resolving user requests
 */
class Request {
public:
    /**
     * Handles user request by its message and player identification
     * @param pl player identification
     * @param msg message
     */
    static void resolve(Player *pl, std::string msg);
};


#endif //RPS_SERVER_REQUEST_H
