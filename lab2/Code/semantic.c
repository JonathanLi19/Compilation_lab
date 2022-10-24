#include "semantic.h"
//创建
ST_node new_STnode(int kind, Type type, char *name, int is_define, int depth)
{

    ST_node node = (ST_node)malloc(sizeof(struct ST_node_));

    node->kind = kind;
    node->is_define = is_define;
    node->name = name;

    node->depth = depth;
    node->type = type;
    node->hash_next = NULL;
    node->ctrl_next = NULL;
    return node;
}

struct Node* getchild(struct Node* cur,int depth)
{
    if(cur == NULL)
        return NULL;
	struct Node*temp = cur;
	temp = temp->child;
	if(depth==0)
    {
		return temp;
	}

	for(int i=1;i<=depth;i++)
    {
        if(temp == NULL)
            return NULL;
		temp = temp->next_sib;
	}
	return temp;

}

int depth_ = 0;
int num_struct_without_name = 0;
hash_stack Domain_head = NULL;//作用域控制栈
int Program(struct Node *cur_node)
{
    //Program -> ExfDefList
    Domain_head = ST_init();
    //assert(find_domain(0) == Domain_head);
    ExtDefList(getchild(cur_node, 0));
    check_func();
    return 0;
}

int ExtDefList(struct Node *cur_node)
{
    //ExfDefList -> ExfDef ExfDefList
    //| (empty)

    ExtDef(getchild(cur_node, 0));

    if (getchild(cur_node, 1) != NULL)
        ExtDefList(getchild(cur_node, 1));
    return 0;
}

int ExtDef(struct Node *cur_node)
{
    // ExtDef -> Specifier ExtDecList SEMI
    // | Specifier SEMI
    // | Specifier FunDec CompSt
    // | Specifier FunDec SEMI

    Type tmp_type = NULL;
    struct Node *tmp_node1 = getchild(cur_node, 1);
    struct Node *tmp_node2 = getchild(cur_node, 2);
    if (getchild(cur_node, 0) != NULL)
        tmp_type = Specifier(getchild(cur_node, 0));
    if(tmp_type == NULL)
        return NULL;
    if (tmp_node2 != NULL)
    {
        if (tmp_node1 != NULL && strcmp(tmp_node1->name, "ExtDecList") == 0)
        {
            int result = ExtDecList(tmp_node1, tmp_type);
        }
        else
        {
            struct Node *FunDec_node = tmp_node1;
            if (strcmp(tmp_node2->name, "SEMI") == 0)
            {
                //hash_stack new_hashstack = enter_domain();
                FunDec(FunDec_node, 0, tmp_type, Domain_head);
                //exit_domain();
            }
            else
            {
                int return_depth = FunDec(FunDec_node, 1, tmp_type, Domain_head);
                if(return_depth == -1) // 形参没有enter domain
                {
                    hash_stack new_hashstack = enter_domain();
                    depth_++;
                    struct Node *CompSt_node = tmp_node2;
                    CompSt(CompSt_node, new_hashstack, tmp_type);
                    exit_domain();
                    depth_--;
                    //assert(find_symbol("i", 0) != NULL);
                }
                else
                {
                    depth_ = return_depth;
                    struct Node *CompSt_node = tmp_node2;
                    CompSt(CompSt_node, find_domain(depth_), tmp_type);
                    depth_--;
                    exit_domain();
                }
            }
        }
    }
    return 0;
}

int CompSt(struct Node *cur_node, hash_stack cur_stack, Type cur_type)
{
    // CompSt -> LC DefList StmtList RC
    // DefList -> Def DefList
    // | (empty)
    struct Node *tmp_node1 = getchild(cur_node, 1);

    if (strcmp(tmp_node1->name, "DefList") == 0)
    {
        DefList(tmp_node1, cur_stack);
        struct Node *StmtList_node = getchild(cur_node, 2);

        if (StmtList_node != NULL && strcmp(StmtList_node->name, "StmtList") == 0)
            StmtList(StmtList_node, cur_stack, cur_type);
    }
    else if (strcmp(tmp_node1->name, "StmtList") == 0)
    {
        StmtList(tmp_node1, cur_stack, cur_type);
    }
    return 0;
}

int StmtList(struct Node *cur_node, hash_stack cur_stack, Type cur_type)
{
    // StmtList -> Stmt StmtList
    // | 空
    if(getchild(cur_node, 0) == NULL)
        return 0;
    struct Node *tmp_node1 = getchild(cur_node, 1);
    Stmt(getchild(cur_node, 0), cur_stack, cur_type);
    if (tmp_node1 != NULL)
        StmtList(tmp_node1, cur_stack, cur_type);
}

int Stmt(struct Node *cur_node, hash_stack cur_stack, Type cur_type)
{
    // Stmt -> Exp SEMI
    // | CompSt
    // | RETURN Exp SEMI
    // | IF LP Exp RP Stmt
    // | IF LP Exp RP Stmt ELSE Stmt
    // | WHILE LP Exp RP Stmt

    struct Node *tmp_node0 = getchild(cur_node, 0);
    if(tmp_node0 == NULL)
        return 0;
    if (strcmp(tmp_node0->name, "Exp") == 0) // Stmt -> Exp SEMI
    {
        Type tmp_exptype = Exp(tmp_node0);
    }
    else if (strcmp(tmp_node0->name, "CompSt") == 0) // | CompSt
    {
        depth_++;
        hash_stack new_hashstack1 = enter_domain();
        CompSt(tmp_node0, new_hashstack1, cur_type);
        exit_domain();
        depth_--;
    }
    else if (strcmp(tmp_node0->name, "RETURN") == 0) // | RETURN Exp SEMI
    {
        struct Node *Exp_node1 = getchild(cur_node, 1);
        Type Return_type1 = Exp(Exp_node1);
        if (Return_type1 != NULL)
        {
            if (type_eq(cur_type, Return_type1) == 0) //返回类型不等，报错8
            {
                print_error(8, cur_node->line_num, NULL);
                return -1;
            }
        }
    }
    else if (strcmp(tmp_node0->name, "WHILE") == 0) // | WHILE LP Exp RP Stmt
    {
        struct Node *Exp_node2 = getchild(cur_node, 2);
        struct Node *Stmt_node4 = getchild(cur_node, 4);
        Type While_type = Exp(Exp_node2);
        if (While_type != NULL)
        {
            if (While_type->kind != BASIC || While_type->u.basic != 0)
            {
                printf("OK1\n");
                print_error(7, cur_node->line_num, NULL); //while条件非int，操作类型不匹配，报错7
            }
        }
        Stmt(Stmt_node4, cur_stack, cur_type);
    }
    else if (strcmp(tmp_node0->name, "IF") == 0)
    {
        // | IF LP Exp RP Stmt
        // | IF LP Exp RP Stmt ELSE Stmt
        struct Node *tmp_node2 = getchild(cur_node, 2);
        struct Node *tmp_node5 = getchild(cur_node, 5);
        Type If_type2 = Exp(tmp_node2);
        if (If_type2 != NULL)
        {
            if (If_type2->kind != BASIC || If_type2->u.basic != 0)
            {
                printf("OK2\n");
                print_error(7, cur_node->line_num, NULL); //if条件非int，类型不匹配，报错7
            }
        }
        if (tmp_node5 == NULL) // | IF LP Exp RP Stmt
        {
            Stmt(getchild(cur_node, 4), cur_stack, cur_type);
        }
        else // | IF LP Exp RP Stmt ELSE Stmt
        {
            Stmt(getchild(cur_node, 4), cur_stack, cur_type);
            Stmt(getchild(cur_node, 6), cur_stack, cur_type);
        }
    }
    return 0;
}

int DefList(struct Node *cur_node, hash_stack cur_stack)
{
    //DefList -> Def DefList
    // | (empty)
    if (getchild(cur_node, 0) != NULL)
    {
        Def(getchild(cur_node, 0), cur_stack);
        if (getchild(cur_node, 1) != NULL)
            DefList(getchild(cur_node, 1), cur_stack);
    }
    return 0;
}

int Def(struct Node *cur_node, hash_stack cur_stack)
{
    //	Def -> Specifier DecList SEMI

    Type Specifier_type = Specifier(getchild(cur_node, 0));
    if(Specifier_type == NULL)
        return 0;
    DecList(getchild(cur_node, 1), cur_stack, Specifier_type);
    return 0;
}

int DecList(struct Node *cur_node, hash_stack cur_stack, Type cur_type)
{
    // 	DecList -> Dec
    // | Dec COMMA DecList

    Dec(getchild(cur_node, 0), cur_stack, cur_type);

    if (getchild(cur_node, 2) != NULL)
        DecList(getchild(cur_node, 2), cur_stack, cur_type);

    return 0;
}

int Dec(struct Node *cur_node, hash_stack cur_stack, Type cur_type)
{
    // 	Dec -> VarDec
    // | VarDec ASSIGNOP Exp
    FieldList VarDec_field = VarDec(getchild(cur_node, 0), cur_type);
    if(VarDec_field == NULL)
        return 0;
    ST_node found = find_symbol_dec(VarDec_field->name, depth_);
    if (found != NULL) //符号已存在，重复定义，报错3
    {
        print_error(3, cur_node->line_num, VarDec_field->name);
        return 0;
    }
    if (getchild(cur_node, 1) == NULL) // Dec -> VarDec
    {
        ST_node insert_node = new_STnode(VARIABLE, VarDec_field->type, VarDec_field->name, 1, depth_);
        insert_symbol(insert_node, cur_stack); //插入符号
    }
    else // | VarDec ASSIGNOP Exp
    {
        Type exp_type = Exp(getchild(cur_node, 2));
        if (exp_type != NULL)
        {
            if (type_eq(VarDec_field->type, exp_type) == 0) //赋值号左右类型不匹配，报错5
            {
                print_error(5, cur_node->line_num, NULL);
                return 0;
            }
        }
        ST_node insert_node = new_STnode(VARIABLE, VarDec_field->type, VarDec_field->name, 1, depth_);
        insert_symbol(insert_node, cur_stack); //插入符号
    }
    return 0;
}

Type Exp(struct Node *cur_node)
{
    /*Exp -> Exp ASSIGNOP Exp
	| Exp AND Exp
	| Exp OR Exp
	| Exp RELOP Exp
 	| Exp PLUS Exp
	| Exp MINUS Exp
	| Exp STAR Exp
	| Exp DIV Exp
	| LP Exp RP
	| MINUS Exp
	| NOT Exp
	| ID LP Args RP
	| ID LP RP
	| Exp LB Exp RB
	| Exp DOT ID
	| ID
	| INT
	| FLOAT
	*/
    if (cur_node == NULL)
        return NULL;
    Type result = NULL;
    struct Node *tmp_node0 = getchild(cur_node, 0);
    if(tmp_node0 == NULL)
        return NULL;
    struct Node *tmp_node1 = getchild(cur_node, 1);
    //左值: ID,EXP DOT ID(结构体) Exp LB Exp RB (数组)

    if (strcmp(tmp_node0->name, "Exp") == 0 && tmp_node1 != NULL && strcmp(tmp_node1->name, "ASSIGNOP") == 0)//Exp -> Exp ASSIGNOP Exp
    {
        struct Node *tmp_node00 = getchild(tmp_node0, 0);
        struct Node *tmp_node01 = getchild(tmp_node0, 1);
        if (tmp_node01 == NULL && strcmp(tmp_node00->name,"ID") != 0)
        {
            // INT FLOAT
            print_error(6, cur_node->line_num, NULL);
            return NULL;
        }
        else if(tmp_node01 != NULL)
        {
            struct Node *tmp_node02 = getchild(tmp_node0, 2);
            if (tmp_node02 != NULL)
            {
                struct Node *tmp_node03 = getchild(tmp_node0, 3);
                if (tmp_node03 == NULL)
                {
                    if (!(strcmp(tmp_node00->name, "Exp") == 0 && strcmp(tmp_node01->name, "DOT") == 0 && strcmp(tmp_node02->name, "ID") == 0))
                    { // 不是左值: EXP DOT ID(结构体)，赋值号左边只有右值，报错6
                        print_error(6, cur_node->line_num, NULL);
                        return NULL;
                    }
                }
                else
                {
                    if (!(strcmp(tmp_node00->name, "Exp") == 0 && strcmp(tmp_node01->name, "LB") == 0 && strcmp(tmp_node02->name, "Exp") == 0 && strcmp(tmp_node03->name, "RB") == 0))
                    { // 不是左值: Exp LB Exp RB (数组)，赋值号左边只有右值，报错6
                        print_error(6, cur_node->line_num, NULL);
                        return NULL;
                    }
                }
            }
            else
            { //无二元左值满足条件，赋值号左边只有右值，报错6
                print_error(6, cur_node->line_num, NULL);
                return NULL;
            }
        }
        // 如果没有发生错误6
        struct Node *tmp_node2 = getchild(cur_node, 2);
        Type Exp1_type = Exp(tmp_node0);
        Type Exp2_type = Exp(tmp_node2);
        if(Exp1_type == NULL || Exp2_type == NULL)
            return NULL;
        else if(type_eq(Exp1_type, Exp2_type))
            return Exp1_type;
        else
        {
            print_error(5, tmp_node0->line_num, NULL);
            return NULL;
        }
    }
    if (tmp_node1 == NULL) //ID INT FLOAT
    {
        if (strcmp(tmp_node0->name, "ID") == 0)
        {
            ST_node found = find_symbol(tmp_node0->string_content, depth_);
            if (found != NULL && found->is_define == 1)
            { //找到定义
                result = found->type;
                return result;
            }
            else
            {
                print_error(1, cur_node->line_num, tmp_node0->string_content);
                return NULL;
            }
        }
        else if (strcmp(tmp_node0->name, "INT") == 0)
        {
            result = (Type)(malloc(sizeof(struct Type_)));
            result->kind = BASIC;
            result->u.basic = 0;
            return result;
        }
        else if (strcmp(tmp_node0->name, "FLOAT") == 0)
        {
            result = (Type)(malloc(sizeof(struct Type_)));
            result->kind = BASIC;
            result->u.basic = 1;
            return result;
        }
    }
    else
    {
        /*
        | Exp AND Exp
        | Exp OR Exp
        | Exp RELOP Exp
        | Exp PLUS Exp
        | Exp MINUS Exp
        | Exp STAR Exp
        | Exp DIV Exp

        | Exp LB Exp RB
	    | Exp DOT ID
        */

        // | LP Exp RP
        // | MINUS Exp
        // | NOT Exp

        // | ID LP Args RP
        // | ID LP RP
        struct Node *tmp_nodee2 = getchild(cur_node, 2);
        if (tmp_nodee2 != NULL)
        {
            struct Node *tmp_nodee3 = getchild(cur_node, 3);
            if (tmp_nodee3 == NULL && strcmp(tmp_nodee2->name, "Exp") == 0)
            {
                /*
                | Exp AND Exp
                | Exp OR Exp
                | Exp RELOP Exp
                | Exp PLUS Exp
                | Exp MINUS Exp
                | Exp STAR Exp
                | Exp DIV Exp
                */
                struct Node *exp_1 = tmp_node0;
                struct Node *exp_2 = tmp_nodee2;
                Type exp1type = Exp(exp_1);
                Type exp2type = Exp(exp_2);
                if (exp1type != NULL && exp2type != NULL)
                {
                    int exp_eqornot = type_eq(exp1type, exp2type);
                    if (exp_eqornot == 0) //操作数类型不匹配
                    {
                        printf("OOps!\n");
                        print_error(7, cur_node->line_num, NULL);
                        return NULL;
                    }
                    else
                    {
                        if(strcmp(getchild(cur_node, 1)->name, "RELOP") == 0) //如果是判断条件就返回整数type
                        {
                            result = (Type)(malloc(sizeof(struct Type_)));
                            result->kind = BASIC;
                            result->u.basic = 0;
                        }
                        else                       
                            result = exp1type;
                        return result;
                    }
                }
                else
                    return NULL;
            }
            else if(tmp_nodee3 == NULL && strcmp(tmp_node0->name, "Exp") == 0 && strcmp(tmp_node1->name, "DOT") == 0 && strcmp(tmp_nodee2->name, "ID") == 0)
            {
                //Exp DOT ID
                Type exp_nodetype = Exp(tmp_node0);
                if (exp_nodetype != NULL)
                {
                    if (exp_nodetype->kind != STRUCTURE)
                    { //报错13
                        print_error(13, cur_node->line_num, NULL);
                        return NULL;
                    }
                    else
                    {
                        char *node2_name = tmp_nodee2->string_content;
                        char *field_name = (char *)(malloc(sizeof(char) * (1 + strlen(node2_name) + strlen(exp_nodetype->u.my_struct.name))));
                        strcpy(field_name, node2_name);
                        strcat(field_name, exp_nodetype->u.my_struct.name);
                        if (find_struct(field_name) != NULL)
                        {
                            result = find_struct(field_name)->type;
                            return result;
                        }
                        else
                        {
                            print_error(14, cur_node->line_num, node2_name);
                            return NULL;
                        }
                    }
                }
                else
                    return NULL;
            }
            else if(tmp_nodee3 != NULL && strcmp(tmp_node0->name, "Exp") == 0 && strcmp(tmp_node1->name, "LB") == 0 && strcmp(tmp_nodee2->name, "Exp") == 0)
            {
                //Exp LB Exp RB
                Type Exp_node0 = Exp(tmp_node0), Exp_node2 = Exp(tmp_nodee2);
                if (Exp_node0 == NULL || Exp_node2 == NULL)
                    return NULL;
                if (Exp_node0->kind != ARRAY)
                {
                    print_error(10, cur_node->line_num, NULL);
                    return NULL;
                }
                else
                {
                    if (Exp_node2->kind != BASIC || Exp_node2->u.basic != 0)
                    {
                        print_error(12, cur_node->line_num, NULL);
                        return NULL;
                    }
                }
                result = Exp_node0->u.array.elem;
                return result;
            }
        }
        if (strcmp(tmp_node0->name, "LP") == 0 || strcmp(tmp_node0->name, "MINUS") == 0 || strcmp(tmp_node0->name, "NOT") == 0)
        {
            struct Node *expnode = tmp_node1;
            Type exp1type = Exp(expnode);
            result = exp1type;
            return result;
        }
        if (strcmp(tmp_node0->name, "ID") == 0)
        {
            char *name_function = tmp_node0->string_content;
            ST_node found = find_symbol(name_function, depth_);
            if (found == NULL || found->is_define == 0)
            { //函数在调用时未经定义
                print_error(2, cur_node->line_num, name_function);
                return NULL;
            }
            Type tmp_findtype = found->type;
            if(tmp_findtype == NULL)
                return NULL;
            if (found->type->kind != FUNCTION)
            { //报错11
                print_error(11, cur_node->line_num, name_function);
                return NULL;
            }
            
            result = tmp_findtype->u.function.ret_para;
            
            if (strcmp(tmp_nodee2->name, "Args") == 0)
            {
                int args_num = Arg(tmp_nodee2, tmp_findtype->u.function.paras);
                if (args_num == -1)
                    return NULL;
                else if(args_num != tmp_findtype->u.function.para_num)
                {
                    print_error(9, cur_node->line_num, NULL);
                    return NULL;
                }
                return result;
            }
            else
            {
                if (tmp_findtype->u.function.paras != NULL)
                {
                    print_error(9, cur_node->line_num, NULL);
                    return NULL;
                }
                else
                    return result;
            }
        }
    }
    return NULL;
}

int Arg(struct Node *cur_node, FieldList paras)
{
    /*Args -> Exp COMMA Args
    | Exp;
    */
    int args_num = 0;
    if (paras == NULL)
    { //参数NULL，报错9
        print_error(9, cur_node->line_num, NULL);
        return -1;
    }
    if (paras->type == NULL) //参数类型NULL，报错9
    {   
        print_error(9, cur_node->line_num, NULL);
        return -1;
    }
    Type exp_type = Exp(getchild(cur_node, 0));
    
    if(exp_type == NULL)
        return -1;
    int result = type_eq(exp_type, paras->type);
    if (result == 0)
    { //参数类型不匹配，报错9

        print_error(9, cur_node->line_num, NULL);
        return -1;
    }
    args_num++;

    if (getchild(cur_node, 1) != NULL)
    {
        if (paras->tail == NULL)
        { //参数数目不匹配，报错9
            print_error(9, cur_node->line_num, NULL);
            return -1;
        }
        else
        {
            int num = Arg(getchild(cur_node, 2), paras->tail);
            if(num == -1)
                return -1;
            else
                args_num += num;
        }
    }
    return args_num;
}

int FunDec(struct Node *cur_node, const int is_define, const Type cur_type, hash_stack cur_stack)
{
    //FunDec -> ID LP VarList RP
    //| ID LP RP
    struct Node *tmp_node0 = getchild(cur_node, 0);
    if(tmp_node0 == NULL)
        return -1;
    char *name_function = tmp_node0->string_content;

    ST_node ret_node = find_symbol(name_function, depth_);

    struct Node *tmp_node2 = getchild(cur_node, 2);
    Type newfunc_type = (Type)(malloc(sizeof(struct Type_)));

    int varlist_enter_depth = -1; //形参是否已经enter过domain，如果没有是-1，否则是对应的depth_
    if (strcmp(tmp_node2->name, "VarList") != 0)
    {
        newfunc_type->u.function.para_num = 0;
        newfunc_type->u.function.paras = NULL;
    }
    else
    {
        FieldList new_field2 = NULL;
        struct Node *Varlistnode = tmp_node2;
        hash_stack newdomain = enter_domain();
        depth_++;
        varlist_enter_depth = depth_;
        new_field2 = VarList(Varlistnode, newdomain);
        depth_--;

        int cnt = 0;
        FieldList temp = new_field2;
        while (temp != NULL)
        {
            cnt++;
            temp = temp->tail;
        }
        newfunc_type->u.function.para_num = cnt;
        newfunc_type->u.function.paras = new_field2;

    }
    newfunc_type->kind = FUNCTION;
    newfunc_type->u.function.ret_para = cur_type;
    if (ret_node != NULL)
    {
        if (is_define == 1)
        {
            if (ret_node->is_define == 1)
            { //报错4,重复定义
                print_error(4, cur_node->line_num, tmp_node0->string_content);
            }
            else if (type_eq(ret_node->type, newfunc_type) == 0)
            {
                print_error(19, cur_node->line_num, tmp_node0->string_content);
            }
            else
            {
                ret_node->is_define = 1;
            }
        }
        else
        {
            if (type_eq(ret_node->type, newfunc_type) == 0)
            {
                print_error(19, cur_node->line_num, tmp_node0->string_content);
            }
        }
    }
    else
    {
        //printf("insert %s to %d\n",name_function,depth_);
        insert_symbol(new_STnode(FUNCTION_NAME, newfunc_type, name_function, is_define, depth_), cur_stack);
        if (is_define == 0)//当只有声明的时候，加入一个链表中，最后检查这个声明是否有对应的函数定义
        {
            add_func(name_function, cur_node->line_num);
        }
    }
    return varlist_enter_depth;
}

FieldList VarList(struct Node *cur_node, hash_stack cur_stack)
{
    //  VarList -> ParamDec COMMA VarList
    //| ParamDec;
    if(cur_node == NULL)
        return NULL;
    FieldList result = ParamDec(getchild(cur_node, 0));

    if(result == NULL)
        return NULL;

    ST_node found = find_symbol(result->name, depth_);
    if (found != NULL && found->kind == STRUCT_NAME) //变量与已定义结构体重复，报错3
    {
        print_error(3, cur_node->line_num, result->name);
        return NULL;
    }
    //为了防止形参的type在exit_domain的时候被删除，这里传的是备份，result->type不会被删除，这样后来调用函数还可以访问到
    Type copy_type = NULL;
    if(result->type == NULL)
        copy_type = NULL;
    else
    {
        copy_type = (Type)(malloc(sizeof(struct Type_)));
        copy_type->kind = result->type->kind;
        copy_type->u = result->type->u;
    }
    ST_node newvar_node = new_STnode(result->type->kind, copy_type, result->name, 1, depth_);
    insert_symbol(newvar_node, cur_stack);

    FieldList res_field = result;
    struct Node *varlist_node = cur_node; //第一个VarList，下面是随后，直到COMMA为NULL
    while (getchild(varlist_node, 1) != NULL)
    {
        varlist_node = getchild(varlist_node, 2);
        FieldList field = VarList(varlist_node, cur_stack);
        res_field->tail = field; //插入局部作用域
        res_field = res_field->tail;
    }
    res_field->tail = NULL;
    
    return result;
}

FieldList ParamDec(struct Node *cur_node)
{
    //ParamDec -> Specifier VarDec
    Type type = Specifier(getchild(cur_node, 0));
    FieldList result = VarDec(getchild(cur_node, 1), type);
    return result;
}

Type Specifier(struct Node *cur_node)
{
    /*
	Specifier -> TYPE
	| StructSpecifier
	StructSpecifier -> STRUCT OptTag LC DefList RC
	| STRUCT Tag
	OptTag -> ID
	| (empty)
	Tag -> ID
	*/
    Type type = (Type)malloc(sizeof(struct Type_));
    struct Node *tmp_node0 = getchild(cur_node, 0);
    if (strcmp(tmp_node0->name, "TYPE") == 0) //TYPE
    {
        type->kind = BASIC;
        if (strcmp(tmp_node0->string_content, "int") == 0)
            type->u.basic = 0; //int
        else if (strcmp(tmp_node0->string_content, "float") == 0)
            type->u.basic = 1; //float
    }
    else if (strcmp(tmp_node0->name, "StructSpecifier") == 0) //StructSpecifier
    {

        type = StructSpecifier(tmp_node0);
    }

    return type;
}

FieldList DefList_struct(struct Node *cur_node,char* name_ofStruct)
{
    //DefList -> Def DefList
    // | (empty)
    //printf("Deflist_struct\n");
    struct Node *tmp_defnode0 = getchild(cur_node, 0);
    if(tmp_defnode0 == NULL)
        return NULL;
    FieldList result = Def_struct(tmp_defnode0, name_ofStruct);
    if(result == NULL)
        return NULL;
    result->tail = DefList_struct(getchild(cur_node, 1), name_ofStruct);

    return result;
}

Type StructSpecifier(struct Node *cur_node)
{
    /*
    StructSpecifier -> STRUCT OptTag LC DefList RC
	| STRUCT Tag
    OptTag -> ID
	| (empty)
	Tag -> ID
    */
    Type type = (Type)malloc(sizeof(struct Type_));
    type->kind = STRUCTURE;
    struct Node *tmp_node01 = getchild(cur_node, 1);
    if (strcmp(tmp_node01->name, "OptTag") == 0) //STRUCT OptTag LC DefList RC
    {
        struct Node *ID_node = getchild(tmp_node01, 0);
        if (strcmp(ID_node->name, "ID") == 0) //OptTag -> ID
        {
            char *name_ofStruct = ID_node->string_content;

            if (find_symbol(name_ofStruct, depth_) != NULL)
            { //结构体重复，报错16
                print_error(16, ID_node->line_num, name_ofStruct);
                return NULL;
            }
            else
            {
                type->u.my_struct.name = (char *)malloc(sizeof(char) * 32);
                strcpy(type->u.my_struct.name, name_ofStruct); //赋值struct name;用于返回给上层;同时struct name作为hash值将填到struct hash表里面;
                struct Node *Deflist_node = getchild(cur_node, 3);
                if (strcmp(Deflist_node->name, "DefList") != 0)
                {
                    type->u.my_struct.structure = NULL;
                    return NULL;
                }
                else
                {
                    //DefList -> Def DefList
                    // | (empty)
                    depth_++;
                    FieldList result = DefList_struct(Deflist_node, name_ofStruct);
                    depth_--;
                    if(result == NULL)
                        return NULL;
                    type->u.my_struct.structure = result;
                }
            }
            insert_symbol(new_STnode(STRUCT_NAME, type, name_ofStruct, 1, depth_), find_domain(depth_));
            return type;
        }
    }
    else if (strcmp(tmp_node01->name, "Tag") == 0) //STRUCT Tag
    {
        struct Node *ID = getchild(tmp_node01, 0);
        char *id_name = ID->string_content;

        if (find_symbol(id_name,depth_) == NULL)
        { //结构体未定义，报错17
            print_error(17, ID->line_num, id_name);
            return NULL;
        }
        else if (find_symbol(id_name,depth_)->type->kind != STRUCTURE)
        {//报错17
            print_error(17, ID->line_num, id_name);
            return NULL;
        }
        else
        {
            return find_symbol(id_name,depth_)->type;
        }
    }
    else if (strcmp(tmp_node01->name, "LC") == 0) //OptTag -> (empty)
    {
        num_struct_without_name += 1;
        char *name_ofStruct = (char *)malloc(32 + 2);
        sprintf(name_ofStruct, "no_name_struct %d", num_struct_without_name);
        type->u.my_struct.name = (char *)malloc(sizeof(char) * 32);
        strcpy(type->u.my_struct.name, name_ofStruct);

        struct Node *DefList_node = getchild(cur_node, 2);
        if(DefList_node == NULL)
            return NULL;
        if (strcmp(DefList_node->name, "DefList") != 0)
            type->u.my_struct.structure = NULL;
        else
        {
            FieldList result = DefList_struct(DefList_node, name_ofStruct);
            type->u.my_struct.structure = result;
        }
        return type;
    }
}

FieldList Def_struct(struct Node *cur_node, char *struct_name)
{
    /*
	Def -> Specifier DecList SEMI
	DecList -> Dec
		| Dec COMMA DecList
	*/
    //printf("Def_struct\n");
    struct Node *DecListNode = getchild(cur_node, 1);
    Type nowtype = Specifier(getchild(cur_node, 0));
    if(nowtype == NULL)
        return NULL;
    FieldList res_field = NULL, tmp_field = NULL;

    while (DecListNode != NULL)
    {
        struct Node *Dec_node = getchild(DecListNode, 0);
        FieldList Dec_field = Dec_struct(Dec_node, nowtype);
        if(Dec_field == NULL)
            return NULL;
        char *Dec_name = (char *)malloc(1 + strlen(struct_name) + strlen(Dec_field->name));
        strcpy(Dec_name, Dec_field->name);
        strcat(Dec_name, struct_name);

        if (find_struct(Dec_name) != NULL) //结构体中域名重复定义
        {
            print_error(15, Dec_node->line_num, Dec_field->name);
            return NULL;
        }
        else
        {
            insert_struct(Dec_field->type, Dec_name);
            insert_struct(NULL, struct_name); // 将结构体名字也插入，方便定义变量的时候查找
        }
        if (res_field == NULL)
        {
            res_field = Dec_field;
            tmp_field = res_field;
        }
        else
        {
            tmp_field->tail = Dec_field;
            tmp_field = tmp_field->tail;
        }
        DecListNode = getchild(DecListNode, 2);
    }
    return res_field;
}

FieldList Dec_struct(struct Node *cur_node, Type cur_type)
{
    //Dec -> VarDec
    //| VarDec ASSIGNOP Exp
    // 这里不能出现VarDec ASSIGNOP Exp
    FieldList VarDec_field = VarDec(getchild(cur_node, 0), cur_type);
    if(VarDec_field == NULL)
        return NULL;
    if (getchild(cur_node, 1) != NULL) //定义是不能初始化，报错15
    {
        print_error(15, cur_node->line_num, VarDec_field->name);
        return NULL;
    }
    return VarDec_field;
}

FieldList VarDec(struct Node *cur_node, Type cur_type)
{

    /*	VarDec -> ID
	| VarDec LB INT RB
	*/
    FieldList tmp_field = (FieldList)(malloc(sizeof(struct FieldList_)));
    tmp_field->tail = NULL;

    struct Node *tmp_node = getchild(cur_node, 0);

    if (strcmp(tmp_node->name, "ID") == 0)
    {
        Type temp_type_ = (Type)malloc(sizeof(struct Type_));
        temp_type_->kind = cur_type->kind;
        temp_type_->u = cur_type->u;
        tmp_field->type = temp_type_;
        tmp_field->name = tmp_node->string_content;
        return tmp_field;
    }
    else
    { //VarDec LB INT RB
        int num_cnt = 0; //有几个中括号
        while (tmp_node->child != NULL)
        {
            tmp_node = tmp_node->child;
            num_cnt++;
        }
        struct Type_ **node_typeList = (struct Type_ **)malloc(sizeof(struct Type_ **) * (num_cnt + 2));
        tmp_field->name = tmp_node->string_content; // 数组的ID

        tmp_node = getchild(cur_node, 0);
        struct Node *INT_node1 = NULL;
        num_cnt--;
        int top_count = num_cnt;
        while (tmp_node->child != NULL)
        {
            Type tmp_Type = (Type)malloc(sizeof(struct Type_));
            INT_node1 = tmp_node->next_sib->next_sib;
            tmp_Type->kind = ARRAY;
            tmp_Type->u.array.size = INT_node1->int_content;
            node_typeList[num_cnt] = tmp_Type;
            num_cnt--;
            tmp_node = tmp_node->child;
        }
        Type temp_type = node_typeList[0];
        Type tmp_Type_ = (Type)malloc(sizeof(struct Type_));
        tmp_Type_->kind = cur_type->kind;
        tmp_Type_->u = cur_type->u;
        node_typeList[top_count]->u.array.elem = tmp_Type_;
        for (int i = 1; i <= top_count; i++)
        {
            temp_type->u.array.elem = node_typeList[i];
            temp_type = temp_type->u.array.elem;
        }
        tmp_field->type = node_typeList[0];
        return tmp_field;
    }
}

int ExtDecList(struct Node *cur_node, Type cur_type)
{
    /*ExtDecList -> VarDec
    | VarDec COMMA ExtDecList
    */
    FieldList VarDec_field = VarDec(getchild(cur_node, 0), cur_type);
    if(VarDec_field == NULL)
        return 0;

    if (find_symbol(VarDec_field->name, depth_) != NULL) //变量重复定义，报错3
    {
        print_error(3, cur_node->line_num, VarDec_field->name);
        return 0;
    }
    else
    {   //没出错就插入符号表
        ST_node Insert_node = NULL;
        if(VarDec_field->type->kind == ARRAY)
        {
            Insert_node = new_STnode(STRUCT_NAME, VarDec_field->type, VarDec_field->name, 1, depth_);
        }
        else
        {
            Insert_node = new_STnode(VARIABLE, VarDec_field->type, VarDec_field->name, 1, depth_);
        }
        insert_symbol(Insert_node, find_domain(depth_));
    }

    if (getchild(cur_node, 1) == NULL)
        return 0;
    else
    {
        struct Node *tmp_node2 = getchild(cur_node, 2);
        if (tmp_node2 != NULL)
            ExtDecList(tmp_node2, cur_type);
    }
    return 0;
}
