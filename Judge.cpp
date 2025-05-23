#include "Judge.h"

bool Judge::judgeWin(int row_chess, int col_chess, ChessColor color, int** chessMap)
{
	int currentColor = (color == CHESS_BLACK) ? 1 : 2;

	// 定义四个方向的增量：水平、垂直、左斜、右斜
	int directions[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };

	for (int i = 0; i < 4; i++) {
		int dx = directions[i][0];
		int dy = directions[i][1];

		int count = 1; // 当前落子点算一个

		// 正向检查
		for (int step = 1; step < 5; step++) {
			int r = row_chess + dx * step, c = col_chess + dy * step;
			if (r < 0 || r >= 15 || c < 0 || c >= 15 || chessMap[r][c] != currentColor)
				break;
			count++;
		}

		// 反向检查
		for (int step = 1; step < 5; step++) {
			int r = row_chess - dx * step, c = col_chess - dy * step;
			if (r < 0 || r >= 15 || c < 0 || c >= 15 || chessMap[r][c] != currentColor)
				break;
			count++;
		}

		// 五子连珠
		if (count >= 5) {
			return true;
		}
	}

	return false;
}