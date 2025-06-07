#include "GameState.h"

GameState::GameState() {
    reset();
}

void GameState::reset() {
    cleanMap();
    mode1 = false;
    mode2 = false;
    mode3 = false;
    isFirst = true;
    isMenuFolded = true;
    running = true;
    currentPlayer = CHESS_BLACK;
}

void GameState::cleanMap() {
    for (auto& row : chessMap) {
        row.fill(0);
    }
}

bool GameState::isPositionEmpty(int row, int col) const {
    return chessMap[row][col] == None;
}

void GameState::setPiece(int row, int col, ChessColor color) {
    chessMap[row][col] = (color == CHESS_BLACK) ? Black : White;
}

int GameState::getPiece(int row, int col) const {
    return chessMap[row][col];
}