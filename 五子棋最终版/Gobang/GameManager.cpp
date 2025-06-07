#include "GameManager.h"
#include "UIManager.h"
#include <Windows.h>

DWORD modeStartTime = 0; // 记录进入任何模式的时间
bool modeDelayPassed = false; // 是否已过模式切换延迟
DWORD lastAITime = 0; // 记录上次AI下棋的时间

GameManager::GameManager() {
    init();
    state.reset();
}

void GameManager::run() {
    ExMessage msg;
    DWORD lastAITime = 0;
    while (state.running) {
        // 处理所有消息
        while (peekmessage(&msg, -1)) {
            handleMenuEvents(msg);
            // 延迟期间不处理游戏事件
            if (modeDelayPassed) {
                handleGameEvents(msg);
            }
        }

        // 检查模式开始时间
        DWORD currentTime = GetTickCount();
        if (modeStartTime != 0 && !modeDelayPassed) {
            // 等待500毫秒后才开始处理游戏事件
            if (currentTime - modeStartTime >= 500) {
                modeDelayPassed = true;

                // 重置AI计时器
                lastAITime = 0;

                // 如果是AIVsAI模式，立即开始下棋
                if (state.mode3) {
                    lastAITime = currentTime - 400; // 确保立即下第一步棋
                }
            }
        }

        // 检查AI下棋时间（仅AIVsAI模式且在延迟后）
        if (state.mode3 && modeDelayPassed) {
            // 第一次下棋或距离上次下棋超过400毫秒
            if (lastAITime == 0 || currentTime - lastAITime > 400) {
                lastAITime = currentTime;

                // 执行AI下棋
                int row, col;
                logic.calculateBestMove(&row, &col, state);

                // 确保位置是空的
                if (state.isPositionEmpty(row, col)) {
                    placeChess(row, col);

                    if (logic.judgeWin(row, col, state.currentPlayer, state)) {
                        showGameResult();
                        continue; // 游戏结束后重置
                    }

                    // 切换玩家
                    state.currentPlayer = (state.currentPlayer == GameState::CHESS_BLACK) ?
                        GameState::CHESS_WHITE : GameState::CHESS_BLACK;
                }
            }
        }
        Sleep(10);
    }

    EndBatchDraw();
}

void GameManager::handleMenuEvents(ExMessage& msg) {
    // 菜单展开/折叠处理
    if (msg.message == WM_LBUTTONDOWN && state.isMenuFolded &&
        msg.x >= 50 && msg.x <= 126 && msg.y >= 0 && msg.y <= 74) {

        state.isMenuFolded = false;
        IMAGE img_btn_more_detai2;
        loadimage(&img_btn_more_detai2, _T("Resource/images/btn_more_detail2.png"));
        putimage(50, 74, &img_btn_more_detai2);
        FlushBatchDraw();
    }
    else if (msg.message == WM_LBUTTONDOWN && !state.isMenuFolded &&
        msg.x >= 50 && msg.x <= 126 && msg.y >= 0 && msg.y <= 74) {

        setfillcolor(RGB(138, 97, 59));
        solidrectangle(50, 74, 160, 600);
        IMAGE img_black;
        loadimage(&img_black, _T("Resource/images/black.png"));
        putimage(50, 480, &img_black);
        FlushBatchDraw();
        state.isMenuFolded = true;

        if (!state.mode1 && !state.mode2 && !state.mode3) {
            reloadModeOptions();
        }
    }

    // 菜单按钮点击
    if (!state.isMenuFolded) {
        if (msg.message == WM_LBUTTONDOWN &&
            msg.x >= 70 && msg.x <= 145 && msg.y >= 99 && msg.y <= 184) {
            handleExitOption();
        }
        else if (msg.message == WM_LBUTTONDOWN &&
            msg.x >= 70 && msg.x <= 145 && msg.y >= 194 && msg.y <= 279) {
            handleSurrenderOption();
        }
        else if (msg.message == WM_LBUTTONDOWN &&
            msg.x >= 70 && msg.x <= 145 && msg.y >= 289 && msg.y <= 374) {
            handleDrawOption();
        }
    }

    // 游戏模式选择
    if (msg.message == WM_LBUTTONDOWN &&
        !state.mode1 && !state.mode2 && !state.mode3) {

        if (msg.x >= 420 && msg.x <= 550 && msg.y >= 190 && msg.y <= 230) {
            state.mode1 = true;
            modeStartTime = GetTickCount(); // 设置模式开始时间
            modeDelayPassed = false; // 重置延迟标志
            reloadBoard();
            
        }
        else if (msg.x >= 420 && msg.x <= 550 && msg.y >= 240 && msg.y <= 280) {
            state.mode2 = true;
            modeStartTime = GetTickCount(); // 设置模式开始时间
            modeDelayPassed = false; // 重置延迟标志
            reloadBoard();
        }
        else if (msg.x >= 420 && msg.x <= 550 && msg.y >= 290 && msg.y <= 330) {
            state.mode3 = true;
            modeStartTime = GetTickCount(); // 设置模式开始时间
            modeDelayPassed = false; // 重置延迟标志
            reloadBoard();
        }
        else if (msg.x >= 420 && msg.x <= 550 && msg.y >= 340 && msg.y <= 380) {
            state.running = false;
        }
    }
}

void GameManager::handleGameEvents(ExMessage& msg) {
    if (state.mode1) {
        handlePlayerVsPlayer(msg);
    }
    else if (state.mode2) {
        handlePlayerVsAI(msg);
    }

    else if (msg.message == WM_CLOSE) {
        state.running = false;
    }
}

void GameManager::handleExitOption() {
    int result = MessageBox(GetHWnd(), _T("您确定要退出吗？"), _T("确认退出"), MB_OKCANCEL | MB_ICONQUESTION);
    if (result == IDOK) {
        resetGame();
        reloadBoard();
        reloadModeOptions();
    }
}

void GameManager::handleSurrenderOption() {
    int result = MessageBox(GetHWnd(), _T("您确定要认输吗？"), _T("确认认输"), MB_OKCANCEL | MB_ICONQUESTION);
    if (result == IDOK) {
        MessageBox(GetHWnd(),
            (state.currentPlayer == GameState::CHESS_BLACK) ?
            _T("黑棋认输\n白方胜利！") : _T("白棋认输\n黑方胜利！"),
            _T("游戏结束"),
            MB_OK);
        resetGame();
    }
}

void GameManager::handleDrawOption() {
    int result = MessageBox(GetHWnd(), _T("您确定要求和吗？"), _T("确认求和"), MB_OKCANCEL | MB_ICONQUESTION);
    if (result == IDOK) {
        MessageBox(GetHWnd(),
            (state.currentPlayer == GameState::CHESS_BLACK) ?
            _T("黑棋求和\n平局！") : _T("白棋求和\n平局！"),
            _T("游戏结束"),
            MB_OK);
        resetGame();
    }
}

void GameManager::handlePlayerVsPlayer(ExMessage& msg) {
    if (msg.message != WM_LBUTTONDOWN) return;
    int row_chess = (msg.x - 220 + 19) / 38;
    int col_chess = (msg.y - 35 + 19) / 38;

    if (row_chess < 0 || row_chess >= GameState::BOARD_SIZE ||
        col_chess < 0 || col_chess >= GameState::BOARD_SIZE ||
        !state.isPositionEmpty(row_chess, col_chess)) {
        return;
    }

    placeChess(row_chess, col_chess);

    if (logic.judgeWin(row_chess, col_chess, state.currentPlayer, state)) {
        showGameResult();
    }

    // 切换玩家
    state.currentPlayer = (state.currentPlayer == GameState::CHESS_BLACK) ?
        GameState::CHESS_WHITE : GameState::CHESS_BLACK;
}

void GameManager::handlePlayerVsAI(ExMessage& msg) {
    if (state.currentPlayer == GameState::CHESS_BLACK && msg.message == WM_LBUTTONDOWN) {
        int row_chess = (msg.x - 220 + 19) / 38;
        int col_chess = (msg.y - 35 + 19) / 38;

        if (row_chess < 0 || row_chess >= GameState::BOARD_SIZE ||
            col_chess < 0 || col_chess >= GameState::BOARD_SIZE ||
            !state.isPositionEmpty(row_chess, col_chess)) {
            return;
        }

        placeChess(row_chess, col_chess);

        if (logic.judgeWin(row_chess, col_chess, state.currentPlayer, state)) {
            showGameResult();
            return;
        }

        state.currentPlayer = GameState::CHESS_WHITE;
        Sleep(100);
    }

    if (state.currentPlayer == GameState::CHESS_WHITE) {
        int row, col;
        logic.calculateBestMove(&row, &col, state);
        placeChess(row, col);

        if (logic.judgeWin(row, col, state.currentPlayer, state)) {
            showGameResult();
            return;
        }

        state.currentPlayer = GameState::CHESS_BLACK;
    }
}


void GameManager::placeChess(int row, int col) {
    if (state.currentPlayer == GameState::CHESS_BLACK)
        setfillcolor(RGB(0, 0, 0));
    else
        setfillcolor(RGB(255, 255, 255));

    int gridX = 220 + row * 38;
    int gridY = 35 + col * 38;
    solidcircle(gridX, gridY, 17);

    state.setPiece(row, col, state.currentPlayer);

    FlushBatchDraw();
}

void GameManager::showGameResult() {
    MessageBox(GetHWnd(),
        (state.currentPlayer == GameState::CHESS_BLACK) ?
        _T("黑方胜利！") : _T("白方胜利！"),
        _T("游戏结束"),
        MB_OK);
    resetGame();
}

void GameManager::resetGame() {
    state.reset();
    reloadBoard();
    reloadModeOptions();
}

void GameManager::sleepWithDrawing() {
    // 如果需要保持绘制，可以在这里实现
    Sleep(100);
}