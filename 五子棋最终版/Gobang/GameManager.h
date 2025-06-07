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
    void handleMenuEvents(ExMessage& msg);
    void handleGameEvents(ExMessage& msg);

    // 菜单选项处理
    void handleExitOption();
    void handleSurrenderOption();
    void handleDrawOption();

    // 游戏模式处理
    void handlePlayerVsPlayer(ExMessage& msg);
    void handlePlayerVsAI(ExMessage& msg);


    // 游戏操作
    void placeChess(int row, int col);
    void showGameResult();
    void resetGame();

    // 辅助函数
    void sleepWithDrawing();

    GameState state;
    GameLogic logic;
};

#endif // GAME_MANAGER_H