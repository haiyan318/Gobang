#include "UIManager.h"



void init()
{

	initgraph(960, 600, EX_SHOWCONSOLE);

	setbkcolor(RGB(138, 97, 59));

	cleardevice();


	setfillcolor(RGB(215, 169, 94)); 
	solidroundrect(200, 15, 772, 587, 20, 20); 


	for (int i = 220, j = 35; i <= 752; i += 38, j += 38)
	{
		setlinecolor(RGB(157, 100, 44));
		line(i, 35, i, 567);
		line(220, j, 752, j);
	}

	IMAGE img_btn_more;
	loadimage(&img_btn_more, _T("Resource/images/btn_more.png")); 
	putimage(50, 0, &img_btn_more);

	IMAGE img_black, img_white;
	loadimage(&img_black, _T("Resource/images/black.png"));
	putimage(50, 480, &img_black);
	loadimage(&img_white, _T("Resource/images/white.png"));
	putimage(820, 480, &img_white);


	setfillcolor(RGB(245, 245, 245));
	fillrectangle(420, 190, 550, 230);
	RECT r1 = { 420, 190, 550, 230 };
	drawtext(_T("双人对战"), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


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

void reloadBoard()
{
	cleardevice();

	setfillcolor(RGB(215, 169, 94)); 
	solidroundrect(200, 15, 772, 587, 20, 20); 


	for (int i = 220, j = 35; i <= 752; i += 38, j += 38)
	{
		setlinecolor(RGB(157, 100, 44));
		line(i, 35, i, 567);
		line(220, j, 752, j);
	}

	IMAGE img_btn_more;
	loadimage(&img_btn_more, _T("Resource/images/btn_more.png")); 
	putimage(50, 0, &img_btn_more);

	IMAGE img_black, img_white;
	loadimage(&img_black, _T("Resource/images/black.png"));
	putimage(50, 480, &img_black);
	loadimage(&img_white, _T("Resource/images/white.png"));
	putimage(820, 480, &img_white);
}

void reloadModeOptions()
{
	setfillcolor(RGB(245, 245, 245));
	fillrectangle(420, 190, 550, 230);
	RECT r1 = { 420, 190, 550, 230 };
	drawtext(_T("双人对战"), &r1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	

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