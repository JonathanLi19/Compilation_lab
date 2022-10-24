#include "symbols_hashtable.h"
//默认选取0x3fff
#define SYMBOL_LEN 0x3fff

struct hash_stack_ global_head[SYMBOL_LEN] = {NULL};
struct hash_stack_ struct_head[SYMBOL_LEN] = {NULL};
extern hash_stack Domain_head; //作用域控制链表;
extern int depth_;
func_list func_head = NULL; //函数定义链表,最后遍历检查;
//初始域
hash_stack ST_init()
{
    hash_stack domain_head = malloc(sizeof(struct hash_stack_));
    domain_head->next = NULL;
    domain_head->head = NULL;
    return domain_head;
}

//当想插入节点时，先创建该节点（若想创建空节点建议全部化为0或null）
ST_node init_symbol(Type type, char *name, int is_define, int depth)
{
    ST_node my_node = (ST_node)malloc(sizeof(struct ST_node_));
    my_node->type = type;
    strcpy(my_node->name, name);
    my_node->depth = depth;
    my_node->is_define = is_define;
    return my_node;
}

//插入节点
void insert_symbol(ST_node my_node, hash_stack domain)
{
    int idx = hash_pjw(my_node->name);

    if (domain == NULL || my_node->hash_next != NULL || my_node->ctrl_next != NULL)
        printf("Error!");
    else
    {
        //插入局部符号链表
        ST_node cur = domain->head;
        if (cur == NULL)
            domain->head = my_node;
        else
        {
            /*while (cur->ctrl_next != NULL)
                cur = cur->ctrl_next;
            cur->ctrl_next = my_node;*/
            my_node->ctrl_next = cur;
            domain->head = my_node;
        }
        //插入全局符号表
        ST_node cur1 = global_head[idx].head;
        /*if (cur1 != NULL)
            my_node->hash_next = cur1;
        global_head[idx].head = my_node;*/
        if (cur1 == NULL)
            global_head[idx].head = my_node;
        else
        {
            my_node->hash_next = cur1;
            global_head[idx].head = my_node;
        }
    }
    return;
}

//返回查询结果，只需要两个参数，变量名和变量位置
ST_node find_symbol(char *name, int depth)
{
    int idx = hash_pjw(name);
    //我们这里通过返回一个空节点的方式表明我们无法在符号表中找到该变量名。
    ST_node cur = global_head[idx].head;
    ST_node ret_node = NULL;
    //遍历哈希值为该值的链表，若hash不到则直接返回NULL。
    int max_depth = 0;
    while (cur)
    {
        //目前的深度应该深于查找节点的深度, 我们只需选择最深的那一个
        if (strcmp(cur->name, name) == 0 && depth >= cur->depth && cur->depth >= max_depth)
        {
            ret_node = cur;
            max_depth = cur->depth;
        }
        cur = cur->hash_next;
    }
    return ret_node;
}

//定义的时候只找同一层作用域的，即使外层也有也可定义。 但是别忘了还可以找合适的depth中定义的结构体的名字（这个不一定同层！)
ST_node find_symbol_dec(char *name, int depth)
{
    if(find_struct(name) != NULL)
        return find_struct(name);
    int idx = hash_pjw(name);
    //我们这里通过返回一个空节点的方式表明我们无法在符号表中找到该变量名。
    ST_node cur = global_head[idx].head;
    ST_node ret_node = NULL;
    //遍历哈希值为该值的链表，若hash不到则直接返回NULL。
    int max_depth = 0;
    while (cur)
    {
        if (strcmp(cur->name, name) == 0 && depth == cur->depth)
        {
            ret_node = cur;
            break;
        }
        cur = cur->hash_next;
    }
    return ret_node;
}

//释放节点
void free_node(ST_node del)
{
    if(del == NULL)
        return;
    assert(del != NULL);
    if(del->type != NULL)
    {
        free(del->type);
        del->type = NULL;
    }
    if(del->ctrl_next != NULL)
    {
        //free(del->ctrl_next);
        del->ctrl_next = NULL;
        
    }
    if(del->hash_next != NULL)
    {
        //free(del->hash_next);
        del->hash_next = NULL;
    }

    free(del);
    del = NULL;

    return;
}

//由节点深度寻找对应局域符号表头
hash_stack find_domain(int depth)
{
    hash_stack domain_iter = Domain_head;
    for (int i = depth_; i > depth; i--)
        domain_iter = domain_iter->next;
    return domain_iter;
}

//删除节点
void delete_domain_nodes(hash_stack domain)
{
    while(domain->head != NULL)
    {
        ST_node node_del = domain->head;
        int idx = hash_pjw(node_del->name);
        
        if(node_del == global_head[idx].head)
        {
            global_head[idx].head = global_head[idx].head->hash_next;
        }
        else
        {
            ST_node HT_iter = global_head[idx].head;
            while (HT_iter->hash_next != node_del)
                HT_iter = HT_iter->hash_next;

            HT_iter->hash_next = node_del->hash_next;
        }
        domain->head = node_del->ctrl_next;
        //这里如果有int i,j;出现那么i和j的type指针是指向同一块内存的，会free两次
        //printf("delete %s in depth: %d\n", node_del->name, node_del->depth);
        free_node(node_del);
    }
    if(domain != NULL)
    {
        free(domain);
        domain = NULL;
    }
    return;
}

//进入域之前进行调用，将新产生的domain加到Domain_head的链表头
hash_stack enter_domain()
{
    //printf("enter\n");
    hash_stack ret = malloc(sizeof(struct hash_stack_));
    ret->head = NULL;
    ret->next = Domain_head;
    Domain_head = ret;
    return ret;
}
//离开域时调用,先删除Domain_head中的第一项，然后进入hash_table删除对应的一系列node
void exit_domain()
{
    //printf("exit\n");
    hash_stack domain_del = Domain_head;
    if (domain_del == NULL)
    {
        printf("Error, Domain_head not exist!");
        return;
    }
    Domain_head = Domain_head->next;
    //进入hash_table删除对应的一系列node
    delete_domain_nodes(domain_del);
    return;
}
//在函数表中添加函数名和函数位置
void add_func(char *name, int func_lineno)
{
    func_list cur = func_head;
    if (cur == NULL)
    {
        cur = (func_list)malloc(sizeof(struct func_list_));
        func_head = cur;
    }
    else
    {
        while (cur->next != NULL)
            cur = cur->next;
        cur->next = (func_list)malloc(sizeof(struct func_list_));
        cur = cur->next;
    }
    cur->name = name;
    cur->fun_lineno = func_lineno;
    cur->next = NULL;
    return;
}

//完成后对于函数进行检查，看是否有声明却没有定义
void check_func()
{
    func_list cur = func_head;
    while (cur != NULL)
    {
        char *name = cur->name;
        ST_node ret_func = find_symbol(name, 0);
        if (ret_func->is_define != 1)
        {
            print_error(18, cur->fun_lineno, cur->name);
        }
        cur = cur->next;
    }
    return;
}

//向结构体符号表中插入符号
int insert_struct(Type type, char *name)
{
    int idx = hash_pjw(name);
    if (struct_head[idx].head == NULL)
    {
        ST_node cur = malloc(sizeof(struct ST_node_));
        cur->type = type;
        cur->name = name;
        cur->hash_next = NULL;
        struct_head[idx].head = cur;
    }
    else
    {
        //插入，由于插入之前已经find_struct过了，所以不需要再检查是否有域名重定义了
        ST_node cur = malloc(sizeof(struct ST_node_));
        cur->type = type;
        cur->hash_next = struct_head[idx].head;
        cur->name = name;
        struct_head[idx].head = cur;
    }
    return 0;
}

//返回的结构体，只需要一个参数，即结构体名
ST_node find_struct(char *name)
{
    int idx = hash_pjw(name);
    ST_node cur = struct_head[idx].head;
    ST_node ret_node = NULL;
    //遍历哈希值为该值的链表，若hash不到则直接返回NULL。
    while (cur)
    {
        if (strcmp(cur->name, name) == 0)
        {
            ret_node = cur;
            break;
        }
        else
            cur = cur->hash_next;
    }
    return ret_node;
}

//检测两个类型是否相等
int type_eq(Type A, Type B)
{
    //为同一指针
    if (A == B)
        return 1;
    else
    {
        if (A == NULL || B == NULL)
            //A or B is null
            return 0;
        else if (A->kind != B->kind)
            //not match
            return 0;
        else
        {
            switch (A->kind)
            {
            case BASIC:
            {
                //数字，这里选择直接返回两者的值相等
                return (A->u.basic == B->u.basic);
            }
            case ARRAY:
            {
                //数组，这里检测弱相等，即当两者的维度相等而且基类型相等时，我们就认为数组相等。
                int dim_A = 0, dim_B = 0;
                int kind_A = 0, kind_B = 0;
                Type cur_A = A, cur_B = B;
                while (cur_A != NULL)
                {
                    cur_A = cur_A->u.array.elem;
                    dim_A += 1;
                    if (cur_A->kind != ARRAY)
                    {
                        kind_A = cur_A->kind;
                        break;
                    }
                }
                while (cur_B != NULL)
                {
                    cur_B = cur_B->u.array.elem;
                    dim_B += 1;
                    if (cur_B->kind != ARRAY)
                    {
                        kind_B = cur_B->kind;
                        break;
                    }
                }
                return (dim_A == dim_B) && (kind_A == kind_B);
            }
            case STRUCTURE:
            {
                //检测结构体相等
                FieldList A_field = A->u.my_struct.structure;
                FieldList B_field = B->u.my_struct.structure;
                while (A_field != NULL && B_field != NULL)
                {
                    if (A_field->type->kind != B_field->type->kind)
                        return 0;
                    else
                    {
                        //若为数组则检查数组强相等，若不为数组则检查类型相等
                        if (A_field->type->kind == ARRAY)
                        {
                            if (strong_array_check(A_field->type, B_field->type) == 0)
                                return 0;
                        }
                        else
                        {
                            if (type_eq(A_field->type, B_field->type) == 0)
                                return 0;
                        }
                    }
                    A_field = A_field->tail;
                    B_field = B_field->tail;
                }
                //若两者不等长，则返回0，反之返回1
                if (A_field != NULL || B_field != NULL)
                    return 0;
                else
                    return 1;
            }
            case FUNCTION:
            {
                FieldList A_paras = A->u.function.paras;
                FieldList B_paras = B->u.function.paras;
                //比较函数的参数和返回值类型是否相等
                if ((A->u.function.para_num != B->u.function.para_num) ||
                    type_eq(A->u.function.ret_para, B->u.function.ret_para) == 0)
                    return 0;
                else
                {
                    while (A_paras != NULL && B_paras != NULL)
                    {
                        //逐个比较函数的参数类型是否相等
                        if (type_eq(A_paras->type, B_paras->type) == 0)
                            return 0;
                        else
                        {
                            A_paras = A_paras->tail;
                            B_paras = B_paras->tail;
                        }
                    }
                }
                if (A_paras != NULL || B_paras != NULL)
                    return 0;
                else
                    return 1;
            }
            default:
                printf("No such type!\n");
                break;
            }
        }
    }
}

//检测两个数组是否强等价。
int strong_array_check(Type A, Type B)
{
    int ret_val = 0;
    if ((A->u.array.size != B->u.array.size) ||
        (A->u.array.elem->kind != B->u.array.elem->kind))
        return 0;

    if (A->u.array.elem->kind == ARRAY)
        ret_val = strong_array_check(A->u.array.elem, B->u.array.elem);
    else
        ret_val = type_eq(A->u.array.elem, B->u.array.elem);
    return ret_val;
}

//散列函数（老师给的）
unsigned int hash_pjw(char *name)
{
    unsigned int val = 0, i;
    for (; *name; ++name)
    {
        val = (val << 2) + *name;
        if (i = val & ~(SYMBOL_LEN))
            val = (val ^ (i >> 12)) & SYMBOL_LEN;
    }
    return val;
}

void print_error(int err_type, int err_col, char *message)
{
    printf("Error type %d at Line %d: ", err_type, err_col);
    switch (err_type)
    {
    case (1):
    {
        printf("Undefined variable \"%s\".\n", message);
        break;
    }
    case (2):
    {
        printf("Undefined function \"%s\".\n", message);
        break;
    }
    case (3):
    {
        printf("Redefined variable \"%s\".\n", message);
        break;
    }
    case (4):
    {
        printf("Redefined function \"%s\".\n", message);
        break;
    }
    case (5):
    {
        printf("Type mismatched for assignment.\n");
        break;
    }
    case (6):
    {
        printf("The left-hand side of an assignment must be a variable.\n");
        break;
    }
    case (7):
    {
        printf("Type mismatched for operands.\n");
        break;
    }
    case (8):
    {
        printf("Type mismatched for return.\n");
        break;
    }
    case (9):
    {
        printf("Function is not applicable for arguments.\n");
        break;
    }
    case (10):
    {
        printf("This is not an array.\n");
        break;
    }
    case (11):
    {
        printf("\"%s\" is not a function.\n", message);
        break;
    }
    case (12):
    {
        printf("This is not an integer.\n");
        break;
    }
    case (13):
    {
        printf("Illegal use of \".\".\n");
        break;
    }
    case (14):
    {
        printf("Non-existent field \"%s\".\n", message);
        break;
    }
    case (15):
    {
        printf("Redefined or initialized field \"%s\" in struct.\n", message);
        break;
    }
    case (16):
    {
        printf("Duplicated name \"%s\".\n", message);
        break;
    }
    case (17):
    {
        printf("Undefined structure \"%s\".\n", message);
        break;
    }
    case (18):
    {
        printf("Undefined function \"%s\".\n", message);
        break;
    }
    case (19):
    {
        printf("Inconsistent declaration of function \"%s\".\n", message);
        break;
    }
    default:
    {
        printf("Other Mistakes, content is :%s\n", message);
        break;
    }
    }
}
