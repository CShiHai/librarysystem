#include "book.h"

// 初始化队列
void initQueue(SqQueue** q) {
    if (!((*q) = (SqQueue*)malloc(sizeof(SqQueue)))) {
        printf("内存分配失败！");
        exit(-1);
    }
    (*q)->front = (*q)->rear = -1; // 置 -1
}


// 判断队列是否为空
bool emptyQueue(SqQueue* q) {
    // 首指针和尾指针相等，说明为空。空-返回真，不空-返回假
    if (q->front == q->rear) {
        return true;
    }
    return false;
}

// 进队列
bool enQueue(SqQueue* q, booknode* node) {
    // 判断队列是否满了。满（插入失败）-返回假，不满（插入成功）-返回真
    if (q->rear == MAX_SIZE - 1) {
        return false;
    }
    q->rear++;               // 头指针加 1
    q->data[q->rear] = node; // 传值
    return true;
}

// 出队列
bool deQueue(SqQueue* q, booknode** node) {
    // 判断是否空了。空（取出失败）-返回假，不空（取出成功）-返回真
    if (q->front == q->rear) {
        return false;
    }
    q->front++;                // 尾指针加 1
    *node = q->data[q->front]; // 取值
    return true;
}

//右旋转
void R_Rotate(booktree* p) {
    booktree L;
    L = (*p)->lchild;
    (*p)->lchild = L->rchild;
    L->rchild = (*p);
    *p = L;
}

//左旋转
void L_Rotate(booktree* p) {
    booktree R;
    R = (*p)->rchild;
    (*p)->rchild = R->lchild;
    R->lchild = (*p);
    *p = R;
}

//左旋转处理函数
void LeftBalance(booktree* T)
{
    booktree lc,rd;
    lc = (*T)->lchild;
    //查看以 T 的左子树为根结点的子树，失去平衡的原因，如果 bf 值为 1 ，则说明添加在左子树为根结点的左子树中，需要对其进行右旋处理；反之，如果 bf 值为 -1，说明添加在以左子树为根结点的右子树中，需要进行双向先左旋后右旋的处理
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
//右子树的平衡处理同左子树的平衡处理完全类似
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

//图书元素查找
bool SearchBST(booktree BT, int key, booktree f, booktree* p) {//需要传入4个参数，一棵树，需要查找的值，还是一颗树，一个指针
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
    //如果本身为空树，则直接添加 e 为根结点
    if ((*T) == NULL)
    {
        *s = book;
        *T = s;
        (*T)->bf = EH;
        *taller = true;
    }
    //如果二叉排序树中已经存在 e ，则不做任何处理
    else if (book.id == (*T)->id)
    {
        *taller = false;
        return 0;
    }
    //如果 e 小于结点 T 的数据域，则插入到 T 的左子树中
    else if (book.id < (*T)->id)
    {
        //如果插入过程，不会影响树本身的平衡，则直接结束
        if (!InsertAVL(&(*T)->lchild, s,book, taller))
            return 0;
        //判断插入过程是否会导致整棵树的深度 +1
        if (*taller)
        {
            //判断根结点 T 的平衡因子是多少，由于是在其左子树添加新结点的过程中导致失去平衡，所以当 T 结点的平衡因子本身为 1 时，需要进行左子树的平衡处理，否则更新树中各结点的平衡因子数
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
    //同样，当 e>T->data 时，需要插入到以 T 为根结点的树的右子树中，同样需要做和以上同样的操作
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

//图书加载函数 将图书从文件中读取出来
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
                if (!SearchBST(*L, book.id, NULL, &p))  //如果查找失败，则插入；此时p指向遍历的最后一个节点
                {
                    *s = book;
                    if (book.id < p->id)  //将s作为p的左孩子
                    {
                        p->lchild = s;
                    }
                    else if (book.id > p->id)  //将s作为p的右孩子
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

//保存图书信息
int save(booktree* L) {
    locale china("zh_CN.UTF-8");
    wofstream outfile("books.txt");
    outfile.imbue(china);
    if (!outfile.is_open()) {
        return 0;
    }
    booknode* stack[50];//声明一个栈(或者说是一个指针数组)
    int top = -1;//top指向栈的底部(用来控制指针数组的下标)
    booknode* p = new booknode;//定义一个指针p，指向这个树的树根
    p = *L;
    while ((p) != NULL || top != -1) {//如果p不等于NULL，或者top不指向栈底(数组不为空)，那么就执行循环，当p为空，而且栈(数组)为空的时候就是循环结束之时
        if (p != NULL) {//判断当前指向的结点是不是NULL，如果不是执行循环
            stack[++top] = p;//节点不是NULL，先是栈顶向前进一位，然后将p送入栈中，
            outfile << p->id << "\t" << p->name << "\t" << p->author << "\t";
            outfile<<p->publisher << "\t" << p->time<<"\t" << p->price <<endl;//入栈时，访问输出
            p = p->lchild;//然后将p指向他的左孩子
        }
        else {//执行这里说明p等于NULL了，说明左子树遍历完了
            p = stack[top--];//将栈的元素弹出来，然后赋值给p，然后栈顶后退
            p = p->rchild;//然后再让p指向他的右子树。再回到while循环中
        }
    }
    outfile.close();
    return 1;
}


//增加图书
int addBookInfo(booktree* h, node e, int* bookNum) {
    node* s = new node;
    booknode* p;
    *s = e;

    if (!SearchBST(*h, e.id, NULL, &p))  //如果查找失败，则插入；此时p指向遍历的最后一个节点
    {
        if (e.id < (*h)->id)  //将s作为p的左孩子
        {
            p->lchild = s;
            (*bookNum)++;
        }
        else if (e.id > (*h)->id)  //将s作为p的右孩子
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

//增加图书
int addBookInfo2(booktree* h, node e, int* bookNum, bool* taller) {
    node* s = new node;
    booknode* p;
    *s = e;
    s->bf = 0;
    *taller = true;

    if (!SearchBST(*h, e.id, NULL, &p))  //如果查找失败，则插入；此时p指向遍历的最后一个节点
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

//用图书编号id来查询图书，，找不到返回NULL
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

//用图书编号id来查询图书，，找不到返回NULL
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

//删除图书
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

//中序遍历图书
void InorderTra(node* L, booknode* BB[], int* i) {
    booknode* stack[30];//声明一个栈
    int top = -1;//top指向栈的底部
    int a = -1;
    booknode* p = new booknode;
    p = L;//定义一个指针p，指向这个树的树根
    while (p != NULL || top != -1) {//如果p不等于NULL，或者top不指向栈底，那么就执行循环，当p为空，而且栈为空的时候就是循环结束之时
        if (p != NULL) {//判断当前指向的结点是不是NULL，如果不是执行循环
            stack[++top] = p;//节点不是NULL，先是栈顶向前进一位，然后将p送入栈中
            p = p->lchild;//然后将p指向他的左孩子
        }
        else {//执行这里说明p等于NULL了，说明左子树遍历完了
            BB[++a] = new node;
            p = stack[top--];//将栈的元素弹出来，然后赋值给p，然后栈顶后退
            BB[a] = p;//printf("%c ", p->element); //出栈时，访问输出
            p = p->rchild;//然后再让p指向他的右子树。再回到while循环中
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
    settextstyle(20, 0, _T("黑体"));
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
* @brief  水平画树
* @param  node	二叉树节点
* @param  left	判断左右
* @param  str 	可变字符串
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
    //  "      " : "|     " 长度为 6
    str[strlen(str) - 6] = '\0';
}

/*****************************************************************************
* @date   2020/4/19
* @brief  根节点画树
* @param  root	二叉树根节点
*****************************************************************************/
void draw(booknode* root) {
    char str[1000];
    memset(str, '\0', 1000);

    /**
     * 1. 在 windows 下，下面是可执行的
     * 2. 在 Linux   下，执行会报 Segmentation fault
     *      需要使用中间变量
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