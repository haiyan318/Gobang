/*
* Gobang.cpp
*/
#include <easyx.h>
#include <graphics.h>
#include <conio.h>
#include <iostream>



int main()
{
	// 创建窗口
	initgraph(960, 600, EX_SHOWCONSOLE);
	// 设置背景色
	setbkcolor(RGB(138, 97, 59));
	// 用背景色清空屏幕
	cleardevice();

	/*IMAGE img;
	loadimage(&img, _T("Resource/images/bk.jpg"), 960, 600);
	putimage(0, 0, &img);*/

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

	// 显示开始界面的文字菜单选项
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

	setfillcolor(BLACK); // 棋子绘制示例
	solidcircle(220, 35, 17);
	solidcircle(258, 35, 17);
	solidcircle(258, 73, 17);

	ExMessage msg;       // 定义消息结构体
	enum ChessColor { CHESS_BLACK, CHESS_WHITE };  // 枚举类型命名
	ChessColor currentPlayer = CHESS_BLACK;        // 初始化当前玩家为黑方

	bool chessMap[753][568];
	bool* pMap = &chessMap[0][0];
	std::fill(pMap, pMap + 753 * 568, 0);

	bool running = true;

	// 主循环：持续监听消息
	while (running) {
		while (peekmessage(&msg, -1)) {
			if (msg.message == WM_LBUTTONDOWN) 
			{
				// 转换鼠标坐标为格点坐标
				int gridX = 220 + static_cast<int>((msg.x - 220) / 38.0 + 0.5) * 38;
				int gridY = 35 + static_cast<int>((msg.y - 35) / 38.0 + 0.5) * 38;
				/*int gridX = 220 + round((msg.x - 220) / 38.0) * 38; // round() 可以四舍五入
				int gridY = 35 + round((msg.y - 35) / 38.0) * 38;*/

				if (currentPlayer == CHESS_BLACK) 
					setfillcolor(RGB(0, 0, 0));    // 黑色
				else 
					setfillcolor(RGB(255, 255, 255)); // 白色

				if (gridX >= 220 && gridX <= 752 && gridY >= 35 && gridY <= 567 && 0 == chessMap[gridX][gridY]) // 判断鼠标是否在棋盘内点击
				{
					solidcircle(gridX, gridY, 17);
					chessMap[gridX][gridY] = 1;
					FlushBatchDraw();

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

	closegraph();
	return 0;
}
