#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define MAX_SIZE 1000
#define LH +1
#define EH  0
#define RH -1
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include <windows.h>
#include <easyx.h>
#include <conio.h>
#include <graphics.h>
#include <fstream>
#include <locale>
#include <iostream>
using namespace std;

//定义图书的结构体
typedef struct node {
    int id;//图书编号
    wchar_t name[20];//书名
    wchar_t author[20];//图书作者
    wchar_t publisher[20];//图书的出版社
    wchar_t time[15];//购买时间，用字符串存储
    int price;//图书价格
    int bf;
    struct node* lchild=NULL;//结点左孩子
    struct node* rchild=NULL;//结点的右孩子
}booknode, * booktree;


//定义一个队列
typedef struct Quene {      // 定义顺序队
    int     front;          // 队头指针
    int     rear;           // 队尾指针
    booknode* data[MAX_SIZE]; // 存放队中元素
} SqQueue;


//按钮结构体
typedef struct Button {
    int x = 0, y = 0;//坐标
    wchar_t str[50] = L"按钮";//按钮内容
    COLORREF color[2] = { RGB(93, 107, 153),RGB(204,213,240) };//按钮颜色
    int status = 0;

}button;


// 初始化队列
void initQueue(SqQueue** q);

// 判断队列是否为空
bool emptyQueue(SqQueue* q);

// 进队列
bool enQueue(SqQueue* q, booknode* node);

// 出队列
bool deQueue(SqQueue* q, booknode** node);

//图书加载函数
int load(booktree* L);

//图书元素查找
bool SearchBST(booktree BT, int key, booktree f, booktree* p);

//保存图书信息
int save(booktree* L);

//增加图书
int addBookInfo(booktree* h, node e, int* bookNum);


//用图书编号id来查询图书，，找不到返回NULL
node* searchbook(int id, node* L, booktree* p);

node* searchbookplus(int id, node* L, booktree* p);

//删除图书
void Delete(booktree* p);
void DeleteBST(booktree* T, node* p);

//中序遍历函数
void InorderTra(node* L, booknode* BB[], int* i);

void R_Rotate(booktree* p);

void L_Rotate(booktree* p);

void LeftBalance(booktree* T);

void RightBalance(booktree* T);

int load2(booktree* L, bool* taller);

int addBookInfo2(booktree* h, node e, int* bookNum, bool* taller);

void refresh(node* L, int px, int py, int x, int y, int level);

void draw_level(booknode* node, bool left, char* str);
void draw(booknode* root);

//void searchBookInfoByArray(int id,booktree book);