#include "semantic.h"
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
//操作数
typedef struct Operand_* Operand;
struct Operand_{
	enum { 
		VARIABLE_OPERAND, //变量
		CONSTANT_OPERAND, //常量
		FUNCTION_OPERAND, //函数
		TEMP_OPERAND,//暂时变量
		LABEL_OPERAND//标号数
		} kind;
	enum{
		VAL,
		ADDRESS
	} type;//为地址准备
	char *varName;//变量名
	char *funcName;//函数名
	int var_no;
    int value;
};

struct InterCode
{
	enum { 
		FUNCTION_INTERCODE,
		PARAM,
		RETURN,
		LABEL,
		GOTO,
		WRITE,
		READ,
		ARG,

		ASSIGN,
		DEC,
		CALL, 

		ADD, 
		SUB,
		MUL,
		DIV,

		IFGOTO
	} kind;
	union {
		struct { Operand result; } para_1;//单目
		struct { Operand right, left; } para_2;//双目
		struct { Operand result, op1, op2; } para_3;//三目
		struct { Operand op1, op2, op3; char *relop; } para_4;//四目
	} u;
};

typedef struct InterCodes_* InterCodes;
struct InterCodes_{//双向链表
	struct InterCode code;
	InterCodes prev;
	InterCodes next;
};
void Link_Insert(InterCodes cur);
void newIntercode(int kind, ...);
void printOP(Operand op, FILE *file);
void printIntercode(FILE *file);
Operand createOP(int kind, int address, ...);
int typeSize(Type cur);
void translate_start(struct Node* cur, FILE *fp);
void translate_Program(struct Node *cur);
void translate_ExtDefList(struct Node *cur);
void translate_ExtDef(struct Node *cur);
void translate_FunDec(struct Node *cur);
void translate_CompSt(struct Node *cur);
void translate_StmtList(struct Node *cur);
int translate_Stmt(struct Node *cur);
void translate_DefList(struct Node *cur);
void translate_Def(struct Node *cur);
void translate_DecList(struct Node *cur);
void translate_Dec(struct Node *cur);
Operand translate_VarDec(struct Node *cur);
void translate_Arg(struct Node *cur, FieldList para);
Operand translate_Exp(struct Node *cur);
void translate_Cond(struct Node* cur,Operand label_true,Operand label_false);
