#include "GameLogic.h"
#include <algorithm>

bool GameLogic::judgeWin(int row_chess, int col_chess, GameState::ChessColor color, const GameState& state) const {
    int currentColor = (color == GameState::CHESS_BLACK) ? GameState::Black : GameState::White;

    int directions[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };

    for (int i = 0; i < 4; i++) {
        int dx = directions[i][0];
        int dy = directions[i][1];

        int count = 1;

        for (int step = 1; step < 5; step++) {
            int r = row_chess + dx * step, c = col_chess + dy * step;
            if (r < 0 || r >= GameState::BOARD_SIZE || c < 0 || c >= GameState::BOARD_SIZE ||
                state.getPiece(r, c) != currentColor)
                break;
            count++;
        }

        for (int step = 1; step < 5; step++) {
            int r = row_chess - dx * step, c = col_chess - dy * step;
            if (r < 0 || r >= GameState::BOARD_SIZE || c < 0 || c >= GameState::BOARD_SIZE ||
                state.getPiece(r, c) != currentColor)
                break;
            count++;
        }

        if (count >= 5) {
            return true;
        }
    }

    return false;
}

int GameLogic::getScoreTable(int self, int e) const {
    if (self > 5) return 200000;
    if (self == 5 && e == 0) return 200000;
    if (self == 5 && e == 1) return 200000;
    if (self == 5 && e == 2) return 200000;

    if (self == 4 && e == 0) return 50000;
    if (self == 4 && e == 1) return 40000;
    if (self == 4 && e == 2) return 1000;

    if (self == 3 && e == 0) return 30000;
    if (self == 3 && e == 1) return 1000;
    if (self == 3 && e == 2) return 500;

    if (self == 2 && e == 0) return 500;
    if (self == 2 && e == 1) return 200;
    if (self == 2 && e == 2) return 100;

    if (self == 1 && e == 0) return 100;
    if (self == 1 && e == 1) return 50;
    if (self == 1 && e == 2) return 30;
    return 0;
}

int GameLogic::getScoreHorizontal(int r, int c, int pieces, const GameState& state) const {
    int self = 1;
    int ai = 0;
    for (int i = c - 1; i >= 0; i--) {
        if (state.getPiece(r, i) == pieces) {
            self++;
        }
        else if (state.getPiece(r, i) == GameState::None) {
            break;
        }
        else {
            ai++;
            break;
        }
    }

    for (int i = c + 1; i < GameState::BOARD_SIZE; i++) {
        if (state.getPiece(r, i) == pieces) {
            self++;
        }
        else if (state.getPiece(r, i) == GameState::None) {
            break;
        }
        else {
            ai++;
            break;
        }
    }
    return getScoreTable(self, ai);
}

int GameLogic::getScoreVertical(int r, int c, int pieces, const GameState& state) const {
    int self = 1;
    int ai = 0;

    for (int i = r - 1; i >= 0; i--) {
        if (state.getPiece(i, c) == pieces) {
            self++;
        }
        else if (state.getPiece(i, c) == GameState::None) {
            break;
        }
        else {
            ai++;
            break;
        }
    }

    for (int i = r + 1; i < GameState::BOARD_SIZE; i++) {
        if (state.getPiece(i, c) == pieces) {
            self++;
        }
        else if (state.getPiece(i, c) == GameState::None) {
            break;
        }
        else {
            ai++;
            break;
        }
    }
    return getScoreTable(self, ai);
}

int GameLogic::getScoreLeftDiagonal(int r, int c, int pieces, const GameState& state) const {
    int self = 1;
    int ai = 0;
    for (int i = r + 1, k = c - 1; i < GameState::BOARD_SIZE && k >= 0; i++, k--) {
        if (state.getPiece(i, k) == pieces) {
            self++;
        }
        else if (state.getPiece(i, k) == GameState::None) {
            break;
        }
        else {
            ai++;
            break;
        }
    }

    for (int i = r - 1, k = c + 1; i >= 0 && k < GameState::BOARD_SIZE; i--, k++) {
        if (state.getPiece(i, k) == pieces) {
            self++;
        }
        else if (state.getPiece(i, k) == GameState::None) {
            break;
        }
        else {
            ai++;
            break;
        }
    }
    return getScoreTable(self, ai);
}

int GameLogic::getScoreRightDiagonal(int r, int c, int pieces, const GameState& state) const {
    int self = 1;
    int ai = 0;
    for (int i = r + 1, k = c + 1; i < GameState::BOARD_SIZE && k < GameState::BOARD_SIZE; i++, k++) {
        if (state.getPiece(i, k) == pieces) {
            self++;
        }
        else if (state.getPiece(i, k) == GameState::None) {
            break;
        }
        else {
            ai++;
            break;
        }
    }

    for (int i = r - 1, k = c - 1; i >= 0 && k >= 0; i--, k--) {
        if (state.getPiece(i, k) == pieces) {
            self++;
        }
        else if (state.getPiece(i, k) == GameState::None) {
            break;
        }
        else {
            ai++;
            break;
        }
    }
    return getScoreTable(self, ai);
}

int GameLogic::getScore(int r, int c, const GameState& state) const {
    int numh1 = getScoreHorizontal(r, c, GameState::Black, state);
    int numh2 = getScoreHorizontal(r, c, GameState::White, state);
    int numv1 = getScoreVertical(r, c, GameState::Black, state);
    int numv2 = getScoreVertical(r, c, GameState::White, state);
    int numlh1 = getScoreLeftDiagonal(r, c, GameState::Black, state);
    int numlh2 = getScoreLeftDiagonal(r, c, GameState::White, state);
    int numrh1 = getScoreRightDiagonal(r, c, GameState::Black, state);
    int numrh2 = getScoreRightDiagonal(r, c, GameState::White, state);

    if (numh1 >= 200000 || numh2 >= 200000 || numv1 >= 200000 || numv2 >= 200000 ||
        numlh1 >= 200000 || numlh2 >= 200000 || numrh1 >= 200000 || numrh2 >= 200000) {
        return INT_MAX;
    }

    int xscore = numh1 + numh2;
    int yscore = numv1 + numv2;
    int lhscore = numlh1 + numlh2;
    int rhscore = numrh1 + numrh2;

    return xscore + yscore + lhscore + rhscore;
}

void GameLogic::calculateBestMove(int* row, int* col, GameState& state) const {
    int max = 0;
    for (int i = 0; i < GameState::BOARD_SIZE; i++) {
        for (int k = 0; k < GameState::BOARD_SIZE; k++) {
            if (!state.isPositionEmpty(i, k))
                continue;

            int score = getScore(i, k, state);
            if (score >= 200000) {
                *row = i;
                *col = k;
                return;
            }
            if (score > max) {
                max = score;
                *row = i;
                *col = k;
            }
        }
    }

    if (*row == 0 && *col == 0 && state.isFirst) {
        *row = GameState::BOARD_SIZE / 2;
        *col = GameState::BOARD_SIZE / 2;
        state.isFirst = false;
    }
}

