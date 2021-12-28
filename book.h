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

//����ͼ��Ľṹ��
typedef struct node {
    int id;//ͼ����
    wchar_t name[20];//����
    wchar_t author[20];//ͼ������
    wchar_t publisher[20];//ͼ��ĳ�����
    wchar_t time[15];//����ʱ�䣬���ַ����洢
    int price;//ͼ��۸�
    int bf;
    struct node* lchild=NULL;//�������
    struct node* rchild=NULL;//�����Һ���
}booknode, * booktree;


//����һ������
typedef struct Quene {      // ����˳���
    int     front;          // ��ͷָ��
    int     rear;           // ��βָ��
    booknode* data[MAX_SIZE]; // ��Ŷ���Ԫ��
} SqQueue;


//��ť�ṹ��
typedef struct Button {
    int x = 0, y = 0;//����
    wchar_t str[50] = L"��ť";//��ť����
    COLORREF color[2] = { RGB(93, 107, 153),RGB(204,213,240) };//��ť��ɫ
    int status = 0;

}button;


// ��ʼ������
void initQueue(SqQueue** q);

// �ж϶����Ƿ�Ϊ��
bool emptyQueue(SqQueue* q);

// ������
bool enQueue(SqQueue* q, booknode* node);

// ������
bool deQueue(SqQueue* q, booknode** node);

//ͼ����غ���
int load(booktree* L);

//ͼ��Ԫ�ز���
bool SearchBST(booktree BT, int key, booktree f, booktree* p);

//����ͼ����Ϣ
int save(booktree* L);

//����ͼ��
int addBookInfo(booktree* h, node e, int* bookNum);


//��ͼ����id����ѯͼ�飬���Ҳ�������NULL
node* searchbook(int id, node* L, booktree* p);

node* searchbookplus(int id, node* L, booktree* p);

//ɾ��ͼ��
void Delete(booktree* p);
void DeleteBST(booktree* T, node* p);

//�����������
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