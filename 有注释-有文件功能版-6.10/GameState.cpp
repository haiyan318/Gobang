#include "GameState.h"
#include <fstream>
#include <windows.h> // 添加Windows API支持
#include <iomanip>   // 添加格式控制支持

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
    moveHistory.clear(); // 清空历史记录
}

void GameState::cleanMap() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            chessMap[i][j] = None;
        }
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

// 添加棋步历史
void GameState::addMoveRecord(ChessColor player, int row, int col) {
    moveHistory.push_back({ player, row, col });
}

// 保存游戏记录
void GameState::saveGameRecord(const std::string& status) const {

CreateDirectoryW(L"records", NULL);
    
    // 获取当前时间作为文件名
    SYSTEMTIME st;
    GetLocalTime(&st);
    

    wchar_t fileName[256];
    swprintf(fileName, 256, L"records\\%04d%02d%02d_%02d%02d%02d.txt", 
             st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    
    // 打开文件
    std::ofstream outFile;
    
    outFile.open(fileName);
    if (!outFile) return;

    // 写入游戏模式
    outFile << "[Mode]\n";
    if (mode1) outFile << "HumanVsHuman\n";
    else if (mode2) outFile << "HumanVsAI\n";
    else if (mode3) outFile << "AIVsAI\n";
    else outFile << "Menu\n";

    // 写入当前玩家
    outFile << "\n[CurrentPlayer]\n";
    outFile << (currentPlayer == CHESS_BLACK ? "Black" : "White") << "\n";

    // 写入棋盘状态
    outFile << "\n[Board]\n";
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            outFile << chessMap[i][j] << (j < BOARD_SIZE - 1 ? " " : "");
        }
        outFile << "\n";
    }

    // 写入游戏状态
    outFile << "\n[GameStatus]\n" << status << "\n";

    // 写入棋步历史
    outFile << "\n[History]\n";
    for (size_t i = 0; i < moveHistory.size(); i++) {
        const auto& move = moveHistory[i];
        outFile << (i + 1) << " "
            << (move.player == CHESS_BLACK ? "Black" : "White") << " "
            << move.row << " " << move.col << "\n";
    }

    outFile.close();
}