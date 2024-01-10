//
// Created by david on 13.12.2021.
//

#ifndef RPS_SERVER_GAME_H
#define RPS_SERVER_GAME_H

#define MAX_PLAYERS 2
#define ROCK 1
#define PAPER 2
#define SCISSORS 3

#include <vector>
#include <string>
#include "Player.h"


using namespace std;
/**
 * Representation of game
 */
class Game {
public:
    /**
     * game state(WAITING or RUNNING)
     */
    string gameState;

    //the board
    string arr[6][7];
    //cheak the places of board
    int c1;
    int c2;
    int c3;
    int c4;
    int c5;
    int c6;
    int c7;

    int counter;
    /**
     * adds player to the game players
     * @param pl
     */
    void addPlayer(Player *pl);

    /**
     * remove player from the game
     * @param pl
     */
    void removePlayer(Player *pl);

    /**
     * Fullnes of the game
     * @return true if yes, false if no
     */
    bool isFull();

    /**
     * Emptines of the game
     * @return  true if yes, false if no
     */
    bool isEmpty();

    /**
     *
     * @return vector of players ingame (connected and not connected(resulted by conn error))
     */
    vector<Player *> getPlayers();

    /**
     *
     * @return id
     */
    int getGameId();


    explicit Game(int gameId);

private:
    /**
     * struct for players
     */
    vector<Player *> players;

    /**
     * Identification of the game.
     */
    int gameId;


};


#endif //RPS_SERVER_GAME_H
