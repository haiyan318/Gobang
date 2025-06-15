#include "GameManager.h"
#include "UIManager.h"
#include <Windows.h>

DWORD modeStartTime = 0;
bool modeDelayPassed = false;
DWORD lastAITime = 0;

GameManager::GameManager() {
	init();
	state.reset();
}

void GameManager::run() {
	ExMessage msg;
	DWORD lastAITime = 0;
	while (state.running) {

		while (peekmessage(&msg, EX_MOUSE)) {
	
			if (msg.message == WM_CLOSE) {
	
				if (state.mode1 || state.mode2 || state.mode3) {
					state.saveGameRecord("ExitByClose");
				}
				state.running = false;
				return; 
			}

			handleMenuEvents(msg);

			if (modeDelayPassed) {
				handleGameEvents(msg);
			}
		}


		DWORD currentTime = GetTickCount();
		if (modeStartTime != 0 && !modeDelayPassed) {

			if (currentTime - modeStartTime >= 500) {
				modeDelayPassed = true;

				lastAITime = 0;

				if (state.mode3) {
					lastAITime = currentTime - 400;
				}
			}
		}


		if (state.mode3 && modeDelayPassed) {

			if (lastAITime == 0 || currentTime - lastAITime > 400) {
				lastAITime = currentTime;

				int row, col;
				logic.calculateBestMove(&row, &col, state);


				if (state.isPositionEmpty(row, col)) {
					placeChess(row, col);

					if (logic.judgeWin(row, col, state.currentPlayer, state)) {
						showGameResult();
						continue;
					}

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

	if (msg.message == WM_LBUTTONDOWN &&
		!state.mode1 && !state.mode2 && !state.mode3) {

		if (msg.x >= 420 && msg.x <= 550 && msg.y >= 190 && msg.y <= 230) {
			state.mode1 = true;
			modeStartTime = GetTickCount();
			modeDelayPassed = false;
			reloadBoard();

		}
		else if (msg.x >= 420 && msg.x <= 550 && msg.y >= 240 && msg.y <= 280) {
			state.mode2 = true;
			modeStartTime = GetTickCount();
			modeDelayPassed = false;
			reloadBoard();
		}
		else if (msg.x >= 420 && msg.x <= 550 && msg.y >= 290 && msg.y <= 330) {
			state.mode3 = true;
			modeStartTime = GetTickCount();
			modeDelayPassed = false;
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


void GameManager::showGameResult() {

	state.saveGameRecord(
		(state.currentPlayer == GameState::CHESS_BLACK) ?
		"BlackWin" : "WhiteWin");

	MessageBox(GetHWnd(),
		(state.currentPlayer == GameState::CHESS_BLACK) ?
		_T("黑方胜利！") : _T("白方胜利！"),
		_T("游戏结束"),
		MB_OK);
	resetGame();
}

void GameManager::handleSurrenderOption() {
	int result = MessageBox(GetHWnd(), _T("确定要认输吗？"), _T("确认认输"), MB_OKCANCEL | MB_ICONQUESTION);
	if (result == IDOK) {

		state.saveGameRecord(
			(state.currentPlayer == GameState::CHESS_BLACK) ?
			"BlackSurrender" : "WhiteSurrender");

		MessageBox(GetHWnd(),
			(state.currentPlayer == GameState::CHESS_BLACK) ?
			_T("白方胜利！") : _T("黑方胜利！"),
			_T("游戏结束"),
			MB_OK);
		resetGame();
	}
}

void GameManager::handleDrawOption() {
	int result = MessageBox(GetHWnd(), _T("确定要和棋吗？"), _T("确认和棋"), MB_OKCANCEL | MB_ICONQUESTION);
	if (result == IDOK) {
	
		state.saveGameRecord("Draw");

		MessageBox(GetHWnd(),
			_T("游戏和棋！"),
			_T("游戏结束"),
			MB_OK);
		resetGame();
	}
}

void GameManager::handleExitOption() {
	int result = MessageBox(GetHWnd(), _T("确定要退出当前游戏吗？"), _T("确认退出"), MB_OKCANCEL | MB_ICONQUESTION);
	if (result == IDOK) {
	
		if (state.mode1 || state.mode2 || state.mode3) {
			state.saveGameRecord("Exit");
		}

		resetGame();
		reloadBoard();
		reloadModeOptions();
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


	state.addMoveRecord(state.currentPlayer, row, col);

	FlushBatchDraw();
}


void GameManager::resetGame() {
	state.reset();
	reloadBoard();
	reloadModeOptions();
}
