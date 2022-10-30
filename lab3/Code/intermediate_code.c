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
        if (op->type == ADDRESS)
        {
            fprintf(file, "&");
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
        if (op->type == ADDRESS)
        {
            fprintf(file, "*");
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
    return 4;
}

//开始生成中间代码
void translate_start(struct Node* cur, FILE *fp)
{
    head_code = (InterCodes)malloc(sizeof(struct InterCodes_));
    head_code->next = NULL;
    head_code->prev = NULL;
    tail_code = head_code;
    translate_Program(cur);
    printIntercode(fp);
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
                paraop = createOP(VARIABLE_OPERAND, ADDRESS, (char *)paras->name);
            else
                paraop = createOP(VARIABLE_OPERAND, VAL, (char *)paras->name);

            ST_node query_paras = find_symbol(paras->name, __INT_MAX__);
            if (query_paras == NULL)
            {
                printf("unsuccessful parameters query!\n");
                assert(0);
            }

            query_paras->var_no = paraop->var_no;
            query_paras->ifaddress = paraop->type;
            paraop->type = VAL;
            newIntercode(PARAM, paraop);
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
        translate_Exp(temp1);
    }
    else if (strcmp(temp1->name, "RETURN") == 0)
    {
        struct Node *expnode = getchild(cur, 1);
        Operand expop = translate_Exp(expnode);
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
        Operand t2 = translate_Exp(getchild(cur, 2));
        newIntercode(ASSIGN, t1, t2);
    }
}

Operand translate_VarDec(struct Node *cur)
{
    Operand result = NULL;

    struct Node *ID_node = getchild(cur, 0);
    if (strcmp(ID_node->name, "ID") == 0)
    {
        ST_node my_id = find_symbol(ID_node->string_content, __INT_MAX__);
        assert(my_id != NULL);
        int typesize = typeSize(my_id->type);
        result = createOP(VARIABLE_OPERAND, VAL, ID_node->string_content);
        my_id->ifaddress = result->type;
        my_id->var_no = result->var_no;
        if (typesize != 4)
        {
            //结构体，需要计算申请空间
            Operand op = createOP(CONSTANT_OPERAND, VAL, typesize);
            newIntercode(DEC, result, op);
        }
    }
    else
    {
        struct Node *find_node = getchild(ID_node, 0);
        while (strcmp(find_node->name, "ID") != 0)
            find_node = find_node->child;

        ST_node my_id = find_symbol(find_node->string_content, __INT_MAX__);

        result = createOP(VARIABLE_OPERAND, VAL, find_node->string_content);
        my_id->ifaddress = result->type;
        my_id->var_no = result->var_no;
        int arraysize = typeSize(my_id->type);
        Operand op = createOP(CONSTANT_OPERAND, VAL, arraysize);
        newIntercode(DEC, result, op);
    }

    return result;
}

void translate_Arg(struct Node *cur, FieldList para)
{
    if (cur == NULL || para == NULL)
        return 0;

    Operand temp_op = translate_Exp(getchild(cur, 0));
    //Operand op = copyOP(temp_op); //?
    //op->address = !op->address; ???

    /* 暂时不考虑拓展功能
    if (para->type->kind == STRUCTURE || para->type->kind == ARRAY)
    {
        int flag = 0;
        if (para->type->kind == ARRAY)
        {
            char *name = op->varName;
            ST_node ARR_node = find_symbol(name, __INT_MAX__);
            int arraydepth = getarraydepth(ARR_node);
            if (op->depth < arraydepth)
                flag = 1;
            if (op->depth == 0)
                flag = 0;
        }
        if (flag == 1)
        {
            op->address = VAL;
        }
        else if (op->address == ADDRESS_OPERAND)
            op->address = VAL;
        else
            op->address = ADDRESS_OPERAND;
    }*/

    if (getchild(cur, 1) != NULL)
        translate_Arg(getchild(cur, 2), para->tail);
    
    newIntercode(ARG, temp_op);
}

//返回值就是pdf 中的place
Operand translate_Exp(struct Node *cur)
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

				if(ID_node->ifaddress==ADDRESS)
					place = createOP(VARIABLE_OPERAND,ADDRESS,my_node1->string_content);
				else
					place = createOP(VARIABLE_OPERAND,VAL,my_node1->string_content);
				varCount--; //这里不是在定义变量
				place->var_no = ID_node->var_no;

				//place->depth=0;
				return place;
			}
            else
            {
				place = createOP(VARIABLE_OPERAND,VAL,my_node1->string_content);
				varCount--;
				place->var_no=ID_node->var_no;

				//place->depth=0;
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
					Operand op_temp=NULL;
					if(strcmp(my_node31->name,"Exp")==0)
                    {
						op_temp = translate_Exp(my_node31);
					}
					if(op_temp != NULL)
					    newIntercode(WRITE, op_temp);
					Operand constant_op = createOP(CONSTANT_OPERAND,VAL,0);
					place = createOP(TEMP_OPERAND,VAL);
					newIntercode(ASSIGN,place,constant_op);

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
		return translate_Exp(exp_node);
	}
    else if(strcmp(my_node1->name,"MINUS")==0)
    {
        struct Node* exp_node=getchild(cur,1);
        Operand t1 = translate_Exp(exp_node);

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
            Operand t1 = translate_Exp(exp_node1);
            Operand t2 = translate_Exp(exp_node2);
            
            place = createOP(TEMP_OPERAND,VAL);
            if(t1!=NULL && t2!=NULL)
                newIntercode(op_kind, place, t1, t2);
            
            return place;
		}
		else if(strcmp(my_node2->name,"ASSIGNOP")==0) //现在仅仅考虑了第一个Exp是ID的情况
        {
            struct Node*exp_node1 = my_node1;
            struct Node*exp_node2 = getchild(cur,2);
            place = translate_Exp(exp_node1);
            Operand t1 = translate_Exp(exp_node2);

            newIntercode(ASSIGN, place, t1);
            place = t1;

            return place;
		}
		/* 先不考虑结构体和数组
        else if(strcmp(my_node2->name,"DOT")==0)
        {
			Operand exp_op=Exp(my_node1);
			Operand temp_expop=copyOP(exp_op);
			struct Node* my_node3=getchild(cur,2);
			int queryok=0;
			ST_node queryid=find_symbol(my_node3->string_content,__INT_MAX__);
			int offset=queryid->offset;

			if(offset==0){
				Operand ttemp=createOP(TEMP_OPERAND,VAL);
				if(temp_expop->address==ADDRESS)
				    temp_expop->address=VAL;
				else
					temp_expop->address=ADDRESS;
				newIntercode(ASSIGN,ttemp,temp_expop);
				place=copyOP(ttemp);
				place->address=ADDRESS;
				place->varName=my_node3->string_content;
				return place;
			}else{
				Operand constantop=createOP(CONSTANT_OPERAND,VAL,offset);
				Operand ttemp=createOP(TEMP_OPERAND,VAL);

				if(temp_expop->address==ADDRESS)
				temp_expop->address=VAL;
				else{
					temp_expop->address=ADDRESS;
				}

				newIntercode(ADD,ttemp,temp_expop,constantop);
				place=copyOP(ttemp);
				place->address=ADDRESS;
				place->varName=my_node3->string_content;
				return place;
			}
			
		}
		else if(strcmp(my_node2->name,"LB")==0){
			Operand expt1=copyOP(Exp(my_node1));
			int depth=expt1->depth;

			ST_node queryid=find_symbol(expt1->varName,__INT_MAX__);
			Type ttemptype=queryid->type;
			Type temptype=ttemptype;
			int cnt=0;
			while(temptype->kind==ARRAY){
				cnt+=1;
				temptype=temptype->u.array.elem;
			}
			int typesize=typeSize(temptype);
			int*arraysize=(int* )malloc(sizeof(int)*(cnt+1));
			cnt=0;
			temptype=ttemptype;
			while(temptype->kind==ARRAY){
				arraysize[cnt]=temptype->u.array.size;
				cnt+=1;
				temptype=temptype->u.array.elem;
			}
			int ptr=cnt-1;
			int tempdepth=cnt-depth-1;
			int offset=1;
			while(tempdepth){
				offset*=arraysize[ptr];
				tempdepth-=1;
				ptr-=1;
			}
			free(arraysize);
			offset=offset*typesize;

			struct Node*tempnode3=getchild(cur,2);
			Operand expt1=Exp(tempnode3);
			
			Operand tempt1=createOP(TEMP_OPERAND,VAL);
			Operand constantt1=createOP(CONSTANT_OPERAND,VAL,offset);
			newIntercode(MUL,tempt1,expt1,constantt1);

			Operand tempt1=createOP(TEMP_OPERAND,VAL);
			tempt1->varName=expt1->varName;
			tempt1->depth=depth+1;
			if(depth==0&&expt1->address==VAL){
				expt1->address=ADDRESS;
			}else{
				expt1->address=VAL;
			}
			newIntercode(ADD,tempt1,expt1,tempt1);

			place=copyOP(tempt1);
			if(tempt1->depth==cnt){
				place->address=ADDRESS;
			}
			return place;
		}*/
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
            translate_Cond(my_node1,new_label,label_false);
            
            newIntercode(LABEL,new_label);

            struct Node* my_node3 = getchild(cur,2);
            translate_Cond(my_node3,label_true,label_false);
            			
        }
        else if(strcmp(my_node2->name,"RELOP")==0)
        {
            Operand op1 = translate_Exp(my_node1);
            struct Node* my_node3 = getchild(cur,2);
            Operand op2 = translate_Exp(my_node3);

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
        Operand op = translate_Exp(cur);
        newIntercode(IFGOTO,op,"!=",zero,label_true);
        newIntercode(GOTO,label_false);
    }
}