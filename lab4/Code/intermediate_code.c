#include "intermediate_code.h"
int varCount = 0, tmpCount = 0, labelCount = 0;
InterCodes head_code = NULL, tail_code = NULL;

//复制操作（有时候一些操作数被加入intercode之后，他们的type会在后面发生改变，因此要用这个函数保存）
Operand copyOP(Operand op)
{
    Operand ans = (Operand)malloc(sizeof(struct Operand_));
    ans->kind = op->kind;
    ans->type = ans->type;
    ans->varName = op->varName;
    ans->var_no = op->var_no;
    ans->funcName = op->funcName;
    ans->value = op->value;
    ans->depth = op->depth;
    return ans;
}

int getarraydepth(ST_node arr_node)
{
    int cnt = 0;
    Type temp = arr_node->type;

    while (temp->kind == ARRAY)
    {
        cnt += 1;
        temp = temp->u.array.elem;
    }
    return cnt;
}

//双向链表的插入
void Link_Insert(InterCodes cur)
{
    cur->prev = tail_code;
    cur->next = head_code;
    tail_code->next = cur;
    head_code->prev = cur;
    tail_code = cur;
}

//中间代码生成
void newIntercode(int kind, ...)
{
    va_list args;
    va_start(args, kind);
    InterCodes p1 = (InterCodes)(malloc(sizeof(struct InterCodes_)));
    p1->code.kind = kind;
    p1->next = NULL;

    switch (kind)
    {
    case FUNCTION_INTERCODE:
    case PARAM:
    case RETURN:
    case LABEL:
    case GOTO:
    case WRITE:
    case READ:
    case ARG:
    {
        p1->code.u.para_1.result = va_arg(args, Operand);
        assert(p1->code.u.para_1.result != NULL);
        break;
    }

    case ASSIGN:
    case DEC:
    case CALL:
    {
        p1->code.u.para_2.left = va_arg(args, Operand);
        p1->code.u.para_2.right = va_arg(args, Operand);
        break;
    }

    case ADD:
    case SUB:
    case MUL:
    case DIV:
    {
        p1->code.u.para_3.result = va_arg(args, Operand);
        p1->code.u.para_3.op1 = va_arg(args, Operand);
        p1->code.u.para_3.op2 = va_arg(args, Operand);
        break;
    }

    case IFGOTO:
    {
        p1->code.u.para_4.op1 = va_arg(args, Operand);
        p1->code.u.para_4.relop = va_arg(args, char *);
        p1->code.u.para_4.op2 = va_arg(args, Operand);
        p1->code.u.para_4.op3 = va_arg(args, Operand);
    }
    }
    Link_Insert(p1);
    va_end(args);
}

//打印符号
void printOP(Operand op, FILE *file)
{
    switch (op->kind)
    {
    case (VARIABLE_OPERAND):
    {
        if (op->type == ADDRESS_AND)
        {
            fprintf(file, "&");
        }
        else if (op->type == ADDRESS_STAR)
        {
            fprintf(file, "*");
        }
        fprintf(file, "v%d", op->var_no);
        break;
    }
    case (CONSTANT_OPERAND):
    {
        fprintf(file, "#%d", op->value);
        break;
    }
    case (FUNCTION_OPERAND):
    {
        fprintf(file, "%s", op->funcName);
        break;
    }
    case (TEMP_OPERAND):
    {
        if (op->type == ADDRESS_STAR)
        {
            fprintf(file, "*");
        }
        else if (op->type == ADDRESS_AND)
        {
            fprintf(file, "&");
        }
        fprintf(file, "t%d", op->var_no);
        break;
    }
    case (LABEL_OPERAND):
    {
        fprintf(file, "%d", op->var_no);
        break;
    }
    }
}

void printIntercode(FILE *file)
{
    InterCodes p1 = head_code->next;
    if(p1 == NULL)
        return;
    while (p1 != head_code)
    {
        switch (p1->code.kind)
        {
        case (FUNCTION_INTERCODE):
        {
            fprintf(file, "FUNCTION ");
            printOP(p1->code.u.para_1.result, file);
            fprintf(file, " : \n");
            break;
        }
        case (PARAM):
        {
            fprintf(file, "PARAM ");
            printOP(p1->code.u.para_1.result, file);
            fprintf(file, "\n");
            break;
        }
        case (RETURN):
        {
            fprintf(file, "RETURN ");
            printOP(p1->code.u.para_1.result, file);
            fprintf(file, "\n");
            break;
        }
        case (LABEL):
        {
            fprintf(file, "LABEL label");
            printOP(p1->code.u.para_1.result, file);
            fprintf(file, " : \n");
            break;
        }
        case (GOTO):
        {
            fprintf(file, "GOTO label");
            printOP(p1->code.u.para_1.result, file);
            fprintf(file, "\n");
            break;
        }
        case (WRITE):
        {
            fprintf(file, "WRITE ");
            printOP(p1->code.u.para_1.result, file);
            fprintf(file, "\n");
            break;
        }
        case (READ):
        {
            fprintf(file, "READ ");
            printOP(p1->code.u.para_1.result, file);
            fprintf(file, "\n");
            break;
        }
        case (ARG):
        {
            fprintf(file, "ARG ");
            printOP(p1->code.u.para_1.result, file);
            fprintf(file, " \n");
            break;
        }
        case (ASSIGN):
        {
            printOP(p1->code.u.para_2.left, file);
            fprintf(file, " := ");
            printOP(p1->code.u.para_2.right, file);
            fprintf(file, "\n");
            break;
        }
        case (DEC):
        {
            fprintf(file, "DEC ");
            printOP(p1->code.u.para_2.left, file);
            fprintf(file, " %d", p1->code.u.para_2.right->value);
            fprintf(file, "\n");
            break;
        }
        case (CALL):
        {
            printOP(p1->code.u.para_2.left, file);
            fprintf(file, " := CALL ");
            printOP(p1->code.u.para_2.right, file);
            fprintf(file, "\n");
            break;
        }
        case (ADD):
        {
            printOP(p1->code.u.para_3.result, file);
            fprintf(file, " := ");
            printOP(p1->code.u.para_3.op1, file);
            fprintf(file, " + ");
            printOP(p1->code.u.para_3.op2, file);
            fprintf(file, "\n");
            break;
        }
        case (SUB):
        {
            printOP(p1->code.u.para_3.result, file);
            fprintf(file, " := ");
            printOP(p1->code.u.para_3.op1, file);
            fprintf(file, " - ");
            printOP(p1->code.u.para_3.op2, file);
            fprintf(file, "\n");
            break;
        }
        case (MUL):
        {
            printOP(p1->code.u.para_3.result, file);
            fprintf(file, " := ");
            printOP(p1->code.u.para_3.op1, file);
            fprintf(file, " * ");
            printOP(p1->code.u.para_3.op2, file);
            fprintf(file, "\n");
            break;
        }
        case (DIV):
        {
            printOP(p1->code.u.para_3.result, file);
            fprintf(file, " := ");
            printOP(p1->code.u.para_3.op1, file);
            fprintf(file, " / ");
            printOP(p1->code.u.para_3.op2, file);
            fprintf(file, "\n");
            break;
        }
        case (IFGOTO):
        {
            fprintf(file, "IF ");
            printOP(p1->code.u.para_4.op1, file);
            fprintf(file, " %s ", p1->code.u.para_4.relop);
            printOP(p1->code.u.para_4.op2, file);
            fprintf(file, " GOTO label");
            printOP(p1->code.u.para_4.op3, file);
            fprintf(file, "\n");
            break;
        }
        }
        p1 = p1->next;
    }
    fclose(file);
}

Operand createOP(int kind, int address, ...)
{
    va_list args;
    va_start(args, address);
    Operand createOP = (Operand)(malloc(sizeof(struct Operand_)));
    createOP->kind = kind;
    createOP->type = address;
    createOP->funcName = NULL;
    createOP->varName = NULL;
    switch (kind)
    {
    case (VARIABLE_OPERAND):
    {
        createOP->varName = va_arg(args, char *);
        createOP->var_no = varCount;
        varCount++;
        break;
    }
    case (CONSTANT_OPERAND):
    {
        createOP->value = va_arg(args, int);
        break;
    }
    case (FUNCTION_OPERAND):
    {
        createOP->funcName = va_arg(args, char *);
        break;
    }
    case (LABEL_OPERAND):
    {
        createOP->var_no = labelCount;
        labelCount++;
        break;
    }
    case (TEMP_OPERAND):
    {
        createOP->var_no = tmpCount;
        tmpCount++;
        break;
    }
    }
    va_end(args);
    return createOP;
}

int typeSize(Type cur)
{
    assert(cur != NULL);
    if (cur->kind == BASIC)
    {
        return 4;
    }
    else if (cur->kind == ARRAY)
    {
        int arrSize = 1;
        Type tmpType = cur;
        while (tmpType != NULL)
        {
            if (tmpType->kind != ARRAY)
                break;
            arrSize *= tmpType->u.array.size;
            tmpType = tmpType->u.array.elem;
        }
        arrSize *= typeSize(tmpType);
        return arrSize;
    }
    else if (cur->kind == STRUCTURE)
    {
        int strucSize = 0;
        FieldList tmpStruct = cur->u.my_struct.structure;
        while (tmpStruct != NULL)
        {
            strucSize += typeSize(tmpStruct->type);
            tmpStruct = tmpStruct->tail;
        }
        return strucSize;
    }
}

int get_struct_offset(Type struct_type, char* name)
{
    FieldList head = struct_type->u.my_struct.structure;
    int res = 0;
    while(head != NULL)
    {
        if(strcmp(name, head->name) == 0)
            break;
        res += typeSize(head->type);
        head = head->tail;
    }
    return res;
}

//开始生成中间代码
void translate_start(struct Node* cur, FILE *fp)
{
    head_code = (InterCodes)malloc(sizeof(struct InterCodes_));
    head_code->next = NULL;
    head_code->prev = NULL;
    tail_code = head_code;
    translate_Program(cur);
    if(LAB4_DBEUG)
        printIntercode(fp); //lab4不需要
}

void translate_Program(struct Node *cur)
{
    translate_ExtDefList(getchild(cur, 0));
    return;
}

void translate_ExtDefList(struct Node *cur)
{
    
    struct Node *ExtDef_node = getchild(cur, 0);
    if (ExtDef_node != NULL)
    {
        translate_ExtDef(ExtDef_node);
        struct Node *ExtDefList_node = getchild(cur, 1);
        if (ExtDefList_node != NULL)
        {
            translate_ExtDefList(ExtDefList_node);
        }
    }
}

void translate_ExtDef(struct Node *cur)
{
    // ExtDef -> Specifier ExtDecList SEMI (这次实验没有全局变量)
    // | Specifier SEMI (不用生成中间代码)
    // | Specifier FunDec CompSt 
    // | Specifier FunDec SEMI (这次没有函数声明)
    struct Node *FunDec = getchild(cur, 1);
    assert(FunDec != NULL);
    if (strcmp(FunDec->name, "FunDec") == 0)
    {
        struct Node *CompSt = getchild(cur, 2);
        if (strcmp(CompSt->name, "CompSt")==0)
        {
            translate_FunDec(FunDec);
            translate_CompSt(CompSt);
        }
    }
}

void translate_FunDec(struct Node *cur)
{
    //FunDec -> ID LP VarList RP
    //| ID LP RP
    struct Node *ID = getchild(cur, 0);
    Operand funcop = createOP(FUNCTION_OPERAND, VAL, ID->string_content);
    newIntercode(FUNCTION_INTERCODE, funcop);

    ST_node func_symbol = find_symbol(funcop->funcName, 0);
    if (func_symbol == NULL)
    {
        printf("unsuccessful func_symbol query!\n");
        assert(0);
    }

    int para_num = func_symbol->type->u.function.para_num;
    if (para_num != 0)
    {
        FieldList paras = func_symbol->type->u.function.paras;
        while (paras != NULL)
        {
            Operand paraop = NULL;
            if (paras->type->kind == ARRAY || paras->type->kind == STRUCTURE)
                paraop = createOP(VARIABLE_OPERAND, ADDRESS_AND, (char *)paras->name);
            else
                paraop = createOP(VARIABLE_OPERAND, VAL, (char *)paras->name);
            ST_node paras_node = find_symbol(paras->name, __INT_MAX__);
            if (paras_node == NULL)
            {
                printf("unsuccessful parameters query!\n");
                assert(0);
            }
            paras_node->var_no = paraop->var_no;
            paras_node->ifaddress = paraop->type;

            Operand paraop_val = copyOP(paraop);
            paraop_val->type = VAL;
            newIntercode(PARAM, paraop_val);

            paras = paras->tail;
        }
    }
}

void translate_CompSt(struct Node *cur)
{
    // CompSt -> LC DefList StmtList RC
    // DefList -> Def DefList
    // | (empty)
    struct Node *temp = getchild(cur, 1);
    if (strcmp(temp->name, "DefList") == 0)
    {
        translate_DefList(temp);
        struct Node *StmtList = getchild(cur, 2);
        if (strcmp(StmtList->name, "StmtList") == 0)
        {
            translate_StmtList(StmtList);
        }
    }
    else if (strcmp(temp->name, "StmtList")==0)
    {
        translate_StmtList(temp);
    }
}

void translate_StmtList(struct Node *cur)
{
    struct Node *Stmt_node = getchild(cur, 0);
    if (Stmt_node != NULL)
    {
        if (strcmp(Stmt_node->name, "Stmt") == 0)
        {
            translate_Stmt(Stmt_node);
            struct Node *temp = getchild(cur, 1);
            if (temp != NULL)
            {
                translate_StmtList(temp);
            }
        }
    }
}

int translate_Stmt(struct Node *cur)
{
    struct Node *temp1 = getchild(cur, 0);
    if (strcmp(temp1->name, "CompSt") == 0)
    {
        translate_CompSt(temp1);
    }
    else if (strcmp(temp1->name, "Exp") == 0)
    {
        translate_Exp(temp1, 0);
    }
    else if (strcmp(temp1->name, "RETURN") == 0)
    {
        struct Node *expnode = getchild(cur, 1);
        Operand expop = translate_Exp(expnode, 0);
        newIntercode(RETURN, expop);
    }
    else if (strcmp(temp1->name, "WHILE") == 0)
    {
        Operand label1 = createOP(LABEL_OPERAND, VAL);
        Operand label2 = createOP(LABEL_OPERAND, VAL);
        Operand label3 = createOP(LABEL_OPERAND, VAL);
        newIntercode(LABEL, label1);
        struct Node *exp_node = getchild(cur, 2);
        translate_Cond(exp_node, label2, label3);
        newIntercode(LABEL, label2);
        struct Node *stmt_node = getchild(cur, 4);
        translate_Stmt(stmt_node);
        newIntercode(GOTO, label1);
        newIntercode(LABEL, label3);
    }
    else if (strcmp(temp1->name, "IF") == 0)
    {
        struct Node *exp_node = getchild(cur, 2);
        struct Node *stmt_node1 = getchild(cur, 4);
        struct Node *temp2 = getchild(cur, 5);
        if (temp2 == NULL)
        {
            Operand label1 = createOP(LABEL_OPERAND, VAL);
            Operand label2 = createOP(LABEL_OPERAND, VAL);
            
            translate_Cond(exp_node, label1, label2);
            newIntercode(LABEL, label1);

            translate_Stmt(stmt_node1);
            newIntercode(LABEL, label2);
        }
        else
        {
            Operand label1 = createOP(LABEL_OPERAND, VAL);
            Operand label2 = createOP(LABEL_OPERAND, VAL);
            Operand label3 = createOP(LABEL_OPERAND, VAL);
            translate_Cond(exp_node, label1, label2);
            newIntercode(LABEL, label1);
            translate_Stmt(stmt_node1);
            newIntercode(GOTO, label3);
            newIntercode(LABEL, label2);
            struct Node *stmt_node2 = getchild(cur, 6);
            translate_Stmt(stmt_node2);
            newIntercode(LABEL, label3);
        }
    }
    return 1;
}

void translate_DefList(struct Node *cur)
{
    struct Node *temp = getchild(cur, 0);
    if (temp != NULL)
    {
        struct Node *Def_node = temp;
        translate_Def(Def_node);
        struct Node *DefList_node = getchild(cur, 1);
        if (DefList_node != NULL)
        {
            translate_DefList(DefList_node);
        }
    }
}

void translate_Def(struct Node *cur)
{
    struct Node *DecList_node = getchild(cur, 1);
    translate_DecList(DecList_node);
}

void translate_DecList(struct Node *cur)
{
    struct Node *dec_node = getchild(cur, 0);
    translate_Dec(dec_node);
    if (getchild(cur, 1) != NULL)
    {
        struct Node *DecList_node = getchild(cur, 2);
        if (DecList_node != NULL)
        {
            translate_DecList(DecList_node);
        }
    }
}

void translate_Dec(struct Node *cur)
{
    struct Node *VarDec_node = getchild(cur, 0);
    if (getchild(cur, 1) == NULL)
    {
        translate_VarDec(VarDec_node); //像int n;这种暂时不需要生成中间代码，到后面对n赋值的时候才需要
    }
    else
    {
        Operand t1 = translate_VarDec(VarDec_node);
        Operand t2 = translate_Exp(getchild(cur, 2), 0);
        newIntercode(ASSIGN, t1, t2);
    }
}

Operand translate_VarDec(struct Node *cur)
{
    Operand result = NULL;

    struct Node *ID_node = getchild(cur, 0);
    if (strcmp(ID_node->name, "ID") == 0)
    {
        //ID
        ST_node my_id = find_symbol(ID_node->string_content, __INT_MAX__);
        assert(my_id != NULL);
        int typesize = typeSize(my_id->type);
        result = createOP(VARIABLE_OPERAND, VAL, ID_node->string_content);
        my_id->ifaddress = 0;//这个node对应的变量里面存放的不是address
        my_id->var_no = result->var_no;
        if (my_id->type->kind == STRUCTURE)
        {
            //结构体, 需要计算申请空间
            Operand op = createOP(CONSTANT_OPERAND, VAL, typesize);
            Operand result_copy = copyOP(result);
            newIntercode(DEC, result_copy, op);
            result->type = ADDRESS_AND;
        }
        else if(my_id->type->kind == ARRAY)
        {
            //数组, 需要计算申请空间
            Operand op = createOP(CONSTANT_OPERAND, VAL, typesize);
            Operand result_copy = copyOP(result);
            newIntercode(DEC, result_copy, op);
            result->type = ADDRESS_AND;
        }
    }
    else//有待确认
    { 
        //VarDec LB INT RB 
        struct Node *find_node = getchild(ID_node, 0); //这时候ID_node是VarDec
        while (strcmp(find_node->name, "ID") != 0)
            find_node = find_node->child;

        ST_node my_id = find_symbol(find_node->string_content, __INT_MAX__);

        result = createOP(VARIABLE_OPERAND, VAL, find_node->string_content);
        my_id->var_no = result->var_no;
        my_id->ifaddress = 0;
        int arraysize = typeSize(my_id->type);
        Operand op = createOP(CONSTANT_OPERAND, VAL, arraysize);
        Operand result_copy = copyOP(result);
        newIntercode(DEC, result_copy, op);
        result->type = ADDRESS_AND;
    }

    return result;
}

void translate_Arg(struct Node *cur, FieldList para)
{
    if (cur == NULL || para == NULL)
        return;

    Operand temp_op = translate_Exp(getchild(cur, 0), 0);

    if (para->type->kind == STRUCTURE || para->type->kind == ARRAY)
    {
        assert(0); //lab4不应该函数参数有数组或者结构体
        //这里应该传地址
        if(temp_op->type != VAL)
        {
            temp_op->type = VAL;
        }
        else
            temp_op->type = ADDRESS_AND;
    }
    else
    {
        //这里要获得值
        if(temp_op->type == VAL)
        {
            //do nothing
        }
        else
            temp_op->type = ADDRESS_STAR;
    }
    
    if (getchild(cur, 1) != NULL)
        translate_Arg(getchild(cur, 2), para->tail);
    
    newIntercode(ARG, temp_op);
}

//返回值就是pdf 中的place
Operand translate_Exp(struct Node *cur, int isleft) //isleft表示当前这个exp是左值还是右值(默认右值)
{
    Operand place=NULL;
    struct Node* my_node1 = getchild(cur, 0);
	if(strcmp(my_node1->name,"ID") == 0)
    {
		struct Node* my_node2 = getchild(cur, 1);
		if(my_node2==NULL)
        {
            ST_node ID_node = find_symbol(my_node1->string_content, __INT_MAX__);
			
            assert(ID_node != NULL);
			if(ID_node->type->kind==ARRAY||ID_node->type->kind==STRUCTURE)
            {
                place = createOP(VARIABLE_OPERAND, ID_node->ifaddress, my_node1->string_content);
				varCount--; //这里不是在定义变量
				place->var_no = ID_node->var_no;
				place->depth=0;
				return place;
			}
            else
            {
				place = createOP(VARIABLE_OPERAND,VAL,my_node1->string_content);
				varCount--;
				place->var_no=ID_node->var_no;
				place->depth=0;
				return place;
			}
		}
        else //函数调用
        {
			struct Node* my_node3=getchild(cur,2);

			if(strcmp(my_node1->string_content,"write")==0)
            {
				if(strcmp(my_node3->name,"Args") == 0)
                {
					struct Node* my_node31=getchild(my_node3,0);
					Operand op_temp = NULL;
					if(strcmp(my_node31->name,"Exp")==0)
                    {
						op_temp = translate_Exp(my_node31, isleft);
					}
					if(op_temp != NULL)
					    newIntercode(WRITE, op_temp);

					return place;
				}
			}

			place=createOP(TEMP_OPERAND,VAL);
			if(strcmp(my_node1->string_content,"read")==0)
            {
				newIntercode(READ, place);
				return place;
			}

			Operand functionname=createOP(FUNCTION_OPERAND,VAL,my_node1->string_content);
			if(strcmp(my_node3->name,"RP") == 0)
            {
				newIntercode(CALL,place,functionname);
				return place;
			}
            else if(strcmp(my_node3->name,"Args")==0)
            {
				ST_node id_node=find_symbol(my_node1->string_content, __INT_MAX__);
				translate_Arg(my_node3,id_node->type->u.function.paras);
				newIntercode(CALL,place,functionname);
				return place;
			}
			return place;
		}
	}
    else if(strcmp(my_node1->name,"INT")==0)
    {
		place=createOP(CONSTANT_OPERAND,VAL,my_node1->int_content);
		return place;
	}
    else if(strcmp(my_node1->name,"FLOAT") == 0)
    {
        //不会有这种情况
		assert(0);	
	}
    else if(strcmp(my_node1->name,"LP") == 0)
    {
		struct Node* exp_node=getchild(cur,1);
		return translate_Exp(exp_node, isleft);
	}
    else if(strcmp(my_node1->name,"MINUS")==0)
    {
        struct Node* exp_node=getchild(cur,1);
        Operand t1 = translate_Exp(exp_node, isleft);

		Operand zero=createOP(CONSTANT_OPERAND, VAL,0);
		place = createOP(TEMP_OPERAND, VAL);

		newIntercode(SUB, place, zero, t1);

		return place;
	}
    else if(strcmp(my_node1->name,"NOT")==0||
		(strcmp(my_node1->name,"Exp")==0 &&getchild(cur, 1)!=NULL &&strcmp(getchild(cur, 1)->name,"RELOP")==0)||
		(strcmp(my_node1->name,"Exp")==0 &&getchild(cur, 1)!=NULL &&strcmp(getchild(cur, 1)->name,"AND")==0)||
		(strcmp(my_node1->name,"Exp")==0 &&getchild(cur, 1)!=NULL &&strcmp(getchild(cur, 1)->name,"OR")==0)
	)
    {
		Operand label1=createOP(LABEL_OPERAND, VAL);
		Operand label2=createOP(LABEL_OPERAND, VAL);
		place = createOP(TEMP_OPERAND,VAL);
		Operand zero = createOP(CONSTANT_OPERAND,VAL,0);
		newIntercode(ASSIGN,place,zero);

		translate_Cond(cur,label1,label2);

		newIntercode(LABEL,label1);

		Operand one=createOP(CONSTANT_OPERAND,VAL,1);
		newIntercode(ASSIGN,place,one);

		newIntercode(LABEL,label2);
		return place;
	}
    else if(strcmp(my_node1->name,"Exp")==0)
    {
		
		struct Node* my_node2=getchild(cur,1);
		if(     (strcmp(my_node2->name,"PLUS")==0)||
				(strcmp(my_node2->name,"MINUS")==0)||
				(strcmp(my_node2->name,"STAR")==0)||
				(strcmp(my_node2->name,"DIV")==0)
		)
        {
            int op_kind;
            if (strcmp(my_node2->name, "PLUS") == 0)
            {
                op_kind = ADD;
            }
            else if (strcmp(my_node2->name, "MINUS") == 0)
            {
                op_kind = SUB;
            }
            else if (strcmp(my_node2->name, "STAR") == 0)
            {
                op_kind = MUL;
            }
            else if (strcmp(my_node2->name, "DIV") == 0)
            {
                op_kind = DIV;
            }

            struct Node*exp_node1 = my_node1;
            struct Node*exp_node2 = getchild(cur,2);
            Operand t1 = translate_Exp(exp_node1, isleft);
            Operand t2 = translate_Exp(exp_node2, isleft);
            
            place = createOP(TEMP_OPERAND,VAL);
            if(t1!=NULL && t2!=NULL)
                newIntercode(op_kind, place, t1, t2);
            
            return place;
		}
		else if(strcmp(my_node2->name,"ASSIGNOP")==0) 
        {
            //现在仅仅考虑了Exp1->ID的情况
            if(strcmp(getchild(my_node1, 0)->name, "ID") == 0 && getchild(my_node1, 0)->next_sib == NULL)
            {
                //注意这里应该考虑如果ID是个地址该咋办
                struct Node* ID_node = getchild(my_node1, 0);
                struct Node* exp_node1 = my_node1;
                struct Node* exp_node2 = getchild(cur,2);
                place = translate_Exp(exp_node1, 1);
                Operand t1 = translate_Exp(exp_node2, 0);
                ST_node found = find_symbol(ID_node->string_content, __INT_MAX__);
                if(found->type->kind == ARRAY || found->type->kind == STRUCTURE)
                {
                    //这个时候place里面可能是值或者地址，要把它转换成地址，对于t1也是同理
                    /*if(place->type != VAL)
                    {
                        place->type = VAL;
                    }
                    else
                    {
                        place->type = ADDRESS_AND;
                    }*/
                    place->type = VAL;
                    if(t1->type != VAL)
                    {
                        t1->type = VAL;
                    }
                    else
                    {
                        t1->type = ADDRESS_AND;
                    }
                    newIntercode(ASSIGN, place, t1);
                    found->ifaddress = 1;
                    return place;
                }
                else
                    newIntercode(ASSIGN, place, t1);
                //其实我觉得这里不用返回place了，等于之后不会再用了
                return place;
            }
            //Exp1 -> Exp DOT ID ,这个时候Exp1一定是一个地址（左值）
            else if(strcmp(getchild(my_node1, 0)->name, "Exp") == 0 && strcmp(getchild(my_node1, 1)->name, "DOT") == 0)
            {
                struct Node* Exp_node1 = my_node1;
                struct Node* Exp_node2 = getchild(cur,2);
                Operand exp1 = translate_Exp(Exp_node1, 1); //这个时候exp1里面放的可能是地址或者值，我要把它转换成值
                //assert(exp1->type != VAL);
                if(exp1->type != VAL)//如果这个Exp对应的node存放的是地址
                {
                    exp1->type = ADDRESS_STAR; //这时候要的是传值*v
                }
                else
                {
                    //不用动，我要传值
                }
                Operand exp2 = translate_Exp(Exp_node2, 0);//这个时候exp2里面放的可能是地址或者值，我要把它转换成值

                if(exp2->type != VAL)
                {
                    exp2->type = ADDRESS_STAR;//如果是一个地址，应该转换成值
                }
                else
                {
                    //不用动，我要传值
                }
                newIntercode(ASSIGN, exp1, exp2);
                place = exp1;
                return place;
            }
            else //这时候Exp1应该是数组元素
            {
                struct Node* Exp_node1 = my_node1;
                assert(strcmp(getchild(my_node1, 0)->name, "Exp") == 0 && strcmp(getchild(my_node1, 1)->name, "LB") == 0);
                struct Node* Exp_node2 = getchild(cur,2);
                Operand exp1 = translate_Exp(Exp_node1, 1); //这个时候exp1里面放的可能是地址或者值，我要把它转换成值

                if(exp1->type != VAL)//如果这个Exp对应的node存放的是地址
                {
                    exp1->type = ADDRESS_STAR; //这时候要的是传值*v
                }
                else
                {
                    //不用动，我要传值
                }
                Operand exp2 = translate_Exp(Exp_node2, 0);////这个时候exp2里面放的可能是地址或者值，我要把它转换成值
                if(exp2->type != VAL)
                {
                    exp2->type = ADDRESS_STAR;//如果是一个地址，应该转换成值
                }
                else
                {
                    //不用动，我要传值
                }
                newIntercode(ASSIGN, exp1, exp2);

                place = exp1;
                return place;
            }
		}
        else if(strcmp(my_node2->name,"DOT")==0)
        {
			Operand exp_op = translate_Exp(my_node1, isleft);//这里Exp1有可能是一个数组的形式比如a[1]
			struct Node* ID_node = getchild(cur,2);
            Type struct_type = Exp(my_node1);
            int offset = get_struct_offset(struct_type, ID_node->string_content);
			//这时候我要exp_op是个地址;
            if(exp_op->type != VAL)//如果这个Exp对应的node存放的是地址
            {
                exp_op->type = VAL;//打印的时候就打印v就可以了
            }
            else
            {
                exp_op->type = ADDRESS_AND; //这时候要传地址&v
            }
            Operand constantop = createOP(CONSTANT_OPERAND,VAL,offset);
            Operand newtemp = createOP(TEMP_OPERAND, VAL);//这里存放的是地址
            Operand newtemp_copy = copyOP(newtemp);
            newIntercode(ADD, newtemp_copy, exp_op, constantop);
            //现在问题是即使我在等号右边，也有可能要传的是地址（比如A.x[1]这个时候即使他在等号右边，我的newtemp也应该传地址)
            ST_node found = find_struct(ID_node->string_content);
            if(found->type->kind == ARRAY) //这个时候我们一直要的是地址,不管是否isleft
            {
                //所以这里不用动
            }
            else
            {
                if(isleft)
                {
                    //不用动，这时候要的是地址
                }
                else
                {
                    newtemp->type = ADDRESS_STAR; //这时候要的是传值*v
                }
            }

            place = createOP(TEMP_OPERAND,VAL);
            Operand place_copy = copyOP(place);
            newIntercode(ASSIGN, place_copy, newtemp);
            place->depth = 0;
            place->varName = ID_node->string_content;
            if(isleft == 1)//这时候place里面存放的是地址
                place->type = ADDRESS_AND;
            else
                place->type = VAL;
            if(found->type->kind == ARRAY) //这个时候我们一直要的是地址,不管是否isleft
            {
                place->type = ADDRESS_AND;
            }
            return place;
		}
		else if(strcmp(my_node2->name,"LB")==0)
        {
			Operand exp1 = translate_Exp(my_node1, isleft);//这里exp1里面有可能是值或者地址
            assert(exp1 != NULL);
			int depth = exp1->depth; //Exp1中有几个中括号
			ST_node queryid = NULL;
            queryid = find_symbol(exp1->varName,__INT_MAX__); //注意这里可能是结构体内定义的数组变量！
            if(queryid == NULL)
                queryid = find_struct(exp1->varName);
            assert(queryid != NULL);
			Type temptype = queryid->type;
            assert(temptype->kind == ARRAY);

			int cnt = getarraydepth(queryid);
			int* arraysize = (int* )malloc(sizeof(int)*(cnt + 1));

			cnt = 0;
			while(temptype->kind==ARRAY)
            {
				arraysize[cnt] = temptype->u.array.size; //arraysize[0]指的是最外层的[]里有几个元素
				cnt++;
				temptype = temptype->u.array.elem;
			}
            int element_size = typeSize(temptype); //array中装的元素的长度（比如int是4）

			int ptr = cnt - 1;
			int tempdepth = cnt - depth - 1;
			int offset = 1;
			while(tempdepth > 0)
            {
				offset *= arraysize[ptr];
				tempdepth--;
				ptr--;
			}

			offset = offset * element_size;

			struct Node*Exp_node2 = getchild(cur,2);
			Operand exp2 = translate_Exp(Exp_node2, isleft);
			
            Operand tempt1 = createOP(TEMP_OPERAND,VAL);
			Operand constant1 = createOP(CONSTANT_OPERAND,VAL,offset);
			newIntercode(MUL,tempt1, exp2, constant1);

			Operand tempt2 = createOP(TEMP_OPERAND,VAL);
			tempt2->varName = exp1->varName;
			tempt2->depth = depth + 1;

            if(exp1->type != VAL)//如果这个Exp对应的node存放的是地址
            {
                exp1->type = VAL;//打印的时候就打印v就可以了
            }
            else
            {
                exp1->type = ADDRESS_AND; //这时候要传地址&v
            }
            Operand tempt2_copy = copyOP(tempt2);
			newIntercode(ADD,tempt2_copy, exp1, tempt1); //tempt2里面存的是一个地址，所以要把exp1也转换成一个地址
            place = copyOP(tempt2);
            //这时候place里面存放的是地址
            place->type = ADDRESS_STAR;
            return place;
		}
	}
	return place;
}

void translate_Cond(struct Node* cur,Operand label_true,Operand label_false)
{
    assert(cur != NULL);

	Operand zero = createOP(CONSTANT_OPERAND,VAL,0);
    struct Node* my_node1 = getchild(cur,0);
    if(strcmp(my_node1->name,"Exp")==0)
    {
        struct Node* my_node2 = getchild(cur,1);
        if(strcmp(my_node2->name,"AND")==0)
        {
            Operand new_label = createOP(LABEL_OPERAND,VAL);
            translate_Cond(my_node1, new_label, label_false);

            newIntercode(LABEL,new_label);

            struct Node* my_node3 = getchild(cur,2);
            translate_Cond(my_node3, label_true, label_false);            
        }
        else if(strcmp(my_node2->name,"OR")==0)
        {
            Operand new_label = createOP(LABEL_OPERAND,VAL);
            translate_Cond(my_node1,label_true,new_label);
            
            newIntercode(LABEL,new_label);

            struct Node* my_node3 = getchild(cur,2);
            translate_Cond(my_node3,label_true,label_false);
            			
        }
        else if(strcmp(my_node2->name,"RELOP")==0)
        {
            Operand op1 = translate_Exp(my_node1, 0);
            struct Node* my_node3 = getchild(cur,2);
            Operand op2 = translate_Exp(my_node3, 0);

            newIntercode(IFGOTO,op1,my_node2->string_content,op2,label_true);
            newIntercode(GOTO,label_false);
        }
    }
    else if(strcmp(my_node1->name,"NOT")==0)
    {
        struct Node* expnode = getchild(cur,1);
        translate_Cond(expnode,label_false,label_true);
    }
    else if(strcmp(my_node1->name,"LP")==0)
    {
        struct Node* expnode = getchild(cur,1);
        translate_Cond(expnode,label_true,label_false);
    }
    else
    {
        Operand op = translate_Exp(cur, 0);
        newIntercode(IFGOTO,op,"!=",zero,label_true);
        newIntercode(GOTO,label_false);
    }
}