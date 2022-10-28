#include "semantic.h"

//操作数
typedef struct Operand_* Operand;
struct Operand_{
	enum { 
		VARIABLE, //变量
		CONSTANT, //常量
		FUNCTION, //函数
		TEMPVAR,//暂时变量
		LABEL//标号数
		} kind;
	enum{
		ADDRESS,
		VAR
	} address;//为地址准备
	char *varName;//变量名
	char *funcName;//函数名
	int var_no;
    int value;
};

struct InterCode
{
	enum { 
		FUNCTION,
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
struct InterCodes_
{//双向链表
	struct InterCode code;
	InterCodes prev;
	InterCodes next;
};

int start_gen(struct Node *cur, FILE *fp);
void Program_gen(struct Node *cur);