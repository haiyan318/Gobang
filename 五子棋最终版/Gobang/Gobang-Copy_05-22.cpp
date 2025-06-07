/*
* Gobang.cpp
*/
#include <easyx.h>
#include <graphics.h>
#include <conio.h>
#include <Windows.h>
#include <iostream>

enum ChessColor { CHESS_BLACK, CHESS_WHITE };  // 枚举类型命名

void init(); // 游戏初始化
void reloadBoard(); // 重新加载棋盘
void reloadModeOptions(); // 重新加载模式选项
bool judgeWin(int x, int y, ChessColor color, int** chessMap); // 判断输赢

int main()
{
	init(); // 初始化界面

	//int chessMap[753][568];
	//int* pMap = &chessMap[0][0];
	//std::fill(pMap, pMap + 753 * 568, 0);
	int **chessMap = new int *[15];
	for (int i = 0; i < 15; i++)
	{
		chessMap[i] = new int[15];
	}
	// 逐行初始化
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

			if (msg.message == WM_LBUTTONDOWN && (mode1 || mode2 || mode3)) 
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
					if (judgeWin(row_chess, col_chess, currentPlayer, chessMap))
					{
						// 显示胜利信息（例如弹窗或文字）
						MessageBox(GetHWnd(),
							(currentPlayer == CHESS_BLACK) ? _T("黑方胜利！") : _T("白方胜利！"),
							_T("游戏结束"),
							MB_OK);
						running = false; // 结束游戏循环
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

	for (int i = 0; i < 15; i++)
	{
		delete [] chessMap[i];
	}
	delete[] chessMap;
	chessMap = nullptr;
	return 0;
}

void init()
{
	// 创建窗口
	initgraph(960, 600, EX_SHOWCONSOLE);
	// 设置背景色
	setbkcolor(RGB(138, 97, 59));
	// 用背景色清空屏幕
	cleardevice();

	// 绘制棋盘
	setfillcolor(RGB(215, 169, 94)); // 棋盘背景色 RGB(215, 169, 94)
	solidroundrect(200, 15, 772, 587, 20, 20); // 绘制棋盘背景区域

	// 绘制棋盘格线
	for (int i = 220, j = 35; i <= 752; i += 38, j += 38)
	{
		setlinecolor(RGB(157, 100, 44));
		line(i, 35, i, 567);
		line(220, j, 752, j);
	}

	IMAGE img_btn_more;
	loadimage(&img_btn_more, _T("Resource/images/btn_more.png")); // 加载"菜单"图片
	putimage(50, 0, &img_btn_more);

	IMAGE img_black, img_white;
	loadimage(&img_black, _T("Resource/images/black.png"));
	putimage(50, 480, &img_black);
	loadimage(&img_white, _T("Resource/images/white.png"));
	putimage(820, 480, &img_white);

	// 显示开始界面的模式选项(双人对战, 人机对战, 机机混战, 退出游戏)
	setfillcolor(RGB(245, 245, 245));
	fillrectangle(420, 190, 550, 230);
	RECT r1 = { 420, 190, 550, 230 };
	drawtext(_T("双人对战"), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//wchar_t s_menu1[] = L"双人对战";
	//outtextxy(400, 200, s_menu1);

	fillrectangle(420, 240, 550, 280);
	RECT r2 = { 420, 240, 550, 280 };
	drawtext(_T("人机对战"), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	fillrectangle(420, 290, 550, 330);
	RECT r3 = { 420, 290, 550, 330 };
	drawtext(_T("机机混战"), &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	fillrectangle(420, 340, 550, 380);
	RECT r4 = { 420, 340, 550, 380 };
	drawtext(_T("退出游戏"), &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

// 重新加载棋盘
void reloadBoard()
{
	cleardevice();
	// 绘制棋盘
	setfillcolor(RGB(215, 169, 94)); // 棋盘背景色 RGB(215, 169, 94)
	solidroundrect(200, 15, 772, 587, 20, 20); // 绘制棋盘背景区域

	// 绘制棋盘格线
	for (int i = 220, j = 35; i <= 752; i += 38, j += 38)
	{
		setlinecolor(RGB(157, 100, 44));
		line(i, 35, i, 567);
		line(220, j, 752, j);
	}

	IMAGE img_btn_more;
	loadimage(&img_btn_more, _T("Resource/images/btn_more.png")); // 加载"菜单"图片
	putimage(50, 0, &img_btn_more);

	IMAGE img_black, img_white;
	loadimage(&img_black, _T("Resource/images/black.png"));
	putimage(50, 480, &img_black);
	loadimage(&img_white, _T("Resource/images/white.png"));
	putimage(820, 480, &img_white);
}

void reloadModeOptions()
{
	// 显示开始界面的模式选项(双人对战, 人机对战, 机机混战, 退出游戏)
	setfillcolor(RGB(245, 245, 245));
	fillrectangle(420, 190, 550, 230);
	RECT r1 = { 420, 190, 550, 230 };
	drawtext(_T("双人对战"), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//wchar_t s_menu1[] = L"双人对战";
	//outtextxy(400, 200, s_menu1);

	fillrectangle(420, 240, 550, 280);
	RECT r2 = { 420, 240, 550, 280 };
	drawtext(_T("人机对战"), &r2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	fillrectangle(420, 290, 550, 330);
	RECT r3 = { 420, 290, 550, 330 };
	drawtext(_T("机机混战"), &r3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	fillrectangle(420, 340, 550, 380);
	RECT r4 = { 420, 340, 550, 380 };
	drawtext(_T("退出游戏"), &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	FlushBatchDraw();
}

bool judgeWin(int row_chess, int col_chess, ChessColor color, int** chessMap)
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