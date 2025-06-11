#pragma once
#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "GameState.h"
#include "GameLogic.h"
#include<easyx.h>
class GameManager {
public:
    GameManager();
    void run();

private:
 
    void handleMenuEvents(ExMessage& msg); 
    void handleGameEvents(ExMessage& msg); 

    void handleExitOption();
    void handleSurrenderOption();
    void handleDrawOption();

 
    void handlePlayerVsPlayer(ExMessage& msg);
    void handlePlayerVsAI(ExMessage& msg); 



    void placeChess(int row, int col);
    void showGameResult();
    void resetGame();


    GameState state;
    GameLogic logic;
};

#endif 