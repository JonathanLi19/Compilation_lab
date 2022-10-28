#include "intermediate_code.h"
int varCount = 0, tmpCount = 0, labelCount = 0;
InterCodes head_code = NULL, tail_code = NULL;

//双向链表的插入
void Link_Insert(InterCodes cur)
{
    cur->prev = tail_code;
    cur->next = head_code;
    tail_code->next = cur;
    head_code->prev = cur;
    tail_code = cur;
}

//开始生成中间代码
int start_gen(struct Node* cur, FILE *fp)
{
    head_code = (InterCodes)malloc(sizeof(struct InterCodes_));
    head_code->next = NULL;
    head_code->prev = NULL;
    tail_code = head_code;

    Program_gen(cur);
    //printIntercode(fp); 待实现
}

void Program_gen(struct Node *cur)
{
    int result = ExtDefList_gen(getchild(cur, 0));
    assert(result != 0);
    return;
}