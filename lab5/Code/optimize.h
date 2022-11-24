#pragma once
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



void IR_optimize(FILE* file1, FILE* file2);