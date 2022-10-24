#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <stdio.h>
#include <stdlib.h>
//因为文件结构的问题，syntax.y不能include，建议单独将数据结构摘出来
//#include "syntax.y"
#include <string.h>
#include <assert.h>
typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
typedef struct ST_node_* ST_node;
typedef struct hash_stack_* hash_stack;
typedef struct func_list_* func_list;

hash_stack ST_init();
void print_error(int err_type, int err_col, char *message);
ST_node init_symbol(Type type, char *name, int is_define, int depth);
void insert_symbol(ST_node my_node, hash_stack domain);
ST_node find_symbol(char *name, int depth);
ST_node find_symbol_dec(char *name, int depth); //定义的时候只找同一层作用域的，即使外层也有也可定义
void free_node(ST_node del);
hash_stack find_domain(int depth);
void delete_domain_nodes(hash_stack domain);
hash_stack enter_domain();
void exit_domain();
void add_func(char *name, int func_lineno);
void check_func();
int insert_struct(Type type, char *name);
ST_node find_struct(char *name);
int type_eq(Type A, Type B);
int strong_array_check(Type A, Type B);
unsigned int hash_pjw(char *name);

struct FieldList_
{
    char *name;     //域的名字;
    Type type;      //域的类型;
    FieldList tail; //下一个域
};

struct Type_
{
    enum
    {
        BASIC = 0,
        ARRAY,
        STRUCTURE,
        FUNCTION
    } kind;
    union
    {
        //基本类型
        int basic; //设定0为int, 1为float
        //结构体类型的信息是一个链表
        struct
        {
            FieldList structure;
            char *name;
        } my_struct;
        struct
        {
            Type ret_para;
            int para_num;
            FieldList paras;
        } function;
        //数组类型信息包括元素类型与数组大小构成
        struct
        {
            Type elem;
            int size;
        } array;
    } u;
};

//符号表 symbol table node
struct ST_node_
{
    //加点东西
    enum
    {
        VARIABLE = 0,
        STRUCT_NAME = 1,
        FUNCTION_NAME = 2
    } kind;
    //定义or声明(0/1)
    int is_define;
    char *name;
    //深度，作用与局部变量和全局变量
    int depth;
    Type type;
    ST_node hash_next; //在hash中同一个值的下一个; -- 实验指导中图一的实线
    //控制域链表
    ST_node ctrl_next; //用来控制作用域的,主要是退出compst等的时候删除相应的元素,初始化为NULL;-- 也就是实验指导中图一的虚线
};

struct hash_stack_
{
    ST_node head;
    hash_stack next; //用来构建domain的栈，hash_table用不到这个属性
};

struct func_list_
{
    char *name;
    int fun_lineno;
    func_list next;
};
#endif