#include <easyx.h>
#include <graphics.h>
#include<time.h>
#include "book.h"


const int WIDTH = 42;//7的倍数
const int HEIGHT = 24;

bool taller;

IMAGE buffer(WIDTH * 30 + 160, HEIGHT * 20);

booktree book;
int bookNum = 0;//图书数量
int uord = 0;//还没用上
int uordsum = 0;//点击总分的标志位，还没用上


button btn[8] = {
	{0,0,L"图书管理系统"},
	{0, 40, L"增加图书信息"},
	{0, 80, L"删除图书信息"},
	{0, 120, L"修改图书信息"},
	{0, 160,L"查询图书信息"},
	{0,200,L"输出图书信息"},
	{0, 240, L"查看树的结构"},
	{0,280,L"  退出程序"}
};

int btnNum = 8;//按键数量

button btnUp = { 200,435,L"  上一页" };
button btnDown = { 650,435,L"  下一页" };

node bookAdd = { 0,L"NULL",L"NULL" ,L"NULL",L"NULL",0 ,NULL };
node bookSearch= { 0,L"NULL",L"NULL" ,L"NULL",L"NULL",0 };

clock_t start, stop;
double times;


node* fixBook = NULL;
int btnShowNum = 0;

//函数声明
//清空StuAdd
void setUpBook();
//绘制时间
void drawtime();

//////////////
//绘制视图函数//
/////////////
void drawView(int status);
//绘制左侧按钮
void drawLeft();
//绘制按钮
void drawButton(button BTN, int width, int height);

//设置按钮状态
void setUpButton(int index, int status);

//绘制作品相关信息
void drawOpusInfo();

//输出所有图书信息
void printBookInfo(node* L, int sbookNum, int num);

//增加图书信息界面
void printAddBookInfo(node* L);

//删除图书信息界面
void printDeleteBookInfo(node* L);

//查询图书信息界面
void printSearchBookInfo(node* L);

//修改图书信息界面
void printFixBookInfo(node* L);

//输出单个图书信息界面
void drawOneBookInfo();

///////////////
// 按钮触发函数//
//////////////

// 增加图书信息页面按钮触发
void addInfoBtn(MOUSEMSG m);
//导航栏按钮触发
void menBtn(MOUSEMSG m, int* status);
//输出图书信息页面按钮触发
void showBookInfoBtn(MOUSEMSG m);
//删除图书信息页面按钮触发
void deleteBookInfoBtn(MOUSEMSG m);
//修改图书信息页面按钮触发
void fixBookInfoBtn(MOUSEMSG m, int* status);
//查询图书信息页面按钮触发
void searchBookInfoBtn(MOUSEMSG m);
void searchBookInfoBtn(MOUSEMSG m, double time);
//查看树的形状
void showBookTree(MOUSEMSG m);
//创建平衡二叉树
int InsertAVL(booktree* T, node* s, node book, bool* taller);



int main() {
	initgraph(WIDTH * 20 + 160, HEIGHT * 20);

	MOUSEMSG m;
	int status = 0;
	int fixStatus = 0;
	bookNum = load2(&book,&taller);//初始化图书信息
	draw(book);
	FlushMouseMsgBuffer();
	while (true) {

		while (MouseHit()) {
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN) {//左键按下
				if (m.x >= 0 && m.x <= 160) {
					menBtn(m, &status);
				}
				else if (m.x > 160 && m.x < WIDTH * 20 + 160) {
					switch (status) {
					case 1://增加信息页面按键触发
						addInfoBtn(m);
						break;
					case 2://删除图书信息页面按键触发
						deleteBookInfoBtn(m);
						break;
					case 3://修改图书信息页面按键触发
						fixBookInfoBtn(m, &fixStatus);
						break;
					case 4://查询图书信息页面按键触发
						searchBookInfoBtn(m);
						//printf("\n%f\n", times);
						break;
					case 5://输出成绩页面按键
						showBookInfoBtn(m);
						break;
					case 6://展示树的结构
						showBookTree(m);
						break;
					}
				}
			}
			else if (m.uMsg == WM_LBUTTONUP) {//左键弹起
				//重置按钮状态
				setUpButton(0, -1);
			}
		}
		drawView(status);
	}
	closegraph();
	return 0;
}

//绘制信息
void drawView(int status) {
	SetWorkingImage(&buffer);
	// 清屏
	cleardevice();
	// 设置背景混合模式 背景为透明
	setbkmode(TRANSPARENT);
	setbkcolor(0xf0f0f0);

	//绘制左侧功能按钮
	drawLeft();

	//绘制右侧信息
	switch (status) {
	case 0:
		drawOpusInfo();//绘制作品信息
		break;
	case 1://增加
		printAddBookInfo(book);
		break;
	case 2://删除
		printDeleteBookInfo(book);
		break;
	case 3://修改
		printFixBookInfo(book);
		break;
	case 4://查询图书成绩
		printSearchBookInfo(book);
		break;
	case 5://输出图书信息
		printBookInfo(book, bookNum, btnShowNum);
		break;
	case 6://画树
		refresh(book, 550, 40, 550, 40, 1);
		break;
	}

	// 设置当前绘图设备
	SetWorkingImage();
	// 绘制图像到屏幕
	putimage(0, 0, &buffer);
}


//绘制左侧格式
void drawLeft() {
	//设置底层背景
	setfillcolor(RGB(137, 101, 88));
	fillrectangle(0, 0, 160, HEIGHT * 20);
	//绘制按钮
	for (int i = 0; i < btnNum; i++) {
		drawButton(btn[i], 160, 30);
	}
}


//绘制按钮
void drawButton(button BTN, int width = 160, int height = 30) {
	setlinecolor(WHITE);
	setfillcolor(BTN.color[BTN.status]);
	int x = BTN.x, y = BTN.y;
	fillroundrect(x, y, x + width, y + height, 10, 10);
	settextcolor(BLACK);
	settextstyle(20, 0, _T("Consolas"));
	outtextxy(x + 28, y + 7, BTN.str);
}


//设置按钮状态
void setUpButton(int index, int status) {
	if (status != -1) {
		for (int i = 0; i < btnNum; i++) {
			btn[i].status = (status + 1) % 2;
		}
		btn[index].status = status;
	}
	else {
		for (int i = 0; i < btnNum; i++) {
			btn[i].status = 0;
		}
	}
}

void drawOpusInfo(){
	LOGFONT f;
	gettextstyle(&f);						// 获取当前字体设置
	f.lfHeight = 64;
	f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿  
	_tcscpy_s(f.lfFaceName, _T("华文行楷"));		// 设置字体为“黑体”
	settextstyle(&f);						// 设置字体样式
	outtextxy(300, 50, _T("图书信息管理系统"));

	settextstyle(25, 0, _T("华文行楷"));
	outtextxy(425, 135, _T("欢迎使用图书管理系统"));


	settextstyle(18, 0, _T("仿宋"));
	outtextxy(300, 180, _T("本系统设有7个功能模块，如左侧功能区所示，以下是使用说明："));
	outtextxy(300, 200, _T("①、图书管理系统模块：在本模块介绍了该系统主要功能以及操作方式。"));
	outtextxy(300, 220, _T("②、增加图书信息：分别点击添加按钮，可编辑其数值，点击保存可记录下本条信息。"));
	outtextxy(300, 240, _T("③、删除图书信息：使用此模块，通过图书编号来查询图书信息记录，点击删除按钮"));
	outtextxy(300, 260, _T("    后，删除该生信息记录。"));
	outtextxy(300, 280, _T("④、修改图书信息：通过编号查询书本信息，若某信息有误，可通过修改按钮修改。"));
	outtextxy(300, 300, _T("⑤、查询图书信息：通过编号来查询，展示图书的所有信息。"));
	outtextxy(300, 320, _T("⑥、输出图书信息：实时输出系统中录入的所有图书信息。"));
	outtextxy(300, 340, _T("⑦、退出程序：点击后退出程序。"));
}

void printBookInfo(node* L, int BookNum, int num) {
	setfillcolor(RGB(204, 213, 240));
	fillrectangle(160, 0, WIDTH * 20 + 160, 75);

	settextstyle(50, 0, _T("华文行楷"));
	outtextxy(380, 15, _T("图书信息表"));

	//输出表格
	settextstyle(20, 0, _T("楷体"));
	LOGFONT f;
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	wchar_t str[1000];
	_stprintf_s(str, L"当前记录在库图书总数为%d 当前第%d/%d页", bookNum, num + 1, (bookNum - 1) / 15 + 1);
	outtextxy(165, 77, str);

	int numx = bookNum - 15 * num;//剩余显示人数

	setlinecolor(BLACK);
	int div = WIDTH / 8;
	line(160, 75, WIDTH * 20 + 160, 75);

	int starx = 160, stary = 100;
	//画横线
	for (int i = 0; i < numx + 2 && i < 17; i++) {
		line(starx, stary + i * 20, WIDTH * 20 + 160, stary + i * 20);
		if (i == 0) {//画出文字
			//图书编号
			outtextxy(starx + 5, stary + 1, L" 图书编号");
			//书名
			outtextxy(starx + 1 * div * 20 + 5, stary + 1, L"  书名");
			//作者
			outtextxy(starx + 3 * div * 20 + 5, stary + 1, L"  作者");
			//出版社
			outtextxy(starx + 4 * div * 20 + 5, stary + 1, L" 出版社");
			//价格
			outtextxy(starx + 6 * div * 20 + 5, stary + 1, L"  价格");
			//购买时间
			outtextxy(starx + 7 * div * 20 + 5, stary + 1, L"购买时间");
		}
	}

	//画六道竖线
	if (numx <= 15) {
		//line(starx + i * div * 20, stary, starx + i * div * 20, stary + (numx + 1) * 20);
		line(starx + 0 * div * 20, stary, starx + 0 * div * 20, stary + (numx + 1) * 20);
		line(starx + 1 * div * 20, stary, starx + 1 * div * 20, stary + (numx + 1) * 20);
		line(starx + 3 * div * 20, stary, starx + 3 * div * 20, stary + (numx + 1) * 20);
		line(starx + 4 * div * 20, stary, starx + 4 * div * 20, stary + (numx + 1) * 20);
		line(starx + 6 * div * 20, stary, starx + 6 * div * 20, stary + (numx + 1) * 20);
		line(starx + 7 * div * 20, stary, starx + 7 * div * 20, stary + (numx + 1) * 20);
	}
	else {
		line(starx + 0 * div * 20, stary, starx + 0 * div * 20, stary + 16 * 20);
		line(starx + 1 * div * 20, stary, starx + 1 * div * 20, stary + 16 * 20);
		line(starx + 3 * div * 20, stary, starx + 3 * div * 20, stary + 16 * 20);
		line(starx + 4 * div * 20, stary, starx + 4 * div * 20, stary + 16 * 20);
		line(starx + 6 * div * 20, stary, starx + 6 * div * 20, stary + 16 * 20);
		line(starx + 7 * div * 20, stary, starx + 7 * div * 20, stary + 16 * 20);
	}

	//输出图书信息
	stary = stary + 20;
	booknode* pp[100];
	int *bn = 0;
	InorderTra(L, pp, bn);
	int cnt = 0;
	int i, len = 0;
	i = 15 * num;
	wchar_t text[500] = { 0 };
	while (i < bookNum && cnt < 15) {
		//图书编号
		_stprintf_s(text, L"%d", pp[i]->id);
		outtextxy(starx + 5, stary + cnt * 20 + 1, text);
		//图书名字
		outtextxy(starx + 1 * div * 20 + 5, stary + cnt * 20 + 1, pp[i]->name);
		//图书作者
		outtextxy(starx + 3 * div * 20 + 5, stary + cnt * 20 + 1, pp[i]->author);
		//图书出版社
		outtextxy(starx + 4 * div * 20 + 5, stary + cnt * 20 + 1, pp[i]->publisher);
		//图书价格
		_stprintf_s(text, L"%d", pp[i]->price);
		outtextxy(starx + 6 * div * 20 + 5, stary + cnt * 20 + 1, text);
		//购买时间
		outtextxy(starx + 7 * div * 20 + 5, stary + cnt * 20 + 1, pp[i]->time);
		cnt++;
		i++;
	}
	drawButton(btnUp);
	drawButton(btnDown);
}


//增加图书信息界面
void printAddBookInfo(node* L) {
	setfillcolor(RGB(204, 213, 240));
	fillrectangle(160, 0, WIDTH * 20 + 160, 75);

	settextstyle(50, 0, _T("华文行楷"));
	outtextxy(380, 15, _T("      增加图书信息"));
	settextstyle(20, 0, _T("楷体"));
	wchar_t str[100];
	_stprintf_s(str, L"当前在库图书总数为%d ", bookNum);
	outtextxy(165, 77, str);

	//绘制外框
	int starx = 300, stary = 150;
	drawOneBookInfo();
	//声明按钮
	button btnAdd[10] = {
		{ starx + 250,stary + 11,L"添加" },
		{ starx + 250,stary + 51,L"添加" },
		{ starx + 250,stary + 91,L"添加" },
		{ starx + 250,stary + 131,L"添加" },
		{ starx + 250,stary + 171,L"添加" },
		{ starx + 250,stary + 211,L"添加" },
		{ starx + 250,stary + 291,L"保存" }
	};
	//绘制按钮
	for (int i = 0; i < 7; i++) {
		drawButton(btnAdd[i], 90, 30);
	}
}

//增加图书信息界面按钮触发
void addInfoBtn(MOUSEMSG m) {
	if (m.x >= 550 && m.x <= 640) {
		if (m.y >= 161 && m.y <= 191) {//增加学号
			wchar_t s[100] = { 0 };
			InputBox(s, 100, L"请输入编号");
			int id = _wtoi(s);
			bookAdd.id = id;
		}
		else if (m.y >= 201 && m.y <= 231) {//增加书名
			wchar_t s[100] = { 0 };
			InputBox(s, 100, L"请输入书名");
			wcscpy_s(bookAdd.name, s);
		}
		else if (m.y >= 241 && m.y <= 271) {//增加作者
			wchar_t s[100] = { 0 };
			InputBox(s, 100, L"请输入作者");
			wcscpy_s(bookAdd.author, s);
		}
		else if (m.y >= 281 && m.y <= 311) {//增加出版社
			wchar_t s[100] = { 0 };
			InputBox(s, 100, L"请输入出版社");
			wcscpy_s(bookAdd.publisher, s);
		}
		else if (m.y >= 321 && m.y <= 361) {//增加购买时间
			wchar_t s[100] = { 0 };
			InputBox(s, 100, L"请输入购买时间");
			wcscpy_s(bookAdd.time, s);
		}
		else if (m.y >= 371 && m.y <= 401) {//增加价格
			wchar_t s[100] = { 0 };
			InputBox(s, 100, L"请输入价格");
			int price = _wtoi(s);
			bookAdd.price = price;
		}
		else if (m.y >= 451 && m.y <= 481) {//保存(m.y >= 401 && m.y <= 431)备份
			wchar_t text2[50] = L"保存情况";

			if (addBookInfo2(&book, bookAdd, &bookNum,&taller) == 1) {
				wchar_t text1[50] = L"保存成功！";
				MessageBox(GetHWnd(), text1, text2, 0);
				bookNum++;
			}
			else {
				wchar_t text1[50] = L"保存失败！";
				MessageBox(GetHWnd(), text1, text2, 0);
			}
			//清空bookAdd
			setUpBook();
		}
	}

}

//导航栏按钮触发
void menBtn(MOUSEMSG m, int* status) {
	if (m.y >= 0 && m.y <= 30) {//移动至展示作品信息按钮
		setUpButton(0, 1);
		*status = 0;
		btnShowNum = 0;
		//drawOpusInfo();//绘制作品信息

	}
	else if (m.y >= 40 && m.y <= 70) {//移动至增加图书信息按钮
		setUpButton(1, 1);
		*status = 1;
		btnShowNum = 0;
	}
	else if (m.y >= 80 && m.y <= 110) {//移动至删除图书信息按钮
		setUpButton(2, 1);
		btnShowNum = 0;
		*status = 2;

	}
	else if (m.y >= 120 && m.y <= 150) {//移动至修改图书信息按钮
		setUpButton(3, 1);
		btnShowNum = 0;
		*status = 3;
	}
	else if (m.y >= 160 && m.y <= 190) {//移动至查询图书信息按钮
		setUpButton(4, 1);
		btnShowNum = 0;
		*status = 4;

	}
	else if (m.y >= 200 && m.y <= 230) {//移动至输出图书信息按钮
		setUpButton(5, 1);
		btnShowNum = 0;
		*status = 5;
	}
	else if (m.y >= 240 && m.y <= 270) {//移动至退出程序按钮
		setUpButton(6, 1);
		btnShowNum = 0;
		*status = 6;
	}
	else if (m.y >= 280 && m.y <= 310) {
		setUpButton(7, 1);
		btnShowNum = 0;
		closegraph();

		exit(0);
	}
}


//删除图书信息界面
void printDeleteBookInfo(node* L) {
	setfillcolor(RGB(204, 213, 240));
	fillrectangle(160, 0, WIDTH * 20 + 160, 75);

	settextstyle(50, 0, _T("华文行楷"));
	outtextxy(380, 15, _T("图书信息删除"));
	settextstyle(20, 0, _T("楷体"));
	wchar_t str[100];
	_stprintf_s(str, L"当前记录图书总数为%d ", bookNum);
	outtextxy(165, 77, str);

	//绘制外框
	int starx = 300, stary = 150;
	setfillcolor(0xd5dce8);
	fillrectangle(starx, stary, starx + 400, stary + 290);

	//声明按钮
	button btnAdd[8] = {
		{700,110,L"删除"},
		{ 280,110,L"以图书编号查询" }
	};
	//绘制按钮
	drawButton(btnAdd[0], 80, 30);
	for (int i = 1; i < 2; i++) {
		drawButton(btnAdd[i], 180, 30);
	}
	drawOneBookInfo();
}

//查询图书信息界面
void printSearchBookInfo(node* L) {
	setfillcolor(RGB(204, 213, 240));
	fillrectangle(160, 0, WIDTH * 20 + 160, 75);

	settextstyle(50, 0, _T("华文行楷"));
	outtextxy(380, 15, _T("图书信息查询"));
	settextstyle(20, 0, _T("楷体"));
	wchar_t str[100];
	_stprintf_s(str, L"记录在库图书总数为%d ", bookNum);
	outtextxy(165, 77, str);



	//声明按钮
	button btnAdd[10] = {
		{ 280,110,L"以图书编号查询" },
		{ 500,110,L"以数组方式查询" }
	};
	//绘制按钮
	int btnNums = 2;
	for (int i = 0; i < btnNums; i++) {
		drawButton(btnAdd[i], 180, 30);
	}
	drawOneBookInfo();
	drawtime();
}

//修改图书信息界面
void printFixBookInfo(node* L) {
	setfillcolor(RGB(204, 213, 240));
	fillrectangle(160, 0, WIDTH * 20 + 160, 75);

	settextstyle(50, 0, _T("华文行楷"));
	outtextxy(380, 15, _T("图书信息修改"));
	settextstyle(20, 0, _T("楷体"));
	wchar_t str[100];
	_stprintf_s(str, L"记录库图书总数为%d ", bookNum);
	outtextxy(165, 77, str);

	//绘制外框
	int starx = 300, stary = 150;
	drawOneBookInfo();

	//声明按钮
	button btnAdd[10] = {
		{ starx + 200,stary - 40,L" 查询图书编号" },
		{ starx + 250,stary + 11,L"修改" },
		{ starx + 250,stary + 51,L"修改" },
		{ starx + 250,stary + 91,L"修改" },
		{ starx + 250,stary + 131,L"修改" },
		{ starx + 250,stary + 171,L"修改" },
		{ starx + 250,stary + 211,L"修改" },
		{ starx + 250,stary + 291,L"保存" }
	};
	//绘制按钮
	int btnNums = 8;
	drawButton(btnAdd[0], 180, 30);
	for (int i = 1; i < btnNums; i++) {
		drawButton(btnAdd[i], 90, 30);
	}

}

void showBookInfoBtn(MOUSEMSG m) {
	if (m.x >= 200 && m.x <= 360 && m.y >= 435 && m.y <= 465) {//上一页
		if (btnShowNum > 0)
			btnShowNum--;
	}

	if (m.x >= btnDown.x && m.x <= btnDown.x + 160 && m.y >= 435 && m.y <= 465) {//下一页
		if (btnShowNum < (bookNum - 1) / 15)
			btnShowNum++;
	}
	//---------------------------------------------------------------------点击触发成绩排序
	int starx = 160, stary = 100;
	int div = WIDTH / 8;
	if (m.x > (starx + 6 * div * 20) && m.x< (starx + 7 * div * 20) && m.y>(stary + 1) && m.y < (stary + 21))
	{
		if (uord == 1)
		{
			uord = 0;
		}
		else if (uord == 0)
		{
			uord = 1;
		}
		uordsum = 1;
	}
	//------------------------------------------------------------------------
	//**************************************************************************点击触发学号排序
	if (m.x > (starx + 0 * div * 20) && m.x< (starx + 1 * div * 20) && m.y>(stary + 1) && m.y < (stary + 21))
	{
		if (uord == 1)
		{
			uord = 0;
		}
		else if (uord == 0)
		{
			uord = 1;
		}
		uordsum = 0;
	}
	//******************************************************************************
}


//删除图书信息页面按钮触发
void deleteBookInfoBtn(MOUSEMSG m) {
	node* st = new node;
	st = NULL;
	if (m.x >= 280 && m.x <= 460 && m.y >= 110 && m.y <= 140) {
		//按图书编号查询按钮
		wchar_t s[100] = { 0 };
		InputBox(s, 100, L"请输入查询的编号");
		int id = _wtoi(s);
		//获取对应图示的位置
		booknode *st1;
		st = searchbook(id, book,&st1);
		//
		if (st == NULL) {//查询此人失败
			wchar_t text1[50] = { L"无信息记录" }, text2[50] = { L"查询结果" };
			MessageBox(GetHWnd(), text1, text2, 0);
		}
		else {
			bookAdd = *st;
		}
	}

	if (&bookAdd != NULL && m.x >= 700 && m.x <= 790 && m.y >= 110 && m.y <= 140) {
		//删除按钮
		DeleteBST(&book,&(bookAdd));
		setUpBook();
		save(&book);
		bookNum--;
		wchar_t text1[50] = { L"删除成功！" }, text2[50] = { L"删除结果" };
		MessageBox(GetHWnd(), text1, text2, 0);
	}
}

void showBookTree(MOUSEMSG m) {

}

//查询图书信息页面按钮触发
void searchBookInfoBtn(MOUSEMSG m) {
	node* st = NULL;
	if (m.x >= 280 && m.x <= 460 && m.y >= 110 && m.y <= 140) {
		//按学号查询按钮
		wchar_t s[100] = { 0 };
		InputBox(s, 100, L"请输入查询的编号");
		int id = _wtoi(s);
		//获取对应图书的位置
		booktree *st1 = NULL;
		LARGE_INTEGER start_time, end_time;
		double dqFreq;
		LARGE_INTEGER f;
		QueryPerformanceFrequency(&f);
		dqFreq = (double)f.QuadPart;
		QueryPerformanceCounter(&start_time);
		st = searchbookplus(id, book,st1);
		QueryPerformanceCounter(&end_time);
		times = 1000000 * (end_time.QuadPart - start_time.QuadPart) / dqFreq;
		//
		if (st == NULL) {//查询失败
			wchar_t text1[50] = { L"无信息记录" }, text2[50] = { L"查询结果" };
			MessageBox(GetHWnd(), text1, text2, 0);
		}
		else {
			bookAdd = *st;
		}
	}
	else if (m.x >= 500 && m.x <= 680 && m.y >= 110 && m.y <= 140) {
		//按数组查询
		wchar_t s[100] = { 0 };
		InputBox(s, 100, L"用图书编号查询数组");
		int id = _wtoi(s);
		booknode* pp[100];
		int* bn = 0;
		InorderTra(book, pp, bn);
		int i = 0;
		LARGE_INTEGER start_time, end_time;
		double dqFreq;
		LARGE_INTEGER f;
		QueryPerformanceFrequency(&f);
		dqFreq = (double)f.QuadPart;
		QueryPerformanceCounter(&start_time);
		while (i < bookNum) {
			if (pp[i]->id == id) {
				st = pp[i];
				QueryPerformanceCounter(&end_time);
				times = 1000000 * (end_time.QuadPart - start_time.QuadPart) / dqFreq;
				break;
			}
			i++;
		}
		if (st == NULL) {//查询失败
			wchar_t text1[50] = { L"无信息记录" }, text2[50] = { L"查询结果" };
			MessageBox(GetHWnd(), text1, text2, 0);
		}
		else {
			bookAdd = *st;
		}
	}
}

//修改图书信息界面按钮触发
void fixBookInfoBtn(MOUSEMSG m, int* status) {
	if (m.x >= 500 && m.x <= 680 && m.y >= 110 && m.y <= 140) {//按学号查询
		wchar_t s[100] = { 0 };
		InputBox(s, 100, L"请输入要查询的图书编号");
		int id = _wtoi(s);
		booktree* st1=NULL;
		fixBook = searchbook(id, book,st1);
		if (fixBook != NULL) {
			bookAdd = *fixBook;
			*status = 1;
		}
	}

	if (*status == 1) {
		if (m.x >= 550 && m.x <= 640) {
			if (m.y >= 161 && m.y <= 191) {//修改学号
				wchar_t s[100] = { 0 };
				InputBox(s, 100, L"请输入新编号");
				int id = _wtoi(s);
				bookAdd.id = id;
			}
			else if (m.y >= 201 && m.y <= 231) {//修改书名
				wchar_t s[100] = { 0 };
				InputBox(s, 100, L"请输入新书名");
				wcscpy_s(bookAdd.name, s);
			}
			else if (m.y >= 241 && m.y <= 271) {//修改作者
				wchar_t s[100] = { 0 };
				InputBox(s, 100, L"请输入新作者");
				wcscpy_s(bookAdd.author, s);
			}
			else if (m.y >= 281 && m.y <= 311) {//修改出版社
				wchar_t s[100] = { 0 };
				InputBox(s, 100, L"请输入新出版社");//
				wcscpy_s(bookAdd.publisher, s);
			}
			else if (m.y >= 321 && m.y <= 351) {//修改购买时间
				wchar_t s[100] = { 0 };
				InputBox(s, 100, L"请输入新购买时间");
				wcscpy_s(bookAdd.time, s);
			}
			else if (m.y >= 361 && m.y <= 391) {//修改价格
				wchar_t s[100] = { 0 };
				InputBox(s, 100, L"请输入新价格");
				int price = _wtoi(s);
				bookAdd.price = price;
			}
			else if (m.y >= 441 && m.y <= 471) {
				if (fixBook != NULL)
					*fixBook = bookAdd;
				wchar_t text2[50] = L"保存情况";

				if (save(&book) == 1 && fixBook != NULL) {
					wchar_t text1[50] = L"保存成功！";
					MessageBox(GetHWnd(), text1, text2, 0);
				}
				else {
					wchar_t text1[50] = L"保存失败！";
					MessageBox(GetHWnd(), text1, text2, 0);
				}
				//清空stAdd
				bookAdd.id = 0;
				wcscpy_s(bookAdd.name, L"NULL");
				wcscpy_s(bookAdd.author, L"NULL");
				wcscpy_s(bookAdd.publisher, L"NULL");
				wcscpy_s(bookAdd.time, L"NULL");
				bookAdd.price = 0;

				*status = 0;
			}
		}
	}
}

//绘制单独的图书信息
void drawOneBookInfo() {
	settextstyle(20, 0, _T("Consolas"));
	//绘制外框
	int starx = 300, stary = 150;
	setfillcolor(RGB(204, 213, 240));
	fillrectangle(starx, stary, starx + 400, stary + 330);
	//绘制提示
	setfillcolor(0xf0f0f0);
	wchar_t text[100];
	//增加编号信息
	outtextxy(starx + 5, stary + 15, L"编号");
	fillroundrect(starx + 60, stary + 11, starx + 220, stary + 41, 10, 10);
	_stprintf_s(text, L"%d", bookAdd.id);
	outtextxy(starx + 65, stary + 15, text);

	//增加书名信息
	outtextxy(starx + 5, stary + 55, L"书名");
	fillroundrect(starx + 60, stary + 51, starx + 220, stary + 81, 10, 10);
	outtextxy(starx + 65, stary + 55, bookAdd.name);

	//增加作者信息
	outtextxy(starx + 5, stary + 95, L"作者");
	fillroundrect(starx + 60, stary + 91, starx + 220, stary + 121, 10, 10);
	outtextxy(starx + 65, stary + 95, bookAdd.author);

	//增加出版社信息
	outtextxy(starx + 5, stary + 135, L"出版社");
	fillroundrect(starx + 60, stary + 131, starx + 220, stary + 161, 10, 10);
	//_stprintf_s(text, L"%d", stAdd.data);
	outtextxy(starx + 65, stary + 135, bookAdd.publisher);

	//增加购买时间
	outtextxy(starx + 5, stary + 175, L"购买时间");
	fillroundrect(starx + 60, stary + 171, starx + 220, stary + 201, 10, 10);
	//_stprintf_s(text, L"%d", bookAdd.time);
	outtextxy(starx + 65, stary + 175, bookAdd.time);

	//增加价格
	outtextxy(starx + 5, stary + 215, L"价格");
	fillroundrect(starx + 60, stary + 211, starx + 220, stary + 241, 10, 10);
	_stprintf_s(text, L"%d", bookAdd.price);
	outtextxy(starx + 65, stary + 215, text);
}

//绘制查询时间
void drawtime() {
	int starx = 300, stary = 150;
	outtextxy(starx + 5, stary + 255, L"耗时");
	fillroundrect(starx + 60, stary + 251, starx + 220, stary + 281, 10, 10);
	wchar_t text[100];
	_stprintf_s(text, L"%f微秒", times);
	outtextxy(starx + 65, stary + 255, text);
}

//清空StuAdd
void setUpBook() {
	bookAdd.id = 0;
	wcscpy_s(bookAdd.name, L"NULL");
	wcscpy_s(bookAdd.author, L"NULL");
	wcscpy_s(bookAdd.publisher, L"NULL");
	wcscpy_s(bookAdd.time, L"NULL");
	bookAdd.price = 0;
	bookAdd.lchild = NULL;
	bookAdd.rchild = NULL;
}