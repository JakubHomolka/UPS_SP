
#ifndef RPS_SERVER_PLAYER_H
#define RPS_SERVER_PLAYER_H

#include <cstdio>
#include <iostream>

using namespace std;

/**
 * Representation of player
 */
class Player {
public:
    /**
     * ip of player, used for debug
     */
    char *ip;
    /**
     * socket id for communication
     */
    int socket;
    /**
     * id of currently participated game
     */
    int game_id;
    /**
     * id of game_turn 1(ROCK),2(PAPER),3(SCISSORS),-1(NOT_PLAYED_YET)
     */
    int game_move;

    int prioriti;
    /**
     * If player is connected = 1, if not = -1
     */
    int connected;

    /**
     * used for validating user input. After 3 invalid messages is player disconnected
     */
    int invalid_message_counter;

    int end;

    int ping_counter;


    string mark;
    /**
     * player name
     */
    string name;
    /**
     * player last send request, used for debug
     */
    string msg_in;
    /**
     * player last send response, used for debug
     */
    string msg_out;

    /**
     * Player state NEW, LOBBY, INGAME, WAITING
     */
    string state;

    /**
     * Player state CONN, DISCONN
     */
    string state1;

    Player(char *ip, int socket);

    ~Player();

    void set_name(string new_name) { name = new_name; };
};


#endif //RPS_SERVER_PLAYER_H
