#include <easyx.h>
#include <graphics.h>
#include<time.h>
#include "book.h"


const int WIDTH = 42;//7�ı���
const int HEIGHT = 24;

bool taller;

IMAGE buffer(WIDTH * 30 + 160, HEIGHT * 20);

booktree book;
int bookNum = 0;//ͼ������
int uord = 0;//��û����
int uordsum = 0;//����ֵܷı�־λ����û����


button btn[8] = {
	{0,0,L"ͼ�����ϵͳ"},
	{0, 40, L"����ͼ����Ϣ"},
	{0, 80, L"ɾ��ͼ����Ϣ"},
	{0, 120, L"�޸�ͼ����Ϣ"},
	{0, 160,L"��ѯͼ����Ϣ"},
	{0,200,L"���ͼ����Ϣ"},
	{0, 240, L"�鿴���Ľṹ"},
	{0,280,L"  �˳�����"}
};

int btnNum = 8;//��������

button btnUp = { 200,435,L"  ��һҳ" };
button btnDown = { 650,435,L"  ��һҳ" };

node bookAdd = { 0,L"NULL",L"NULL" ,L"NULL",L"NULL",0 ,NULL };
node bookSearch= { 0,L"NULL",L"NULL" ,L"NULL",L"NULL",0 };

clock_t start, stop;
double times;


node* fixBook = NULL;
int btnShowNum = 0;

//��������
//���StuAdd
void setUpBook();
//����ʱ��
void drawtime();

//////////////
//������ͼ����//
/////////////
void drawView(int status);
//������ఴť
void drawLeft();
//���ư�ť
void drawButton(button BTN, int width, int height);

//���ð�ť״̬
void setUpButton(int index, int status);

//������Ʒ�����Ϣ
void drawOpusInfo();

//�������ͼ����Ϣ
void printBookInfo(node* L, int sbookNum, int num);

//����ͼ����Ϣ����
void printAddBookInfo(node* L);

//ɾ��ͼ����Ϣ����
void printDeleteBookInfo(node* L);

//��ѯͼ����Ϣ����
void printSearchBookInfo(node* L);

//�޸�ͼ����Ϣ����
void printFixBookInfo(node* L);

//�������ͼ����Ϣ����
void drawOneBookInfo();

///////////////
// ��ť��������//
//////////////

// ����ͼ����Ϣҳ�水ť����
void addInfoBtn(MOUSEMSG m);
//��������ť����
void menBtn(MOUSEMSG m, int* status);
//���ͼ����Ϣҳ�水ť����
void showBookInfoBtn(MOUSEMSG m);
//ɾ��ͼ����Ϣҳ�水ť����
void deleteBookInfoBtn(MOUSEMSG m);
//�޸�ͼ����Ϣҳ�水ť����
void fixBookInfoBtn(MOUSEMSG m, int* status);
//��ѯͼ����Ϣҳ�水ť����
void searchBookInfoBtn(MOUSEMSG m);
void searchBookInfoBtn(MOUSEMSG m, double time);
//�鿴������״
void showBookTree(MOUSEMSG m);
//����ƽ�������
int InsertAVL(booktree* T, node* s, node book, bool* taller);



int main() {
	initgraph(WIDTH * 20 + 160, HEIGHT * 20);

	MOUSEMSG m;
	int status = 0;
	int fixStatus = 0;
	bookNum = load2(&book,&taller);//��ʼ��ͼ����Ϣ
	draw(book);
	FlushMouseMsgBuffer();
	while (true) {

		while (MouseHit()) {
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN) {//�������
				if (m.x >= 0 && m.x <= 160) {
					menBtn(m, &status);
				}
				else if (m.x > 160 && m.x < WIDTH * 20 + 160) {
					switch (status) {
					case 1://������Ϣҳ�水������
						addInfoBtn(m);
						break;
					case 2://ɾ��ͼ����Ϣҳ�水������
						deleteBookInfoBtn(m);
						break;
					case 3://�޸�ͼ����Ϣҳ�水������
						fixBookInfoBtn(m, &fixStatus);
						break;
					case 4://��ѯͼ����Ϣҳ�水������
						searchBookInfoBtn(m);
						//printf("\n%f\n", times);
						break;
					case 5://����ɼ�ҳ�水��
						showBookInfoBtn(m);
						break;
					case 6://չʾ���Ľṹ
						showBookTree(m);
						break;
					}
				}
			}
			else if (m.uMsg == WM_LBUTTONUP) {//�������
				//���ð�ť״̬
				setUpButton(0, -1);
			}
		}
		drawView(status);
	}
	closegraph();
	return 0;
}

//������Ϣ
void drawView(int status) {
	SetWorkingImage(&buffer);
	// ����
	cleardevice();
	// ���ñ������ģʽ ����Ϊ͸��
	setbkmode(TRANSPARENT);
	setbkcolor(0xf0f0f0);

	//������๦�ܰ�ť
	drawLeft();

	//�����Ҳ���Ϣ
	switch (status) {
	case 0:
		drawOpusInfo();//������Ʒ��Ϣ
		break;
	case 1://����
		printAddBookInfo(book);
		break;
	case 2://ɾ��
		printDeleteBookInfo(book);
		break;
	case 3://�޸�
		printFixBookInfo(book);
		break;
	case 4://��ѯͼ��ɼ�
		printSearchBookInfo(book);
		break;
	case 5://���ͼ����Ϣ
		printBookInfo(book, bookNum, btnShowNum);
		break;
	case 6://����
		refresh(book, 550, 40, 550, 40, 1);
		break;
	}

	// ���õ�ǰ��ͼ�豸
	SetWorkingImage();
	// ����ͼ����Ļ
	putimage(0, 0, &buffer);
}


//��������ʽ
void drawLeft() {
	//���õײ㱳��
	setfillcolor(RGB(137, 101, 88));
	fillrectangle(0, 0, 160, HEIGHT * 20);
	//���ư�ť
	for (int i = 0; i < btnNum; i++) {
		drawButton(btn[i], 160, 30);
	}
}


//���ư�ť
void drawButton(button BTN, int width = 160, int height = 30) {
	setlinecolor(WHITE);
	setfillcolor(BTN.color[BTN.status]);
	int x = BTN.x, y = BTN.y;
	fillroundrect(x, y, x + width, y + height, 10, 10);
	settextcolor(BLACK);
	settextstyle(20, 0, _T("Consolas"));
	outtextxy(x + 28, y + 7, BTN.str);
}


//���ð�ť״̬
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
	gettextstyle(&f);						// ��ȡ��ǰ��������
	f.lfHeight = 64;
	f.lfQuality = ANTIALIASED_QUALITY;		// �������Ч��Ϊ�����  
	_tcscpy_s(f.lfFaceName, _T("�����п�"));		// ��������Ϊ�����塱
	settextstyle(&f);						// ����������ʽ
	outtextxy(300, 50, _T("ͼ����Ϣ����ϵͳ"));

	settextstyle(25, 0, _T("�����п�"));
	outtextxy(425, 135, _T("��ӭʹ��ͼ�����ϵͳ"));


	settextstyle(18, 0, _T("����"));
	outtextxy(300, 180, _T("��ϵͳ����7������ģ�飬����๦������ʾ��������ʹ��˵����"));
	outtextxy(300, 200, _T("�١�ͼ�����ϵͳģ�飺�ڱ�ģ������˸�ϵͳ��Ҫ�����Լ�������ʽ��"));
	outtextxy(300, 220, _T("�ڡ�����ͼ����Ϣ���ֱ�����Ӱ�ť���ɱ༭����ֵ���������ɼ�¼�±�����Ϣ��"));
	outtextxy(300, 240, _T("�ۡ�ɾ��ͼ����Ϣ��ʹ�ô�ģ�飬ͨ��ͼ��������ѯͼ����Ϣ��¼�����ɾ����ť"));
	outtextxy(300, 260, _T("    ��ɾ��������Ϣ��¼��"));
	outtextxy(300, 280, _T("�ܡ��޸�ͼ����Ϣ��ͨ����Ų�ѯ�鱾��Ϣ����ĳ��Ϣ���󣬿�ͨ���޸İ�ť�޸ġ�"));
	outtextxy(300, 300, _T("�ݡ���ѯͼ����Ϣ��ͨ���������ѯ��չʾͼ���������Ϣ��"));
	outtextxy(300, 320, _T("�ޡ����ͼ����Ϣ��ʵʱ���ϵͳ��¼�������ͼ����Ϣ��"));
	outtextxy(300, 340, _T("�ߡ��˳����򣺵�����˳�����"));
}

void printBookInfo(node* L, int BookNum, int num) {
	setfillcolor(RGB(204, 213, 240));
	fillrectangle(160, 0, WIDTH * 20 + 160, 75);

	settextstyle(50, 0, _T("�����п�"));
	outtextxy(380, 15, _T("ͼ����Ϣ��"));

	//������
	settextstyle(20, 0, _T("����"));
	LOGFONT f;
	gettextstyle(&f);
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	wchar_t str[1000];
	_stprintf_s(str, L"��ǰ��¼�ڿ�ͼ������Ϊ%d ��ǰ��%d/%dҳ", bookNum, num + 1, (bookNum - 1) / 15 + 1);
	outtextxy(165, 77, str);

	int numx = bookNum - 15 * num;//ʣ����ʾ����

	setlinecolor(BLACK);
	int div = WIDTH / 8;
	line(160, 75, WIDTH * 20 + 160, 75);

	int starx = 160, stary = 100;
	//������
	for (int i = 0; i < numx + 2 && i < 17; i++) {
		line(starx, stary + i * 20, WIDTH * 20 + 160, stary + i * 20);
		if (i == 0) {//��������
			//ͼ����
			outtextxy(starx + 5, stary + 1, L" ͼ����");
			//����
			outtextxy(starx + 1 * div * 20 + 5, stary + 1, L"  ����");
			//����
			outtextxy(starx + 3 * div * 20 + 5, stary + 1, L"  ����");
			//������
			outtextxy(starx + 4 * div * 20 + 5, stary + 1, L" ������");
			//�۸�
			outtextxy(starx + 6 * div * 20 + 5, stary + 1, L"  �۸�");
			//����ʱ��
			outtextxy(starx + 7 * div * 20 + 5, stary + 1, L"����ʱ��");
		}
	}

	//����������
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

	//���ͼ����Ϣ
	stary = stary + 20;
	booknode* pp[100];
	int *bn = 0;
	InorderTra(L, pp, bn);
	int cnt = 0;
	int i, len = 0;
	i = 15 * num;
	wchar_t text[500] = { 0 };
	while (i < bookNum && cnt < 15) {
		//ͼ����
		_stprintf_s(text, L"%d", pp[i]->id);
		outtextxy(starx + 5, stary + cnt * 20 + 1, text);
		//ͼ������
		outtextxy(starx + 1 * div * 20 + 5, stary + cnt * 20 + 1, pp[i]->name);
		//ͼ������
		outtextxy(starx + 3 * div * 20 + 5, stary + cnt * 20 + 1, pp[i]->author);
		//ͼ�������
		outtextxy(starx + 4 * div * 20 + 5, stary + cnt * 20 + 1, pp[i]->publisher);
		//ͼ��۸�
		_stprintf_s(text, L"%d", pp[i]->price);
		outtextxy(starx + 6 * div * 20 + 5, stary + cnt * 20 + 1, text);
		//����ʱ��
		outtextxy(starx + 7 * div * 20 + 5, stary + cnt * 20 + 1, pp[i]->time);
		cnt++;
		i++;
	}
	drawButton(btnUp);
	drawButton(btnDown);
}


//����ͼ����Ϣ����
void printAddBookInfo(node* L) {
	setfillcolor(RGB(204, 213, 240));
	fillrectangle(160, 0, WIDTH * 20 + 160, 75);

	settextstyle(50, 0, _T("�����п�"));
	outtextxy(380, 15, _T("      ����ͼ����Ϣ"));
	settextstyle(20, 0, _T("����"));
	wchar_t str[100];
	_stprintf_s(str, L"��ǰ�ڿ�ͼ������Ϊ%d ", bookNum);
	outtextxy(165, 77, str);

	//�������
	int starx = 300, stary = 150;
	drawOneBookInfo();
	//������ť
	button btnAdd[10] = {
		{ starx + 250,stary + 11,L"���" },
		{ starx + 250,stary + 51,L"���" },
		{ starx + 250,stary + 91,L"���" },
		{ starx + 250,stary + 131,L"���" },
		{ starx + 250,stary + 171,L"���" },
		{ starx + 250,stary + 211,L"���" },
		{ starx + 250,stary + 291,L"����" }
	};
	//���ư�ť
	for (int i = 0; i < 7; i++) {
		drawButton(btnAdd[i], 90, 30);
	}
}

//����ͼ����Ϣ���水ť����
void addInfoBtn(MOUSEMSG m) {
	if (m.x >= 550 && m.x <= 640) {
		if (m.y >= 161 && m.y <= 191) {//����ѧ��
			wchar_t s[100] = { 0 };
			InputBox(s, 100, L"��������");
			int id = _wtoi(s);
			bookAdd.id = id;
		}
		else if (m.y >= 201 && m.y <= 231) {//��������
			wchar_t s[100] = { 0 };
			InputBox(s, 100, L"����������");
			wcscpy_s(bookAdd.name, s);
		}
		else if (m.y >= 241 && m.y <= 271) {//��������
			wchar_t s[100] = { 0 };
			InputBox(s, 100, L"����������");
			wcscpy_s(bookAdd.author, s);
		}
		else if (m.y >= 281 && m.y <= 311) {//���ӳ�����
			wchar_t s[100] = { 0 };
			InputBox(s, 100, L"�����������");
			wcscpy_s(bookAdd.publisher, s);
		}
		else if (m.y >= 321 && m.y <= 361) {//���ӹ���ʱ��
			wchar_t s[100] = { 0 };
			InputBox(s, 100, L"�����빺��ʱ��");
			wcscpy_s(bookAdd.time, s);
		}
		else if (m.y >= 371 && m.y <= 401) {//���Ӽ۸�
			wchar_t s[100] = { 0 };
			InputBox(s, 100, L"������۸�");
			int price = _wtoi(s);
			bookAdd.price = price;
		}
		else if (m.y >= 451 && m.y <= 481) {//����(m.y >= 401 && m.y <= 431)����
			wchar_t text2[50] = L"�������";

			if (addBookInfo2(&book, bookAdd, &bookNum,&taller) == 1) {
				wchar_t text1[50] = L"����ɹ���";
				MessageBox(GetHWnd(), text1, text2, 0);
				bookNum++;
			}
			else {
				wchar_t text1[50] = L"����ʧ�ܣ�";
				MessageBox(GetHWnd(), text1, text2, 0);
			}
			//���bookAdd
			setUpBook();
		}
	}

}

//��������ť����
void menBtn(MOUSEMSG m, int* status) {
	if (m.y >= 0 && m.y <= 30) {//�ƶ���չʾ��Ʒ��Ϣ��ť
		setUpButton(0, 1);
		*status = 0;
		btnShowNum = 0;
		//drawOpusInfo();//������Ʒ��Ϣ

	}
	else if (m.y >= 40 && m.y <= 70) {//�ƶ�������ͼ����Ϣ��ť
		setUpButton(1, 1);
		*status = 1;
		btnShowNum = 0;
	}
	else if (m.y >= 80 && m.y <= 110) {//�ƶ���ɾ��ͼ����Ϣ��ť
		setUpButton(2, 1);
		btnShowNum = 0;
		*status = 2;

	}
	else if (m.y >= 120 && m.y <= 150) {//�ƶ����޸�ͼ����Ϣ��ť
		setUpButton(3, 1);
		btnShowNum = 0;
		*status = 3;
	}
	else if (m.y >= 160 && m.y <= 190) {//�ƶ�����ѯͼ����Ϣ��ť
		setUpButton(4, 1);
		btnShowNum = 0;
		*status = 4;

	}
	else if (m.y >= 200 && m.y <= 230) {//�ƶ������ͼ����Ϣ��ť
		setUpButton(5, 1);
		btnShowNum = 0;
		*status = 5;
	}
	else if (m.y >= 240 && m.y <= 270) {//�ƶ����˳�����ť
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


//ɾ��ͼ����Ϣ����
void printDeleteBookInfo(node* L) {
	setfillcolor(RGB(204, 213, 240));
	fillrectangle(160, 0, WIDTH * 20 + 160, 75);

	settextstyle(50, 0, _T("�����п�"));
	outtextxy(380, 15, _T("ͼ����Ϣɾ��"));
	settextstyle(20, 0, _T("����"));
	wchar_t str[100];
	_stprintf_s(str, L"��ǰ��¼ͼ������Ϊ%d ", bookNum);
	outtextxy(165, 77, str);

	//�������
	int starx = 300, stary = 150;
	setfillcolor(0xd5dce8);
	fillrectangle(starx, stary, starx + 400, stary + 290);

	//������ť
	button btnAdd[8] = {
		{700,110,L"ɾ��"},
		{ 280,110,L"��ͼ���Ų�ѯ" }
	};
	//���ư�ť
	drawButton(btnAdd[0], 80, 30);
	for (int i = 1; i < 2; i++) {
		drawButton(btnAdd[i], 180, 30);
	}
	drawOneBookInfo();
}

//��ѯͼ����Ϣ����
void printSearchBookInfo(node* L) {
	setfillcolor(RGB(204, 213, 240));
	fillrectangle(160, 0, WIDTH * 20 + 160, 75);

	settextstyle(50, 0, _T("�����п�"));
	outtextxy(380, 15, _T("ͼ����Ϣ��ѯ"));
	settextstyle(20, 0, _T("����"));
	wchar_t str[100];
	_stprintf_s(str, L"��¼�ڿ�ͼ������Ϊ%d ", bookNum);
	outtextxy(165, 77, str);



	//������ť
	button btnAdd[10] = {
		{ 280,110,L"��ͼ���Ų�ѯ" },
		{ 500,110,L"�����鷽ʽ��ѯ" }
	};
	//���ư�ť
	int btnNums = 2;
	for (int i = 0; i < btnNums; i++) {
		drawButton(btnAdd[i], 180, 30);
	}
	drawOneBookInfo();
	drawtime();
}

//�޸�ͼ����Ϣ����
void printFixBookInfo(node* L) {
	setfillcolor(RGB(204, 213, 240));
	fillrectangle(160, 0, WIDTH * 20 + 160, 75);

	settextstyle(50, 0, _T("�����п�"));
	outtextxy(380, 15, _T("ͼ����Ϣ�޸�"));
	settextstyle(20, 0, _T("����"));
	wchar_t str[100];
	_stprintf_s(str, L"��¼��ͼ������Ϊ%d ", bookNum);
	outtextxy(165, 77, str);

	//�������
	int starx = 300, stary = 150;
	drawOneBookInfo();

	//������ť
	button btnAdd[10] = {
		{ starx + 200,stary - 40,L" ��ѯͼ����" },
		{ starx + 250,stary + 11,L"�޸�" },
		{ starx + 250,stary + 51,L"�޸�" },
		{ starx + 250,stary + 91,L"�޸�" },
		{ starx + 250,stary + 131,L"�޸�" },
		{ starx + 250,stary + 171,L"�޸�" },
		{ starx + 250,stary + 211,L"�޸�" },
		{ starx + 250,stary + 291,L"����" }
	};
	//���ư�ť
	int btnNums = 8;
	drawButton(btnAdd[0], 180, 30);
	for (int i = 1; i < btnNums; i++) {
		drawButton(btnAdd[i], 90, 30);
	}

}

void showBookInfoBtn(MOUSEMSG m) {
	if (m.x >= 200 && m.x <= 360 && m.y >= 435 && m.y <= 465) {//��һҳ
		if (btnShowNum > 0)
			btnShowNum--;
	}

	if (m.x >= btnDown.x && m.x <= btnDown.x + 160 && m.y >= 435 && m.y <= 465) {//��һҳ
		if (btnShowNum < (bookNum - 1) / 15)
			btnShowNum++;
	}
	//---------------------------------------------------------------------��������ɼ�����
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
	//**************************************************************************�������ѧ������
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


//ɾ��ͼ����Ϣҳ�水ť����
void deleteBookInfoBtn(MOUSEMSG m) {
	node* st = new node;
	st = NULL;
	if (m.x >= 280 && m.x <= 460 && m.y >= 110 && m.y <= 140) {
		//��ͼ���Ų�ѯ��ť
		wchar_t s[100] = { 0 };
		InputBox(s, 100, L"�������ѯ�ı��");
		int id = _wtoi(s);
		//��ȡ��Ӧͼʾ��λ��
		booknode *st1;
		st = searchbook(id, book,&st1);
		//
		if (st == NULL) {//��ѯ����ʧ��
			wchar_t text1[50] = { L"����Ϣ��¼" }, text2[50] = { L"��ѯ���" };
			MessageBox(GetHWnd(), text1, text2, 0);
		}
		else {
			bookAdd = *st;
		}
	}

	if (&bookAdd != NULL && m.x >= 700 && m.x <= 790 && m.y >= 110 && m.y <= 140) {
		//ɾ����ť
		DeleteBST(&book,&(bookAdd));
		setUpBook();
		save(&book);
		bookNum--;
		wchar_t text1[50] = { L"ɾ���ɹ���" }, text2[50] = { L"ɾ�����" };
		MessageBox(GetHWnd(), text1, text2, 0);
	}
}

void showBookTree(MOUSEMSG m) {

}

//��ѯͼ����Ϣҳ�水ť����
void searchBookInfoBtn(MOUSEMSG m) {
	node* st = NULL;
	if (m.x >= 280 && m.x <= 460 && m.y >= 110 && m.y <= 140) {
		//��ѧ�Ų�ѯ��ť
		wchar_t s[100] = { 0 };
		InputBox(s, 100, L"�������ѯ�ı��");
		int id = _wtoi(s);
		//��ȡ��Ӧͼ���λ��
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
		if (st == NULL) {//��ѯʧ��
			wchar_t text1[50] = { L"����Ϣ��¼" }, text2[50] = { L"��ѯ���" };
			MessageBox(GetHWnd(), text1, text2, 0);
		}
		else {
			bookAdd = *st;
		}
	}
	else if (m.x >= 500 && m.x <= 680 && m.y >= 110 && m.y <= 140) {
		//�������ѯ
		wchar_t s[100] = { 0 };
		InputBox(s, 100, L"��ͼ���Ų�ѯ����");
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
		if (st == NULL) {//��ѯʧ��
			wchar_t text1[50] = { L"����Ϣ��¼" }, text2[50] = { L"��ѯ���" };
			MessageBox(GetHWnd(), text1, text2, 0);
		}
		else {
			bookAdd = *st;
		}
	}
}

//�޸�ͼ����Ϣ���水ť����
void fixBookInfoBtn(MOUSEMSG m, int* status) {
	if (m.x >= 500 && m.x <= 680 && m.y >= 110 && m.y <= 140) {//��ѧ�Ų�ѯ
		wchar_t s[100] = { 0 };
		InputBox(s, 100, L"������Ҫ��ѯ��ͼ����");
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
			if (m.y >= 161 && m.y <= 191) {//�޸�ѧ��
				wchar_t s[100] = { 0 };
				InputBox(s, 100, L"�������±��");
				int id = _wtoi(s);
				bookAdd.id = id;
			}
			else if (m.y >= 201 && m.y <= 231) {//�޸�����
				wchar_t s[100] = { 0 };
				InputBox(s, 100, L"������������");
				wcscpy_s(bookAdd.name, s);
			}
			else if (m.y >= 241 && m.y <= 271) {//�޸�����
				wchar_t s[100] = { 0 };
				InputBox(s, 100, L"������������");
				wcscpy_s(bookAdd.author, s);
			}
			else if (m.y >= 281 && m.y <= 311) {//�޸ĳ�����
				wchar_t s[100] = { 0 };
				InputBox(s, 100, L"�������³�����");//
				wcscpy_s(bookAdd.publisher, s);
			}
			else if (m.y >= 321 && m.y <= 351) {//�޸Ĺ���ʱ��
				wchar_t s[100] = { 0 };
				InputBox(s, 100, L"�������¹���ʱ��");
				wcscpy_s(bookAdd.time, s);
			}
			else if (m.y >= 361 && m.y <= 391) {//�޸ļ۸�
				wchar_t s[100] = { 0 };
				InputBox(s, 100, L"�������¼۸�");
				int price = _wtoi(s);
				bookAdd.price = price;
			}
			else if (m.y >= 441 && m.y <= 471) {
				if (fixBook != NULL)
					*fixBook = bookAdd;
				wchar_t text2[50] = L"�������";

				if (save(&book) == 1 && fixBook != NULL) {
					wchar_t text1[50] = L"����ɹ���";
					MessageBox(GetHWnd(), text1, text2, 0);
				}
				else {
					wchar_t text1[50] = L"����ʧ�ܣ�";
					MessageBox(GetHWnd(), text1, text2, 0);
				}
				//���stAdd
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

//���Ƶ�����ͼ����Ϣ
void drawOneBookInfo() {
	settextstyle(20, 0, _T("Consolas"));
	//�������
	int starx = 300, stary = 150;
	setfillcolor(RGB(204, 213, 240));
	fillrectangle(starx, stary, starx + 400, stary + 330);
	//������ʾ
	setfillcolor(0xf0f0f0);
	wchar_t text[100];
	//���ӱ����Ϣ
	outtextxy(starx + 5, stary + 15, L"���");
	fillroundrect(starx + 60, stary + 11, starx + 220, stary + 41, 10, 10);
	_stprintf_s(text, L"%d", bookAdd.id);
	outtextxy(starx + 65, stary + 15, text);

	//����������Ϣ
	outtextxy(starx + 5, stary + 55, L"����");
	fillroundrect(starx + 60, stary + 51, starx + 220, stary + 81, 10, 10);
	outtextxy(starx + 65, stary + 55, bookAdd.name);

	//����������Ϣ
	outtextxy(starx + 5, stary + 95, L"����");
	fillroundrect(starx + 60, stary + 91, starx + 220, stary + 121, 10, 10);
	outtextxy(starx + 65, stary + 95, bookAdd.author);

	//���ӳ�������Ϣ
	outtextxy(starx + 5, stary + 135, L"������");
	fillroundrect(starx + 60, stary + 131, starx + 220, stary + 161, 10, 10);
	//_stprintf_s(text, L"%d", stAdd.data);
	outtextxy(starx + 65, stary + 135, bookAdd.publisher);

	//���ӹ���ʱ��
	outtextxy(starx + 5, stary + 175, L"����ʱ��");
	fillroundrect(starx + 60, stary + 171, starx + 220, stary + 201, 10, 10);
	//_stprintf_s(text, L"%d", bookAdd.time);
	outtextxy(starx + 65, stary + 175, bookAdd.time);

	//���Ӽ۸�
	outtextxy(starx + 5, stary + 215, L"�۸�");
	fillroundrect(starx + 60, stary + 211, starx + 220, stary + 241, 10, 10);
	_stprintf_s(text, L"%d", bookAdd.price);
	outtextxy(starx + 65, stary + 215, text);
}

//���Ʋ�ѯʱ��
void drawtime() {
	int starx = 300, stary = 150;
	outtextxy(starx + 5, stary + 255, L"��ʱ");
	fillroundrect(starx + 60, stary + 251, starx + 220, stary + 281, 10, 10);
	wchar_t text[100];
	_stprintf_s(text, L"%f΢��", times);
	outtextxy(starx + 65, stary + 255, text);
}

//���StuAdd
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