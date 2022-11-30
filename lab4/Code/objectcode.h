#include "intermediate_code.h"
struct Register
{
	int state; //表示状态 0为空闲1为使用
	char* name; //表示名字
};
struct stack_node
{
	int offset;
	// enum{
	// 	S_VARIABLE=0,
	// 	S_TEMPVAR=3
	// }kind;
	int kind;
	int no;
	struct stack_node*next;
};
void Objectcode_gen(FILE*fp);