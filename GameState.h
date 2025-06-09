#pragma once
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <array>

class GameState {
public:
    enum ChessColor { CHESS_BLACK, CHESS_WHITE };
    enum PiecesType { None, Black, White };

    static const int BOARD_SIZE = 15;

    GameState();

    void reset();
    void cleanMap();
    bool isPositionEmpty(int row, int col) const;
    void setPiece(int row, int col, ChessColor color);
    int getPiece(int row, int col) const;

    // ģʽ״̬
    bool mode1 = false;
    bool mode2 = false;
    bool mode3 = false;

    // ��Ϸ״̬
    bool isFirst = true;
    bool isMenuFolded = true;
    bool running = true;
    ChessColor currentPlayer = CHESS_BLACK;

private:
    int chessMap[BOARD_SIZE][BOARD_SIZE];
};

#endif // GAME_STATE_H