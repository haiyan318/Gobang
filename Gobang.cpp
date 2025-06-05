
#include "UIManager.h"
#include "Judge.h"
#include <Windows.h>
#include <iostream>
enum ChessColor { CHESS_BLACK, CHESS_WHITE };
enum PiecesType { None, Black, White };
bool judgeWin(int row_chess, int col_chess, ChessColor color);
int getscoretable(int self, int e);
int getscore_h(int r, int c, int pieces);

int getscore_v(int r, int c, int pieces);
int getscore_lht(int r, int c, int pieces);
int getscore_rht(int r, int c, int pieces);
int getscore(int r, int c);


int getscore(int r, int c);

void gocalculatebest(int* row, int* col);
int chessMap[15][15];
bool isFirst = true;
void cleanMap()
{
	for (int i = 0; i < 15; i++) {
		std::fill(chessMap[i], chessMap[i] + 15, 0);
	}

}
int main()
{
	init();


	for (int i = 0; i < 15; i++) {
		std::fill(chessMap[i], chessMap[i] + 15, 0);
	}

	bool running = true;
	bool mode1 = false;
	bool mode2 = false;
	bool mode3 = false;
	bool isMenuFolded = true;
	ExMessage msg;       // 定义消息结构体
	ChessColor currentPlayer = CHESS_BLACK;        // 初始化当前玩家为黑方

	// 主循环：持续监听消息
	while (running) {
		while (peekmessage(&msg, -1))
		{
			// 左键单击后打开和折叠菜单
			if (msg.message == WM_LBUTTONDOWN && isMenuFolded && msg.x >= 50 && msg.x <= 126 && msg.y >= 0 && msg.y <= 74)
			{
				isMenuFolded = false;
				IMAGE img_btn_more_detai2;
				loadimage(&img_btn_more_detai2, _T("Resource/images/btn_more_detail2.png")); // 加载"菜单"图片
				putimage(50, 74, &img_btn_more_detai2);
				FlushBatchDraw();
			}
			else if (msg.message == WM_LBUTTONDOWN && (!isMenuFolded) && msg.x >= 50 && msg.x <= 126 && msg.y >= 0 && msg.y <= 74)
			{
				setfillcolor(RGB(138, 97, 59));
				solidrectangle(50, 74, 160, 600);
				IMAGE img_black;
				loadimage(&img_black, _T("Resource/images/black.png"));
				putimage(50, 480, &img_black);
				FlushBatchDraw();

				isMenuFolded = true;
				if (!mode1 && !mode2 && !mode3)
				{
					reloadModeOptions();
				}
			}

			// 单击菜单内按钮
			if (false == isMenuFolded)
			{
				if (msg.message == WM_LBUTTONDOWN && msg.x >= 70 && msg.x <= 145 && msg.y >= 99 && msg.y <= 184)
				{
					// 添加确认对话框，包含"确定"和"取消"按钮
					int result = MessageBox(GetHWnd(), // 窗口句柄
						_T("您确定要退出吗？"),        // 消息内容
						_T("确认退出"),                // 标题
						MB_OKCANCEL | MB_ICONQUESTION); // MB_OKCANCEL 会显示"确认"和"取消"两个按钮，MB_ICONQUESTION在弹窗中部显示一个问号图标

					if (result == IDOK) // IDOK为Windows API预定义常量，"确定"返回IDOK(对应数值1)，"取消"返回IDCANCEL(对应2)，点击右上角的关闭同"取消"一样
					{
						mode1 = false;
						mode2 = false;
						mode3 = false;
						reloadBoard();
						reloadModeOptions();
						cleanMap();
					}

				}
				else if (msg.message == WM_LBUTTONDOWN && msg.x >= 70 && msg.x <= 145 && msg.y >= 194 && msg.y <= 279)
				{
					// 添加确认对话框，包含"确定"和"取消"按钮
					int result = MessageBox(GetHWnd(),  // 窗口句柄
						_T("您确定要认输吗？"),         // 消息内容
						_T("确认认输"),                 // 标题
						MB_OKCANCEL | MB_ICONQUESTION); // MB_OKCANCEL 会显示"确认"和"取消"两个按钮，MB_ICONQUESTION在弹窗中部显示一个问号图标

					// 如果用户点击"确定"
					if (result == IDOK) // IDOK为Windows API预定义常量，"确定"返回IDOK(对应数值1)，"取消"返回IDCANCEL(对应2)，点击右上角的关闭同"取消"一样
					{
						// 显示认输与胜利信息
						MessageBox(GetHWnd(),
							(currentPlayer == CHESS_BLACK) ? _T("黑棋认输\n白方胜利！") : _T("白棋认输\n黑方胜利！"),
							_T("游戏结束"),
							MB_OK);

						// 重置棋盘和模式选项

						reloadBoard();
						reloadModeOptions();

						// 重置游戏状态
						mode1 = false;
						mode2 = false;
						mode3 = false;


						cleanMap();
						// 重置当前玩家为黑方
						currentPlayer = CHESS_BLACK;
					}
					// 如果用户点击"取消"，不做任何操作
				}
				else if (msg.message == WM_LBUTTONDOWN && msg.x >= 70 && msg.x <= 145 && msg.y >= 289 && msg.y <= 374)
				{
					// 添加确认对话框，包含"确定"和"取消"按钮
					int result = MessageBox(GetHWnd(), // 窗口句柄
						_T("您确定要求和吗？"),        // 消息内容
						_T("确认求和"),                // 标题
						MB_OKCANCEL | MB_ICONQUESTION); // MB_OKCANCEL 会显示"确认"和"取消"两个按钮，MB_ICONQUESTION在弹窗中部显示一个问号图标

					// 如果用户点击"确定"
					if (result == IDOK) // IDOK为Windows API预定义常量，"确定"返回IDOK(对应数值1)，"取消"返回IDCANCEL(对应2)，点击右上角的关闭同"取消"一样
					{
						// 显示求和信息
						MessageBox(GetHWnd(),
							(currentPlayer == CHESS_BLACK) ? _T("黑棋求和\n平局！") : _T("白棋求和\n平局！"),
							_T("游戏结束"),
							MB_OK);
						mode1 = false;
						mode2 = false;
						mode3 = false;
						cleanMap();
						reloadBoard();
						reloadModeOptions();
					}
				}
			}

			// 玩家选择游戏模式
			if (msg.message == WM_LBUTTONDOWN && false == mode1 && false == mode2 && false == mode3)
			{
				bool mode1_clicked = (msg.x >= 420 && msg.x <= 550 && msg.y >= 190 && msg.y <= 230);
				bool mode2_clicked = (msg.x >= 420 && msg.x <= 550 && msg.y >= 240 && msg.y <= 280);
				bool mode3_clicked = (msg.x >= 420 && msg.x <= 550 && msg.y >= 290 && msg.y <= 330);
				bool mode4_clicked = (msg.x >= 420 && msg.x <= 550 && msg.y >= 340 && msg.y <= 380);
				if (mode1_clicked)
				{
					mode1 = true;
					reloadBoard();
					continue;
				}
				else if (mode2_clicked)
				{
					mode2 = true;
					reloadBoard();
					continue;
				}
				else if (mode3_clicked)
				{
					mode3 = true;
					reloadBoard();
					continue;
				}
				else if (mode4_clicked)
				{
					running = false;
					continue;
				}
			}
			/*模式选择*/
			if (msg.message == WM_LBUTTONDOWN && mode1)
			{
				// 将鼠标像素坐标映射为棋盘格点坐标
				int row_chess = (msg.x - 220 + 19) / 38; // +19用于四舍五入
				int col_chess = (msg.y - 35 + 19) / 38;
				/*int gridX = 220 + round((msg.x - 220) / 38.0) * 38; // round() 可以四舍五入
				int gridY = 35 + round((msg.y - 35) / 38.0) * 38;*/

				if (currentPlayer == CHESS_BLACK)
					setfillcolor(RGB(0, 0, 0));    // 黑色
				else
					setfillcolor(RGB(255, 255, 255)); // 白色

				if (row_chess >= 0 && row_chess < 15 && col_chess >= 0 && col_chess < 15 && 0 == chessMap[row_chess][col_chess]) // 判断鼠标是否在棋盘内点击
				{
					// 绘制棋子到对应像素坐标（根据行列计算）
					int gridX = 220 + row_chess * 38;
					int gridY = 35 + col_chess * 38;
					solidcircle(gridX, gridY, 17);
					// 记录棋子颜色
					chessMap[row_chess][col_chess] = (currentPlayer == CHESS_BLACK) ? 1 : 2;
					FlushBatchDraw();

					// 判断胜负
					if (judgeWin(row_chess, col_chess, currentPlayer))
					{
						// 显示胜利信息（例如弹窗或文字）
						MessageBox(GetHWnd(),
							(currentPlayer == CHESS_BLACK) ? _T("黑方胜利！") : _T("白方胜利！"),
							_T("游戏结束"),
							MB_OK);
						mode1 = false;
						mode2 = false;
						mode3 = false;
						reloadBoard();
						reloadModeOptions();
						cleanMap();
					}

					// 切换玩家
					currentPlayer = (currentPlayer == CHESS_BLACK) ? CHESS_WHITE : CHESS_BLACK;
				}
			}
			else if (msg.message == WM_LBUTTONDOWN && mode2)
			{


				/*int gridX = 220 + round((msg.x - 220) / 38.0) * 38; // round() 可以四舍五入
				int gridY = 35 + round((msg.y - 35) / 38.0) * 38;*/

				


				if (currentPlayer == CHESS_BLACK) {

					// 将鼠标像素坐标映射为棋盘格点坐标
					int row_chess = (msg.x - 220 + 19) / 38; // +19用于四舍五入
					int col_chess = (msg.y - 35 + 19) / 38;
					setfillcolor(RGB(0, 0, 0));    // 黑色
					if (row_chess >= 0 && row_chess < 15 && col_chess >= 0 && col_chess < 15 && 0 == chessMap[row_chess][col_chess]) // 判断鼠标是否在棋盘内点击
					{
						int gridX = 220 + row_chess * 38;
						int gridY = 35 + col_chess * 38;
						solidcircle(gridX, gridY, 17);
						// 记录棋子颜色
						chessMap[row_chess][col_chess] = (currentPlayer == CHESS_BLACK) ? 1 : 2;
						FlushBatchDraw();
						// 判断胜负
						if (judgeWin(row_chess, col_chess, currentPlayer))
						{
							// 显示胜利信息（例如弹窗或文字）
							MessageBox(GetHWnd(),
								(currentPlayer == CHESS_BLACK) ? _T("黑方胜利！") : _T("白方胜利！"),
								_T("游戏结束"),
								MB_OK);
							mode1 = false;
							mode2 = false;
							mode3 = false;
							reloadBoard();
							reloadModeOptions();
							cleanMap();
							continue;
						}

						// 切换玩家
						currentPlayer = (currentPlayer == CHESS_BLACK) ? CHESS_WHITE : CHESS_BLACK;
						Sleep(100);
					}
				}
				if (currentPlayer == CHESS_WHITE) {
					setfillcolor(RGB(255, 255, 255)); // 白色
					int row, col;
					gocalculatebest(&row, &col);
					int gridX = 220 + row * 38;
					int gridY = 35 + col * 38;
					solidcircle(gridX, gridY, 17);
					chessMap[row][col] = (currentPlayer == CHESS_BLACK) ? 1 : 2;
					FlushBatchDraw();

					// 判断胜负
					if (judgeWin(row, col, currentPlayer))
					{
						// 显示胜利信息（例如弹窗或文字）
						MessageBox(GetHWnd(),
							(currentPlayer == CHESS_BLACK) ? _T("黑方胜利！") : _T("白方胜利！"),
							_T("游戏结束"),
							MB_OK);
						mode1 = false;
						mode2 = false;
						mode3 = false;
						reloadBoard();
						reloadModeOptions();
						cleanMap();
					}
					currentPlayer = (currentPlayer == CHESS_BLACK) ? CHESS_WHITE : CHESS_BLACK;
				}

			}
			else if (msg.message == WM_LBUTTONDOWN && mode3)
			{
				// 将鼠标像素坐标映射为棋盘格点坐标
				int row_chess = (msg.x - 220 + 19) / 38; // +19用于四舍五入
				int col_chess = (msg.y - 35 + 19) / 38;
				/*int gridX = 220 + round((msg.x - 220) / 38.0) * 38; // round() 可以四舍五入
				int gridY = 35 + round((msg.y - 35) / 38.0) * 38;*/

				if (currentPlayer == CHESS_BLACK)
					setfillcolor(RGB(0, 0, 0));    // 黑色
				else
					setfillcolor(RGB(255, 255, 255)); // 白色

				if (row_chess >= 0 && row_chess < 15 && col_chess >= 0 && col_chess < 15 && 0 == chessMap[row_chess][col_chess]) // 判断鼠标是否在棋盘内点击
				{
					// 绘制棋子到对应像素坐标（根据行列计算）
					int gridX = 220 + row_chess * 38;
					int gridY = 35 + col_chess * 38;
					solidcircle(gridX, gridY, 17);
					// 记录棋子颜色
					chessMap[row_chess][col_chess] = (currentPlayer == CHESS_BLACK) ? 1 : 2;
					FlushBatchDraw();

					// 判断胜负
					if (judgeWin(row_chess, col_chess, currentPlayer))
					{
						// 显示胜利信息（例如弹窗或文字）
						MessageBox(GetHWnd(),
							(currentPlayer == CHESS_BLACK) ? _T("黑方胜利！") : _T("白方胜利！"),
							_T("游戏结束"),
							MB_OK);
						mode1 = false;
						mode2 = false;
						mode3 = false;
						reloadBoard();
						reloadModeOptions();
						cleanMap();
					}

					// 切换玩家
					currentPlayer = (currentPlayer == CHESS_BLACK) ? CHESS_WHITE : CHESS_BLACK;
				}
			}
			else if (msg.message == WM_CLOSE)
			{
				running = false;
			}
		}
		Sleep(10);
	}

	EndBatchDraw();

	return 0;
}

bool judgeWin(int row_chess, int col_chess, ChessColor color)
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
		if (count == 5) {
			return true;
		}
	}

	return false;
}
int getscoretable(int self, int e)
{
	if (self > 5) return 200000;
	if (self == 5 && e == 0) return 200000;
	if (self == 5 && e == 1) return 200000;
	if (self == 5 && e == 2) return 200000;

	if (self == 4 && e == 0) return 1000;
	if (self == 4 && e == 1) return 3000;
	if (self == 4 && e == 2) return 50000;

	if (self == 3 && e == 0) return 500;
	if (self == 3 && e == 1) return 1000;
	if (self == 3 && e == 2) return 3000;

	if (self == 2 && e == 0) return 100;
	if (self == 2 && e == 1) return 200;
	if (self == 2 && e == 2) return 500;

	if (self == 1 && e == 0) return 30;
	if (self == 1 && e == 1) return 50;
	if (self == 1 && e == 2) return 100;
	return 0;
}
int getscore_h(int r, int c, int pieces)
{
	//从c向左遍历，直到遇到空白或者其他的棋子，停止统计
	int self = 1;
	int ai = 0;
	for (int i = c - 1; i >= 0; i--)
	{
		if (chessMap[r][i] == pieces)
		{
			self++;
		}
		else if (chessMap[r][i] == None)
		{
			break;
		}
		else {
			ai++;
			break;
		}
	}

	for (int i = c + 1; i < 15; i++)
	{
		if (chessMap[r][i] == pieces)
		{
			self++;
		}
		else if (chessMap[r][i] == None)
		{
			break;
		}
		else {
			ai++;
			break;
		}
	}
	return getscoretable(self, ai);
}
int getscore_v(int r, int c, int pieces)
{
	int self = 1;
	int ai = 0;

	for (int i = r - 1; i >= 0; i--)
	{
		if (chessMap[i][c] == pieces)
		{
			self++;
		}
		else if (chessMap[i][c] == None)
		{
			break;
		}
		else {
			ai++;
			break;
		}
	}

	for (int i = r + 1; i < 15; i--)
	{
		if (chessMap[i][c] == pieces)
		{
			self++;
		}
		else if (chessMap[i][c] == None)
		{
			break;
		}
		else {
			ai++;
			break;
		}
	}
	return getscoretable(self, ai);
}
int getscore_lht(int r, int c, int pieces)
{
	int self = 1;
	int ai = 0;
	for (int i = r + 1, k = c - 1; i < 15 && k >= 0; i++, k--)
	{
		if (chessMap[i][k] == pieces)
		{
			self++;
		}
		else if (chessMap[i][k] == None)
		{
			break;
		}
		else {
			ai++;
			break;
		}

	}

	for (int i = r - 1, k = c + 1; i >= 0 && k < 15; i--, k++)
	{
		if (chessMap[i][k] == pieces)
		{
			self++;
		}
		else if (chessMap[i][k] == None)
		{
			break;
		}
		else {
			ai++;
			break;
		}

	}
	return getscoretable(self, ai);
}
int getscore_rht(int r, int c, int pieces)
{
	int self = 1;
	int ai = 0;
	for (int i = r + 1, k = c + 1; i < 15 && k < 15; i++, k++)
	{
		if (chessMap[i][k] == pieces)
		{
			self++;
		}
		else if (chessMap[i][k] == None)
		{
			break;
		}
		else {
			ai++;
			break;
		}

	}

	for (int i = r - 1, k = c - 1; i >= 0 && k >= 0; i--, k--)
	{
		if (chessMap[i][k] == pieces)
		{
			self++;
		}
		else if (chessMap[i][k] == None)
		{
			break;
		}
		else {
			ai++;
			break;
		}

	}
	return getscoretable(self, ai);
}
int getscore(int r, int c)
{
	int numh1 = getscore_h(r, c, Black);
	int numh2 = getscore_h(r, c, White);
	int numv1 = getscore_v(r, c, Black);
	int numv2 = getscore_v(r, c, White);
	int numlh1 = getscore_lht(r, c, Black);
	int numlh2 = getscore_lht(r, c, White);
	int numrh1 = getscore_rht(r, c, Black);
	int numrh2 = getscore_rht(r, c, White);

	if (numh1 >= 200000 || numh2 >= 200000 || numv1 >= 200000 || numv2 >= 200000 ||
		numlh1 >= 200000 || numlh2 >= 200000 || numrh1 >= 200000 || numrh2 >= 200000)
	{
		return INT_MAX;
	}

	int xscore = numh1 + numh2;
	int yscore = numv1 + numv2;
	int lhscore = numlh1 + numlh2;
	int rhscore = numrh1 + numrh2;

	return xscore + yscore + lhscore + rhscore;
}
void gocalculatebest(int* row, int* col)
{
	int max = 0;
	for (int i = 0; i < 15; i++)
	{
		for (int k = 0; k < 15; k++)
		{
			if (chessMap[i][k] != None)
				continue;
			int score = getscore(i, k);
			if (score >= 200000)
			{
				*row = i;
				*col = k;
				return;
			}
			if (score > max)
			{
				max = score;
				*row = i;
				*col = k;
			}

		}
	}

	if (*row == 0 && *col == 0 && isFirst == true)
	{
		*row = 15 / 2;
		*col = 15 / 2;
		isFirst = false;
	}

}