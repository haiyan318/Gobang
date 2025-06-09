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
    // 事件处理
    void handleMenuEvents(ExMessage& msg); // 菜单操作与游戏模式选择
    void handleGameEvents(ExMessage& msg); // 主界面模式切换

    // 菜单选项处理
    void handleExitOption();
    void handleSurrenderOption();
    void handleDrawOption();

    // 游戏模式处理
    void handlePlayerVsPlayer(ExMessage& msg);
    void handlePlayerVsAI(ExMessage& msg); // xry


    // 游戏操作
    void placeChess(int row, int col);
    void showGameResult();
    void resetGame();


    GameState state;
    GameLogic logic;
};

#endif // GAME_MANAGER_H