#pragma once
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <string> // 添加string支持
#include <ctime>  // 添加时间支持

class GameState {
public:
    enum ChessColor { CHESS_BLACK, CHESS_WHITE };
    enum PiecesType { None, Black, White };

    static const int BOARD_SIZE = 15;

    // 添加棋步历史记录结构
    struct MoveRecord {
        ChessColor player;
        int row;
        int col;
    };

    GameState();

    void reset();
    void cleanMap();
    bool isPositionEmpty(int row, int col) const;
    void setPiece(int row, int col, ChessColor color);
    int getPiece(int row, int col) const;

    // 添加保存游戏记录功能
    void saveGameRecord(const std::string& status) const;
    void addMoveRecord(ChessColor player, int row, int col);

    bool mode1 = false;
    bool mode2 = false;
    bool mode3 = false;

    bool isFirst = true;
    bool isMenuFolded = true;
    bool running = true;
    ChessColor currentPlayer = CHESS_BLACK;

private:
    int chessMap[BOARD_SIZE][BOARD_SIZE];
    std::vector<MoveRecord> moveHistory; // 存储棋步历史
};

#endif