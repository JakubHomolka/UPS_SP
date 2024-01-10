//
// Created by David Kuta on 26.11.2021.
//

#include "Player.h"

/**
 *  Constructor for player
 */
Player::Player(char *ip, int socket) {
    this->ip = ip;
    this->socket = socket;
    this->name = "";
    this->connected = 1;
    this->game_id = 0;
    this->game_move = -1;
    this->prioriti = 0;
    this->mark = "";
    this->state = "NEW";
    this->state1 = "CONN";
    this->invalid_message_counter = 0;
    this->end = 0;
    this->ping_counter = 0;
    cout << "New player created on socket: " + to_string(socket) << endl;
}

Player::~Player() {
    cout << "Player on socket: " + to_string(this->socket) + " has been deleted" << endl;
}


