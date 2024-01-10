//
// Created by david on 13.12.2021.
//

#ifndef RPS_SERVER_GAMEMANAGER_H
#define RPS_SERVER_GAMEMANAGER_H

#include <queue>
#include <map>
#include <cstdio>
#include <vector>
#include "Player.h"
#include "Game.h"
#include "Response.h"
#include "GameLogic.h"

using namespace std;
/**
 * Handles games
 */
class GameManager {
public:
    /**
     * map of running games(was started)
     */
    static map<int, Game *> runningGames;
    /**
     * queue of players, used for pairing players
     */
    static queue<Game *> waitingGames;
    /**
     * id of the game generator. Starts by 1
     */
    static int idOfNextGame;

    /**
     * Sets player ready for play. Finds a game for the player.
     * @param pl
     */
    static void playerEnqueue(Player *pl);
    /**
        * Player wants to leave the waiting queue
        * @param pl
        */
    static void playerDequeue(Player *pl);

    /**
     * starts the game
     * @param pl player that is starting
     */
    static void startGame(Player *pl);

    /**
     * player game move (game_turn)
     * @param pl
     * @param option 1(ROCK), 2(PAPER), 3(SCISSORS)
     */
    static void turn(Player *pl, int option);

    /**
     * Leaves game and notifying othed players
     * @param pl
     */
    static void playerLeaveGame(Player *pl);

    /**
     * sends info to all players in game, about player state. Used for disconnect(error)/reconnect
     * @param pl
     * @param message
     */
    static void notifyOponent(Player *pl, string message);

    /**
     * if game exists, return pointer to it
     * @param gameId
     * @return
     */
    static Game *gameExists(int gameId);
};


#endif //RPS_SERVER_GAMEMANAGER_H
