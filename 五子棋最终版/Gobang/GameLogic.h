#pragma once
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "GameState.h"
#include <random>

class GameLogic {
public:
    bool judgeWin(int row_chess, int col_chess, GameState::ChessColor color, const GameState& state) const;
    void calculateBestMove(int* row, int* col, GameState& state) const;

private:
    int getScoreTable(int self, int e) const;
    int getScoreHorizontal(int r, int c, int pieces, const GameState& state) const;
    int getScoreVertical(int r, int c, int pieces, const GameState& state) const;
    int getScoreLeftDiagonal(int r, int c, int pieces, const GameState& state) const;
    int getScoreRightDiagonal(int r, int c, int pieces, const GameState& state) const;
    int getScore(int r, int c, const GameState& state) const;
};

#endif // GAME_LOGIC_H