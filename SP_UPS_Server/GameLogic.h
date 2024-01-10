//
// Created by david on 19.12.2021.
//

#ifndef RPS_SERVER_GAMELOGIC_H
#define RPS_SERVER_GAMELOGIC_H


#include "Game.h"
/**
 * Used for game rules and score counting
 */
class GameLogic {
public:
    static int insert(int x, Game *game);

    static bool win_p1_row (int a, string m, Game *game);

    static bool win_p1_dai1 (int a , int b, string m, Game *game);

    static bool win_p1_dai2 (int a , int b, string m, Game *game);

    static bool win_p1_col (int a, string m, Game *game);

    static void insertboard(int a, int b, string x, Game *game);

    static int Connect4(Game *game, Player *pl);
};


#endif //RPS_SERVER_GAMELOGIC_H
