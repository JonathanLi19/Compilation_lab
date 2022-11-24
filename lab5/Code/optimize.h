#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
typedef struct Node_* Node;
struct Node_
{
    char exp[1023];
    Node next;
};//存放SSA指令的链表节点

struct T_state
{
    int is_dead; //该t_id是否已经dead
    int refer_id; //如果t_id已经dead，它的替代品的id
};

struct exp_
{
    char exp[1023]; //存放这个子表达式的等号右边
    int refer_id; //这个子表达式的左边是t_referid
};

void IR_optimize(FILE* file1, FILE* file2);