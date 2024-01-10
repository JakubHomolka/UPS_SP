//
// Created by david on 19.12.2021.
//

#include "GameLogic.h"
int GameLogic::insert(int x, Game *game)
{

    //cheak values
    if(x==1){
        if (game->c1>0) {
            game->c1--;
            return game->c1;
        }
        else
            return -1;
    }
    else if(x==2){
        if (game->c2>0){
            game->c2--;
            return game->c2;
        }
        else
            return -1;
    }
    else if(x==3){
        if (game->c3>0){
            game->c3--;
            return game->c3;
        }
        else
            return -1;
    }
    else if(x==4){
        if (game->c4>0)
        {
            game->c4--;
            return game->c4;
        }
        else
            return -1;
    }
    else if(x==5){
        if (game->c5>0){
            game->c5--;
            return game->c5;
        }
        else
            return -1;
    }
    else if(x==6){
        if (game->c6>0){
            game->c6--;
            return game->c6;
        }
        else
            return -1;
    }
    else if(x==7){
        if (game->c7>0){
            game->c7--;
            return game->c7;
        }
        else
            return -1;
    }
    else
        return -1;

}

bool GameLogic::win_p1_row (int a, string m, Game *game)//1st case to win p1
{
    int count=0;
    for(int j=0; j<7;j++){
        if (game->arr[a][j]==m)
        {
            count++;
            if (count==4)
                return true ;
        }
        else
            count =0;
    }
    return false;

}

bool GameLogic::win_p1_dai1 (int a , int b, string m, Game *game)//2nd case to win p1
{
    int count=0;
    b--;
    if (a>=b)
    {
        a-=b;
        b=0;
        while(a<6)
        {
            if (game->arr[a][b]==m)
            {
                count++;
                if (count==4)
                    return true ;
            }
            else
                count =0;
            a++;
            b++;
        }
    }
    else
    {
        b-=a;
        a=0;
        while(b<7)
        {
            if (game->arr[a][b]==m)
            {
                count++;
                if (count==4)
                    return true ;
            }
            else
                count =0;
            a++;
            b++;
        }
    }
    return false;

}

bool GameLogic::win_p1_dai2 (int a , int b, string m, Game *game)//3rd case to win p1
{
    int count=0;
    b--;
    int c =a+b;
    if (c<7 && c>2)
    {
        b=c;
        a=0;
        while(a<6)
        {
            if (game->arr[a][b]==m)
            {
                count++;
                if (count==4)
                    return true ;
            }
            else
                count =0;
            a++;
            b--;
        }
    }
    else if (c>6 && c<9)
    {
        b=6;
        a=c-6;
        while(a<6)
        {
            if (game->arr[a][b]==m)
            {
                count++;
                if (count==4)
                    return true ;
            }
            else
                count =0;
            a++;
            b--;
        }
    }
    return false;

}

bool GameLogic::win_p1_col (int a, string m, Game *game)//4th case to win p1
{
    int count=0;
    a--;
    for(int j=0; j<6;j++)
    {
        if (game->arr[j][a]==m)
        {
            count++;
            if (count==4)
                return true ;
        }
        else
            count =0;
    }
    return false;

}


void GameLogic::insertboard(int a, int b, string x, Game *game)//show the board
{
    //the move
    game->arr[a][b-1]=x;

    for(int f=1; f<8;f++)
    {
        cout<<f<<'|';
    }
    cout<<endl;
    //the board
    for(int i=0;i<6;i++)
    {
        for(int j =0;j<7;j++)
        {
            cout<<game->arr[i][j]<<'|';
        }
        cout<<endl;
    }
}

int GameLogic::Connect4(Game *game, Player *pl){

        auto c = game->counter;

        int row = 0;
        auto m = pl->mark;
        auto col = pl->game_move;


        row = insert(col, game);//calculate the row
        cout << row << endl;
        while (row==-1){
             cout << "invalid place"<< endl;
            return 0;
        }
        insertboard(row, col, m, game);

        //cheak the p1 is winner or no
        if (win_p1_row (row, m, game)==true)
        {
            cout<<"Player " + pl->name + " is Winner"<<endl;
            return 1;
        }

        if (win_p1_col (col, m, game)==true)
        {
            cout<<"Player " + pl->name + " is Winner"<<endl;
            return 1;
        }

        if (win_p1_dai1 (row,col, m, game)==true)
        {
            cout<<"Player " + pl->name + " is Winner"<<endl;
            return 1;
        }
        if (win_p1_dai2 (row,col, m, game)==true)
        {
            cout<<"Player " + pl->name + " is Winner"<<endl;
            return 1;
        }
        game->counter++;
        if (c==42)
        {
            cout<<"DRAW";
            return 2;
        }

        return 3;
}


