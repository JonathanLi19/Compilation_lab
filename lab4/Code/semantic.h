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
int Program(struct Node *cur_node);
int ExtDefList(struct Node *cur_node);
int ExtDef(struct Node *cur_node);
int CompSt(struct Node *cur_node, hash_stack cur_stack, Type cur_type);
int StmtList(struct Node *cur_node, hash_stack cur_stack, Type cur_type);
int Stmt(struct Node *cur_node, hash_stack cur_stack, Type cur_type);
int DefList(struct Node *cur_node, hash_stack cur_stack);
int Def(struct Node *cur_node, hash_stack cur_stack);
int DecList(struct Node *cur_node, hash_stack cur_stack, Type cur_type);
int Dec(struct Node *cur_node, hash_stack cur_stack, Type cur_type);
Type Exp(struct Node *cur_node);
int Arg(struct Node *cur_node, FieldList paras);
int FunDec(struct Node *cur_node, const int is_define, const Type cur_type, hash_stack cur_stack);
FieldList VarList(struct Node *cur_node, hash_stack cur_stack);
FieldList ParamDec(struct Node *cur_node);
Type Specifier(struct Node *cur_node);
FieldList Def_struct(struct Node *cur_node, char *struct_name);
FieldList Dec_struct(struct Node *cur_node, Type cur_type);
FieldList VarDec(struct Node *cur_node, Type cur_type);
int ExtDecList(struct Node *cur_node, Type cur_type);
FieldList DefList_struct(struct Node *cur_node,char* name_ofStruct);
Type StructSpecifier(struct Node *cur_node);
struct Node* getchild(struct Node* cur,int depth);
ST_node new_STnode(int kind, Type type, char *name, int is_define, int depth);
#endif