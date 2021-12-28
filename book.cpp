#include "book.h"

// ��ʼ������
void initQueue(SqQueue** q) {
    if (!((*q) = (SqQueue*)malloc(sizeof(SqQueue)))) {
        printf("�ڴ����ʧ�ܣ�");
        exit(-1);
    }
    (*q)->front = (*q)->rear = -1; // �� -1
}


// �ж϶����Ƿ�Ϊ��
bool emptyQueue(SqQueue* q) {
    // ��ָ���βָ����ȣ�˵��Ϊ�ա���-�����棬����-���ؼ�
    if (q->front == q->rear) {
        return true;
    }
    return false;
}

// ������
bool enQueue(SqQueue* q, booknode* node) {
    // �ж϶����Ƿ����ˡ���������ʧ�ܣ�-���ؼ٣�����������ɹ���-������
    if (q->rear == MAX_SIZE - 1) {
        return false;
    }
    q->rear++;               // ͷָ��� 1
    q->data[q->rear] = node; // ��ֵ
    return true;
}

// ������
bool deQueue(SqQueue* q, booknode** node) {
    // �ж��Ƿ���ˡ��գ�ȡ��ʧ�ܣ�-���ؼ٣����գ�ȡ���ɹ���-������
    if (q->front == q->rear) {
        return false;
    }
    q->front++;                // βָ��� 1
    *node = q->data[q->front]; // ȡֵ
    return true;
}

//����ת
void R_Rotate(booktree* p) {
    booktree L;
    L = (*p)->lchild;
    (*p)->lchild = L->rchild;
    L->rchild = (*p);
    *p = L;
}

//����ת
void L_Rotate(booktree* p) {
    booktree R;
    R = (*p)->rchild;
    (*p)->rchild = R->lchild;
    R->lchild = (*p);
    *p = R;
}

//����ת������
void LeftBalance(booktree* T)
{
    booktree lc,rd;
    lc = (*T)->lchild;
    //�鿴�� T ��������Ϊ������������ʧȥƽ���ԭ����� bf ֵΪ 1 ����˵�������������Ϊ�������������У���Ҫ�����������������֮����� bf ֵΪ -1��˵���������������Ϊ�������������У���Ҫ����˫���������������Ĵ���
    switch (lc->bf)
    {
        case LH:
            (*T)->bf = lc->bf = EH;
            R_Rotate(T);
            break;
        case RH:
            rd = lc->rchild;
            switch(rd->bf)
        {
            case LH:
                (*T)->bf = RH;
                lc->bf = EH;
                break;
            case EH:
                (*T)->bf = lc->bf = EH;
                break;
            case RH:
                (*T)->bf = EH;
                lc->bf = LH;
                break;
        }
            rd->bf = EH;
            L_Rotate(&(*T)->lchild);
            R_Rotate(T);
            break;
    }
}
//��������ƽ�⴦��ͬ��������ƽ�⴦����ȫ����
void RightBalance(booktree* T)
{
    booktree lc,rd;
    lc= (*T)->rchild;
    switch (lc->bf)
    {
        case RH:
            (*T)->bf = lc->bf = EH;
            L_Rotate(T);
            break;
        case LH:
            rd = lc->lchild;
            switch(rd->bf)
        {
            case LH:
                (*T)->bf = EH;
                lc->bf = RH;
                break;
            case EH:
                (*T)->bf = lc->bf = EH;
                break;
            case RH:
                (*T)->bf = EH;
                lc->bf = LH;
                break;
        }
            rd->bf = EH;
            R_Rotate(&(*T)->rchild);
            L_Rotate(T);
            break;
    }
}

//ͼ��Ԫ�ز���
bool SearchBST(booktree BT, int key, booktree f, booktree* p) {//��Ҫ����4��������һ��������Ҫ���ҵ�ֵ������һ������һ��ָ��
    if (!BT) {
        *p = f;
        return false;
    }
    else if (key == BT->id) {
        *p = BT;
        return true;
    }
    else if (key < BT->id) {
        return SearchBST(BT->lchild, key, BT, p);
    }
    else {
        return SearchBST(BT->rchild, key, BT, p);
    }
}

int InsertAVL(booktree* T, node* s, node book, bool* taller)
{
    //�������Ϊ��������ֱ����� e Ϊ�����
    if ((*T) == NULL)
    {
        *s = book;
        *T = s;
        (*T)->bf = EH;
        *taller = true;
    }
    //����������������Ѿ����� e �������κδ���
    else if (book.id == (*T)->id)
    {
        *taller = false;
        return 0;
    }
    //��� e С�ڽ�� T ������������뵽 T ����������
    else if (book.id < (*T)->id)
    {
        //���������̣�����Ӱ���������ƽ�⣬��ֱ�ӽ���
        if (!InsertAVL(&(*T)->lchild, s,book, taller))
            return 0;
        //�жϲ�������Ƿ�ᵼ������������� +1
        if (*taller)
        {
            //�жϸ���� T ��ƽ�������Ƕ��٣���������������������½��Ĺ����е���ʧȥƽ�⣬���Ե� T ����ƽ�����ӱ���Ϊ 1 ʱ����Ҫ������������ƽ�⴦������������и�����ƽ��������
            switch ((*T)->bf)
            {
            case LH:
                LeftBalance(T);
                *taller = false;
                break;
            case  EH:
                (*T)->bf = LH;
                *taller = true;
                break;
            case RH:
                (*T)->bf = EH;
                *taller = false;
                break;
            }
        }
    }
    //ͬ������ e>T->data ʱ����Ҫ���뵽�� T Ϊ�����������������У�ͬ����Ҫ��������ͬ���Ĳ���
    else
    {
        if (!InsertAVL(&(*T)->rchild, s, book, taller))
            return 0;
        if (*taller)
        {
            switch ((*T)->bf)
            {
            case LH:
                (*T)->bf = EH;
                *taller = false;
                break;
            case EH:
                (*T)->bf = RH;
                *taller = true;
                break;
            case RH:
                RightBalance(T);
                *taller = false;
                break;
            }
        }
    }
    return 1;
}

//ͼ����غ��� ��ͼ����ļ��ж�ȡ����
int load(booktree* L) {
    locale china("zh_CN.UTF-8");
    wifstream infile;
    infile.open("books.txt");
    node book = { 0 };
    int bookNum = 0;
    if (infile.is_open()) {
        infile.imbue(china);
        while (!infile.eof()) {
            infile >> book.id >> book.name >> book.author >> book.publisher >> book.time >> book.price;
            
            node* s = new node;
            if (s != NULL) {
                if (*L == NULL) {
                    *s = book;
                    *L = s;
                    continue;
                }
                
                booknode* p;
                if (!SearchBST(*L, book.id, NULL, &p))  //�������ʧ�ܣ�����룻��ʱpָ����������һ���ڵ�
                {
                    *s = book;
                    if (book.id < p->id)  //��s��Ϊp������
                    {
                        p->lchild = s;
                    }
                    else if (book.id > p->id)  //��s��Ϊp���Һ���
                    {
                        p->rchild = s;
                    }
                }
                else
                {
                    ;
                }
            }
            bookNum++;
            if (infile.eof())
                break;
        }
    }
    infile.close();
    return bookNum;
}

int load2(booktree* L, bool* taller) {
    locale china("zh_CN.UTF-8");
    wifstream infile;
    infile.open("books.txt");
    node book = { 0 };
    int bookNum = 0;
    if (infile.is_open()) {
        infile.imbue(china);
        while (!infile.eof()) {
            infile >> book.id >> book.name >> book.author >> book.publisher >> book.time >> book.price;

            node* s = new node;
            InsertAVL(L, s, book, taller);
            bookNum++;
            if (infile.eof())
                break;
        }
    }
    infile.close();
    return bookNum-1;
}

//����ͼ����Ϣ
int save(booktree* L) {
    locale china("zh_CN.UTF-8");
    wofstream outfile("books.txt");
    outfile.imbue(china);
    if (!outfile.is_open()) {
        return 0;
    }
    booknode* stack[50];//����һ��ջ(����˵��һ��ָ������)
    int top = -1;//topָ��ջ�ĵײ�(��������ָ��������±�)
    booknode* p = new booknode;//����һ��ָ��p��ָ�������������
    p = *L;
    while ((p) != NULL || top != -1) {//���p������NULL������top��ָ��ջ��(���鲻Ϊ��)����ô��ִ��ѭ������pΪ�գ�����ջ(����)Ϊ�յ�ʱ�����ѭ������֮ʱ
        if (p != NULL) {//�жϵ�ǰָ��Ľ���ǲ���NULL���������ִ��ѭ��
            stack[++top] = p;//�ڵ㲻��NULL������ջ����ǰ��һλ��Ȼ��p����ջ�У�
            outfile << p->id << "\t" << p->name << "\t" << p->author << "\t";
            outfile<<p->publisher << "\t" << p->time<<"\t" << p->price <<endl;//��ջʱ���������
            p = p->lchild;//Ȼ��pָ����������
        }
        else {//ִ������˵��p����NULL�ˣ�˵����������������
            p = stack[top--];//��ջ��Ԫ�ص�������Ȼ��ֵ��p��Ȼ��ջ������
            p = p->rchild;//Ȼ������pָ���������������ٻص�whileѭ����
        }
    }
    outfile.close();
    return 1;
}


//����ͼ��
int addBookInfo(booktree* h, node e, int* bookNum) {
    node* s = new node;
    booknode* p;
    *s = e;

    if (!SearchBST(*h, e.id, NULL, &p))  //�������ʧ�ܣ�����룻��ʱpָ����������һ���ڵ�
    {
        if (e.id < (*h)->id)  //��s��Ϊp������
        {
            p->lchild = s;
            (*bookNum)++;
        }
        else if (e.id > (*h)->id)  //��s��Ϊp���Һ���
        {
            p->rchild = s;
            (*bookNum)++;
        }
    }
    else {
        return 0;
    }
    save(h);
    return 1;
}

//����ͼ��
int addBookInfo2(booktree* h, node e, int* bookNum, bool* taller) {
    node* s = new node;
    booknode* p;
    *s = e;
    s->bf = 0;
    *taller = true;

    if (!SearchBST(*h, e.id, NULL, &p))  //�������ʧ�ܣ�����룻��ʱpָ����������һ���ڵ�
    {
        InsertAVL(h, s, e, taller);
    }
    else {
        *taller = false;
        return 0;
    }
    save(h);
    return 1;
}

//��ͼ����id����ѯͼ�飬���Ҳ�������NULL
node* searchbook(int id, node* L, booktree* p) {
    if (!L) {
        p = new booktree;
        *p = NULL;
        return* p;
    }
    else if (id == L->id) {
        p = new booktree;
        *p = L;
        return *p;
    }
    else if (id < L->id) {
        return searchbook(id,L->lchild,p);
    }
    else {
        return searchbook(id,L->rchild,p);
    }
}

//��ͼ����id����ѯͼ�飬���Ҳ�������NULL
node* searchbookplus(int id, node* L, booktree* p) {
    while (1) {
        if (!L) {
            p = new booktree;
            *p = NULL;
            return*p;
        }else if (id == L->id) {
            p = new booktree;
            *p = L;
            return *p;
        }
        else if (id < L->id) {
            L = L->lchild;
        }
        else {
            L = L->rchild;
        }
    }

}

//ɾ��ͼ��
void Delete(booktree* p) {
    booktree q, s;
    if ((*p)->rchild == NULL) {
        q = *p;
        *p = (*p)->lchild;
        free(q);
    }
    else if ((*p)->lchild == NULL) {
        q = *p;
        *p = (*p)->rchild;
        free(q);
    }
    else {
        q = *p;
        s = (*p)->lchild;
        while (s->rchild) {
            q = s;
            s = s->rchild;
        }
        (*p)->id = s->id;
        if (q != *p)
            q->rchild = s->lchild;
        else
        {
            q->lchild = s->lchild;
        }
        free(s);
    }
}

void DeleteBST(booktree* T, node* p) {
    if ((*T)->id == (p)->id)
        Delete(T);
    else if ((*T)->id > (p)->id)
        DeleteBST(&(*T)->lchild, p);
    else
        DeleteBST(&(*T)->rchild, p);
}

//�������ͼ��
void InorderTra(node* L, booknode* BB[], int* i) {
    booknode* stack[30];//����һ��ջ
    int top = -1;//topָ��ջ�ĵײ�
    int a = -1;
    booknode* p = new booknode;
    p = L;//����һ��ָ��p��ָ�������������
    while (p != NULL || top != -1) {//���p������NULL������top��ָ��ջ�ף���ô��ִ��ѭ������pΪ�գ�����ջΪ�յ�ʱ�����ѭ������֮ʱ
        if (p != NULL) {//�жϵ�ǰָ��Ľ���ǲ���NULL���������ִ��ѭ��
            stack[++top] = p;//�ڵ㲻��NULL������ջ����ǰ��һλ��Ȼ��p����ջ��
            p = p->lchild;//Ȼ��pָ����������
        }
        else {//ִ������˵��p����NULL�ˣ�˵����������������
            BB[++a] = new node;
            p = stack[top--];//��ջ��Ԫ�ص�������Ȼ��ֵ��p��Ȼ��ջ������
            BB[a] = p;//printf("%c ", p->element); //��ջʱ���������
            p = p->rchild;//Ȼ������pָ���������������ٻص�whileѭ����
        }
    }
}

void refresh(node* L, int px, int py, int x, int y, int level)
{
    int tx;
    TCHAR s[6];
    char ss[6];
    int i;

    s[0] = s[1] = s[2] = s[3] = s[4]= s[5] = '\0';
    if (level == 1)
    {
        tx = 180;
    }
    else if (level == 2)
    {
        tx = 90;
    }
    else if (level == 3)
    {
        tx = 45;
    }
    else if(level==4)
    {
        tx = 23;//18
    }
    else {
        tx = 12;//9
    }
    //setbkcolor(0x273500);
    setfillcolor(0x673500);
    setlinecolor(0x673500);
    setlinestyle(PS_SOLID | PS_ENDCAP_FLAT, 4);
    settextcolor(WHITE);
    settextstyle(20, 0, _T("����"));
    if (L != NULL)
    {
        line(px, py, x, y);
        fillcircle(x, y, 20);
        refresh(L->lchild, x, y, x - tx, y + 98, level + 1);
        refresh(L->rchild, x, y, x + tx, y + 98, level + 1);
        sprintf_s(ss, "%d", L->id);
        for (i = 0; i < 5; i++)
        {
            s[i] = ss[i];
        }
        if (L->id < 0)
        {
            outtextxy(x - 22, y - 10, s);
        }
        else
        {
            outtextxy(x - 15, y - 10, s);
        }
    }
}

/*****************************************************************************
* @date   2020/4/19
* @brief  ˮƽ����
* @param  node	�������ڵ�
* @param  left	�ж�����
* @param  str 	�ɱ��ַ���
*****************************************************************************/
void draw_level(booknode* node, bool left, char* str) {
    if (node->rchild) {
        draw_level(node->rchild, false, strcat(str, (left ? "|     " : "      ")));
    }

    printf("%s", str);
    printf("%c", (left ? '\\' : '/'));
    printf("-----");
    printf("%d\n", node->id);

    if (node->lchild) {
        draw_level(node->lchild, true, strcat(str, (left ? "      " : "|     ")));
    }
    //  "      " : "|     " ����Ϊ 6
    str[strlen(str) - 6] = '\0';
}

/*****************************************************************************
* @date   2020/4/19
* @brief  ���ڵ㻭��
* @param  root	���������ڵ�
*****************************************************************************/
void draw(booknode* root) {
    char str[1000];
    memset(str, '\0', 1000);

    /**
     * 1. �� windows �£������ǿ�ִ�е�
     * 2. �� Linux   �£�ִ�лᱨ Segmentation fault
     *      ��Ҫʹ���м����
     */
    if (root->rchild) {
        draw_level(root->rchild, false, str);
    }
    printf("%d\n", root->id);
    if (root->lchild) {
        draw_level(root->lchild, true, str);
    }
}
//int main() {
//    node *L=NULL;
//    int bookNum = 0;
//    bookNum = load(L);
//    printf("%d", bookNum);
//    return 0;
//}