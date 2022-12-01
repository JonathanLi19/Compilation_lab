#include "objectcode.h"
extern InterCodes head_code;
struct stack_node* stack_sp;   //栈顶
struct stack_node* stack_fp;   //ebp
struct stack_node* stack_head; //栈底
struct Register _reg[32] = {0};//寄存器数组.保存寄存器编号,名字和状态

void init_reg()
{
	//寄存器的名字和状态初始化;
	for(int i=0;i<32;++i){
		_reg[i].state=0;
	}

	_reg[0].name="$zero";
	_reg[1].name="$at";
	_reg[2].name="$v0";
	_reg[3].name="$v1";

	_reg[4].name="$a0";
	_reg[5].name="$a1";
	_reg[6].name="$a2";
	_reg[7].name="$a3";

	_reg[8].name="$t0";
	_reg[9].name="$t1";
	_reg[10].name="$t2";
	_reg[11].name="$t3";

	_reg[12].name="$t4";
	_reg[13].name="$t5";
	_reg[14].name="$t6";
	_reg[15].name="$t7";

	_reg[16].name="$s0";
	_reg[17].name="$s1";
	_reg[18].name="$s2";
	_reg[19].name="$s3";

	_reg[20].name="$s4";
	_reg[21].name="$s5";
	_reg[22].name="$s6";
	_reg[23].name="$s7";

	_reg[24].name="$t8";
	_reg[25].name="$t9";

	_reg[26].name="$k0";
	_reg[27].name="$k1";

	_reg[28].name="$gp";
	_reg[29].name="$sp";

	_reg[30].name="$fp";
	_reg[31].name="$ra";

}

void init_data(FILE *fp)
{
	//数据段初始化;
	fprintf(fp,".data\n");
	fprintf(fp, "_prompt: .asciiz \"Enter an integer:\"\n_ret: .asciiz \"\\n\"\n.globl main\n");
}

void init_code(FILE*fp)
{
	//函数段初始化;read and write
	//read
	fprintf(fp, ".text\n");
	fprintf(fp, "_func_read:\n");
    fprintf(fp, "  li $v0, 4\n");
    fprintf(fp, "  la $a0, _prompt\n");
    fprintf(fp, "  syscall\n");
	fprintf(fp, "  li $v0, 5\n");
	fprintf(fp, "  syscall\n");
	fprintf(fp, "  jr $ra\n\n");
	//write
	fprintf(fp, "_func_write:\n");
	fprintf(fp, "  li $v0, 1\n");
	fprintf(fp, "  syscall\n");
	fprintf(fp, "  li $v0, 4\n");
	fprintf(fp, "  la $a0, _ret\n");
	fprintf(fp, "  syscall\n");
	fprintf(fp, "  move $v0, $0\n");
	fprintf(fp, "  jr $ra\n");
	stack_head=(struct stack_node*)(malloc(sizeof(struct stack_node)));
	stack_head->next=NULL;
	stack_fp=stack_head;
	stack_sp=stack_head;
	stack_head->no=-1;
}

int find_op_offset(Operand cur)
{
	struct stack_node* tempnode = stack_fp;
	int kind = cur->kind;
	int no = cur->var_no;
	int offset = -1;
	while(tempnode!=NULL)
	{
		if(tempnode->no==no && tempnode->kind==kind)
		{
			offset = tempnode->offset;
			break;
		}
		tempnode = tempnode->next;
	}
	if(offset==-1)
	{
		printf("kind:%d no:%d\n",kind,no);
		printf("bug in find_op_offset\n");
		assert(0);
	}
	return offset;
}

void load_reg(Operand op, int reg, FILE*fp)
{
	switch(op->kind)
    {
		case TEMP_OPERAND: //与V一样处理
		case VARIABLE_OPERAND:
        {
			int offset = find_op_offset(op);
			if(op->type == ADDRESS_AND)
            {
				//赋给reg当前变量的地址;
				fprintf(fp,"  la %s, %d($fp)\n",_reg[reg].name,-offset);
			}
			else if(op->type == VAL)
            {
				//赋给reg当前变量的值;
				fprintf(fp,"  lw %s, %d($fp)\n",_reg[reg].name,-offset);
			}
			else
			{
				assert(op->type == ADDRESS_STAR);
				//赋给reg当前变量作为地址的里面的值
				fprintf(fp,"  lw %s, %d($fp)\n",_reg[14].name,-offset); // 先把当前变量的值放到t6寄存器
				fprintf(fp,"  lw %s, 0(%s)\n",_reg[reg].name,_reg[14].name); //再把t6寄存器里面存放的值作为地址，取出里面的值存入reg
			}
			break;
		}
		case CONSTANT_OPERAND:
        {
			fprintf(fp,"  li %s, %d\n",_reg[reg].name,op->value);
			break;
		}
		case FUNCTION_OPERAND:
        {
			//赋给reg FUNCTION的地址;
			fprintf(fp,"  la %s, %s\n",_reg[reg].name,op->funcName);
			break;
		}
		case LABEL_OPERAND:
        {
            //赋给reg LABEL的地址;
			fprintf(fp,"  la %s, label%d\n",_reg[reg].name,op->var_no);
			break;
		}
		
	}
	return;
}

void save_reg(Operand op,int reg,FILE*fp)//把reg的值存到内存;
{
	switch(op->kind)
	{
		case TEMP_OPERAND:  //与V一样处理
		case VARIABLE_OPERAND:
		{
			int offset = find_op_offset(op);
			if(op->type == ADDRESS_AND)
			{
				printf("error! can't save address\n"); //&不应该出现在等号左边
				assert(0);
			}
			else if(op->type == VAL)
			{
				fprintf(fp,"  sw %s, %d($fp)\n",_reg[reg].name,-offset);
			}
			else
			{
				assert(op->type == ADDRESS_STAR);
				//把当前变量的值作为地址，将reg里面的值存到这个地址中
				fprintf(fp,"  lw %s, %d($fp)\n",_reg[14].name,-offset);//t6存储要存的目标地址;
				fprintf(fp,"  sw %s, 0(%s)\n",_reg[reg].name,_reg[14].name);//往目标地址存;
			}
			break;
		}
		default:
			printf("Impossible to get here in save_reg. Not t or v.\n");
			assert(0);
	}
}

void print_relop(char*relop,FILE*fp)
{
	if(strcmp(relop,"==")==0)
	{
		fprintf(fp,"  beq");
	}
	else if(strcmp(relop,"!=")==0)
	{
		fprintf(fp,"  bne");
	}
	else if(strcmp(relop,">")==0)
	{
		fprintf(fp,"  bgt");
	}
	else if(strcmp(relop,"<")==0)
	{
		fprintf(fp,"  blt");
	}
	else if(strcmp(relop,">=")==0)
	{
		fprintf(fp,"  bge");
	}
	else if(strcmp(relop,"<=")==0)
	{
		fprintf(fp,"  ble");
	}
}

void pop_op()
{
	//删除fp及之后的所有内容;
	struct stack_node* temp = stack_head;

	while(1)
	{
		if(temp==NULL)
		{
			printf("bug in pop_op!\n");
			assert(0);
			break;
		}
		if(temp == stack_fp)
		{
			temp->next=NULL;
			break;
		}
		if(temp->next == stack_fp)
		{
			temp->next=NULL;
			break;
		}
		temp = temp->next;
	}

	stack_fp=stack_head;
	stack_sp=temp;
}

void push_op(Operand op,int offset)
{
	struct stack_node* temp=(struct stack_node*)(malloc(sizeof(struct stack_node)));
	int kind = op->kind;
	if(kind != VARIABLE_OPERAND && kind != TEMP_OPERAND)
	{
		printf("wrong push kind:%d no:%d value:%d\n",kind,op->var_no,op->value);
		printf("can only push op of tempvar or variable\n");
		assert(0);
	}
	temp->kind = kind;
	temp->no = op->var_no;
	temp->offset = offset;
	temp->next = NULL;
	stack_sp->next = temp;
	stack_sp = temp;
}

int search_stack(Operand cur)//在stack中寻找变量来确定要不要push进入栈;找不到是0 找到是1
{

	struct stack_node* tempnode=stack_fp;
	int success=0;
	while(tempnode!=NULL)
	{
		if(tempnode->no == cur->var_no && tempnode->kind == cur->kind)
		{
			return 1;
		}
		tempnode=tempnode->next;
	}
	return 0;
}

void trans_one_code(InterCodes p, FILE* fp)
{
    switch(p->code.kind)
    {
        case FUNCTION_INTERCODE:
        {            
			pop_op();//更新stack;
			int frame_size = 0; //函数栈帧的size
			if(strcmp(p->code.u.para_1.result->funcName, "main") == 0)
				fprintf(fp, "\nmain:\n");
			else
				fprintf(fp, "\n_func_%s:\n",p->code.u.para_1.result->funcName);
			frame_size += 8;
			InterCodes temp = p->next;
			while(temp != head_code && temp->code.kind != FUNCTION_INTERCODE)
			{
				//先把PARAM push进入栈
				InterCodes p1 = temp;
				int param_offset = 0;
				while(p1->code.kind == PARAM)
				{
					Operand tempop1 = p1->code.u.para_1.result;
					if(search_stack(tempop1) == 0)
					{
						push_op(tempop1, param_offset);
						//printf("Kind: %d, varno: %d, offset: %d\n",tempop1->kind,tempop1->var_no,param_offset);
					}
					param_offset -= 4;
					p1 = p1->next;
				}
				temp = p1;
				switch(temp->code.kind)
				{
					case DEC:
					{
						Operand tempop1=temp->code.u.para_2.left;
						if(search_stack(tempop1) == 0)
						{
							frame_size += temp->code.u.para_2.right->value;
							push_op(tempop1, frame_size);
						}
						break;
					}
					case ARG:
					{
						Operand op = temp->code.u.para_1.result;
						if(op->kind==VARIABLE_OPERAND||op->kind==TEMP_OPERAND)
						{
							if(search_stack(op) == 0)
							{
								frame_size += 4;
								push_op(op, frame_size);
							}
						}
						break;
					}
					case ADD:
					case SUB:
					case MUL:
					case DIV:
					{
						Operand tempop1 = temp->code.u.para_3.op1;
						if(tempop1->kind==VARIABLE_OPERAND||tempop1->kind==TEMP_OPERAND)
						{
							if(search_stack(tempop1)==0)
							{
								frame_size += 4;
								push_op(tempop1, frame_size);
								
							}
						}
						Operand tempop2 = temp->code.u.para_3.op2;
						if(tempop2->kind==VARIABLE_OPERAND||tempop2->kind==TEMP_OPERAND)
						{
							if(search_stack(tempop2)==0)
							{
								frame_size += 4;
								push_op(tempop2,frame_size);
								
							}
						}
						Operand tempop3=temp->code.u.para_3.result;
						if(search_stack(tempop3)==0)
						{
							frame_size += 4;
							push_op(tempop3,frame_size);
						}
						break;
					}
					case READ:
					{
						Operand tempop1=temp->code.u.para_1.result;
						if(search_stack(tempop1) == 0)
						{
							frame_size += 4;
							push_op(tempop1, frame_size);
						}
						break;
					}
					case ASSIGN:
					{
						Operand tempop1 = temp->code.u.para_2.left;
						if(tempop1->kind==VARIABLE_OPERAND||tempop1->kind==TEMP_OPERAND)
						{
							if(search_stack(tempop1)==0)
							{
								frame_size += 4;
								push_op(tempop1, frame_size);	
							}
						}
						Operand tempop2 = temp->code.u.para_2.right;
						if(tempop2->kind==VARIABLE_OPERAND||tempop2->kind==TEMP_OPERAND)
						{
							if(search_stack(tempop2)==0)
							{
								frame_size += 4;
								push_op(tempop2, frame_size);	
							}
						}
						break;
					}
					case CALL:
					{
						Operand tempop1=temp->code.u.para_2.left;
						if(search_stack(tempop1)==0)
						{
							frame_size += 4;
							push_op(tempop1, frame_size);	
						}
						break;
					}
				}
				temp = temp->next;
			}
			fprintf(fp,"  addi $sp, $sp, -%d\n",frame_size);
			fprintf(fp,"  sw $ra, %d($sp)\n",frame_size-4);
			fprintf(fp,"  sw $fp, %d($sp)\n",frame_size-8);
			fprintf(fp,"  addi $fp, $sp, %d\n",frame_size);
        }
        case PARAM:
        {
			break;
		}
		case RETURN:
        {
			int temp_reg=8;
			load_reg(p->code.u.para_1.result,temp_reg,fp);
			fprintf(fp,"  move $v0, %s\n",_reg[temp_reg].name);//把返回值存在v0寄存器,这里和讲义中不同，讲义中的返回值也放在栈里
			//结果已经保存好了,现在开始修改返回地址和fp;
			fprintf(fp,"  move $sp, $fp\n");
			fprintf(fp,"  lw $ra, -4($fp)\n"); //函数返回地址在ebp-4
			fprintf(fp,"  lw $fp, -8($fp)\n"); //ebp旧值在ebp-8
			fprintf(fp,"  jr $ra\n");//跳转到返回地址
			break;
		}
		case LABEL:
        {
			fprintf(fp,"label%d:\n",p->code.u.para_1.result->var_no);
			break;
		}
		case GOTO:
        {
			fprintf(fp,"  j label%d\n",p->code.u.para_1.result->var_no);
			break;
		}
		case WRITE:
        {
			int temp_reg=8;//t0
			int write_reg=4;//a0 -- 存放函数参数
			load_reg(p->code.u.para_1.result,temp_reg,fp);
			fprintf(fp,"  move %s, %s\n",_reg[write_reg].name,_reg[temp_reg].name);
			fprintf(fp,"  jal _func_write\n");
			break;
		}
		case READ:
        {
			int temp_reg=8;//t0
			int read_reg=2;//v0
			fprintf(fp,"  jal _func_read\n");
			fprintf(fp,"  move %s, %s\n",_reg[temp_reg].name,_reg[read_reg].name);
			save_reg(p->code.u.para_1.result,temp_reg,fp);
			break;
		}
		case ARG:
        {
			//所有参数都存到栈上去
			fprintf(fp,"  addi $sp,$sp,-4\n");
			int arg_reg = 16;//s0
			load_reg(p->code.u.para_1.result, arg_reg, fp);
			fprintf(fp,"  sw %s, 0($sp)\n", _reg[arg_reg].name); //把s0里面的参数压到栈里去
			break;
		}
		case ASSIGN:
        {
			int left_reg=8;//t0
			int right_reg=9;//t1
			load_reg(p->code.u.para_2.left,left_reg,fp);
			load_reg(p->code.u.para_2.right,right_reg,fp);
			fprintf(fp,"  move %s, %s\n",_reg[left_reg].name,_reg[right_reg].name);
			save_reg(p->code.u.para_2.left,left_reg,fp);
			break;
		}
		case DEC:
        {
			break;
		}
		case CALL:
        {
			if(strcmp(p->code.u.para_2.right->funcName, "main") == 0)
				fprintf(fp,"  jal main\n");
			else
				fprintf(fp,"  jal _func_%s\n",p->code.u.para_2.right->funcName);
			//从v0--函数返回值里面取;存到对应的内存;		
			int return_reg=2;//v0
			save_reg(p->code.u.para_2.left,return_reg,fp);
			break;
		} 
		case ADD:
        {
            int result_reg=8;//t0
			int left_reg=9;//t1
			int right_reg=10;//t2;
			load_reg(p->code.u.para_3.op1, left_reg, fp);
			load_reg(p->code.u.para_3.op2, right_reg, fp);
			fprintf(fp,"  add %s, %s, %s\n",_reg[result_reg].name,_reg[left_reg].name,_reg[right_reg].name);
			save_reg(p->code.u.para_3.result,result_reg,fp);
			break;
        }
		case SUB:
        {
			int result_reg=8;//t0
			int left_reg=9;//t1
			int right_reg=10;//t2;
			load_reg(p->code.u.para_3.op1,left_reg,fp);
			load_reg(p->code.u.para_3.op2,right_reg,fp);
			fprintf(fp,"  sub %s, %s, %s\n",_reg[result_reg].name,_reg[left_reg].name,_reg[right_reg].name);
			save_reg(p->code.u.para_3.result,result_reg,fp);
			break;
		}
		case MUL:
        {
			int result_reg=8;//t0
			int left_reg=9;//t1
			int right_reg=10;//t2;
			load_reg(p->code.u.para_3.op1,left_reg,fp);
			load_reg(p->code.u.para_3.op2,right_reg,fp);
			fprintf(fp,"  mul %s, %s, %s\n",_reg[result_reg].name,_reg[left_reg].name,_reg[right_reg].name);
			save_reg(p->code.u.para_3.result,result_reg,fp);
			break;
		}
		case DIV:
        {
			int result_reg=8;//t0
			int left_reg=9;//t1
			int right_reg=10;//t2;
			load_reg(p->code.u.para_3.op1,left_reg,fp);
			load_reg(p->code.u.para_3.op2,right_reg,fp);
			fprintf(fp,"  div %s, %s\n",_reg[left_reg].name,_reg[right_reg].name);
			fprintf(fp,"  mflo %s\n",_reg[result_reg].name);
			save_reg(p->code.u.para_3.result,result_reg,fp);
			break;
		}
		case IFGOTO:
        {
			int op1_reg=8;//t0
			int op2_reg=9;//t1
			load_reg(p->code.u.para_4.op1,op1_reg,fp);
			load_reg(p->code.u.para_4.op2,op2_reg,fp);
			print_relop(p->code.u.para_4.relop,fp);
			fprintf(fp," %s, %s, label%d\n",_reg[op1_reg].name,_reg[op2_reg].name,p->code.u.para_4.op3->var_no); 
			break;
		}
    }
}

void trans_code(FILE *fp)
{
	InterCodes p = head_code->next;
	assert(p != NULL);
	while(p != head_code)
    {
        trans_one_code(p, fp);
        p = p -> next;
	}
}


void Objectcode_gen(FILE*fp)
{
	init_reg();
	init_data(fp);
	init_code(fp);
	trans_code(fp);
}