#include "GameState.h"
#include <fstream>
#include <windows.h> 
#include <iomanip>   

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
    moveHistory.clear(); 
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


void GameState::addMoveRecord(ChessColor player, int row, int col) {
    moveHistory.push_back({ player, row, col });
}

void GameState::saveGameRecord(const std::string& status) const {

CreateDirectoryW(L"records", NULL);
    
   
    SYSTEMTIME st;
    GetLocalTime(&st);
    

    wchar_t fileName[256];
    swprintf(fileName, 256, L"records\\%04d%02d%02d_%02d%02d%02d.txt", 
             st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    

    std::ofstream outFile;
    
    outFile.open(fileName);
    if (!outFile) return;


    outFile << "[Mode]\n";
    if (mode1) outFile << "HumanVsHuman\n";
    else if (mode2) outFile << "HumanVsAI\n";
    else if (mode3) outFile << "AIVsAI\n";
    else outFile << "Menu\n";

   
    outFile << "\n[CurrentPlayer]\n";
    outFile << (currentPlayer == CHESS_BLACK ? "Black" : "White") << "\n";

    
    outFile << "\n[Board]\n";
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            outFile << chessMap[i][j] << (j < BOARD_SIZE - 1 ? " " : "");
        }
        outFile << "\n";
    }

    
    outFile << "\n[GameStatus]\n" << status << "\n";

    
    outFile << "\n[History]\n";
    for (size_t i = 0; i < moveHistory.size(); i++) {
        const auto& move = moveHistory[i];
        outFile << (i + 1) << " "
            << (move.player == CHESS_BLACK ? "Black" : "White") << " "
            << move.row << " " << move.col << "\n";
    }

    outFile.close();
}