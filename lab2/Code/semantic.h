#ifndef SEMANTIC_H
#define SEMANTIC_H
#include "symbols_hashtable.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifndef TREE_H
#define TREE_H
typedef enum {
    LEX_INT=0,
    LEX_FLOAT=1,
    LEX_ID=2,
    LEX_TYPE=3,
    OTHERS     //syntax unit or other LEX type
}NODE_TYPE;
struct Node
{              // multi-way tree
    struct Node *child;
    struct Node* next_sib;
    char name[32];//节点名称
    union
    {
        int int_content;
        float float_content;
        char string_content[32];
    };//包含的内容
    int unit;//unit type: 0--lexical,1--syntax;
    int line_num;//表示行数;
    NODE_TYPE type;
};
extern void func(struct Node* s_node,int h);
#endif
void print_error(int err_type, int err_col, char *message);
int Program_check(struct Node *cur_node);
int ExtDefList_check(struct Node *cur_node);
int ExtDef_check(struct Node *cur_node);
int CompSt_check(struct Node *cur_node, hash_stack cur_stack, Type cur_type);
int StmtList_check(struct Node *cur_node, hash_stack cur_stack, Type cur_type);
int Stmt_check(struct Node *cur_node, hash_stack cur_stack, Type cur_type);
int DefList_check(struct Node *cur_node, hash_stack cur_stack);
int Def_check(struct Node *cur_node, hash_stack cur_stack);
int DecList_check(struct Node *cur_node, hash_stack cur_stack, Type cur_type);
int Dec_check(struct Node *cur_node, hash_stack cur_stack, Type cur_type);
Type Exp_check(struct Node *cur_node);
int Arg_check(struct Node *cur_node, FieldList paras);
int FunDec_check(struct Node *cur_node, const int is_define, const Type cur_type, hash_stack cur_stack);
FieldList VarList_check(struct Node *cur_node, hash_stack cur_stack);
FieldList ParamDec_check(struct Node *cur_node);
Type Specifier_check(struct Node *cur_node);
FieldList Def_struct_check(struct Node *cur_node, char *struct_name);
FieldList Dec_struct_check(struct Node *cur_node, Type cur_type);
FieldList VarDec_check(struct Node *cur_node, Type cur_type);
int ExtDecList_check(struct Node *cur_node, Type cur_type);

#endif