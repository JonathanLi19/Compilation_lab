#include "optimize.h"
Node SSA_head = NULL; //存放所有的SSA形式的中间代码

int V[1024] = { 0 }; //记录从三地址码转化为SSA的过程中vi变量对应的t_id
int T[1024] = { 0 }; //记录从三地址码转化为SSA的过程中ti变量对应的t_id
int cur_id = 0; //记录当前SSA转换过程中记录到t_id
Node newNode(char* buf)
{
    Node p = (Node)(malloc(sizeof(struct Node_)));
    strcpy(p->exp, buf);
    p->next = NULL;
    return p;
}

void insert(Node p) //将一句SSA代码插入
{
    Node p1 = SSA_head;
    if (p1 == NULL)
    {
        SSA_head = p;
        return;
    }
    while (p1->next != NULL)
    {
        p1 = p1->next;
    }
    p1->next = p;
    p->next = NULL;
    return;
}

void delete_node(Node p) //将一句SSA代码删除
{
    Node p1 = SSA_head;
    if (p1 == p)
    {
        SSA_head = NULL;
        return;
    }
    while (p1->next != p)
    {
        p1 = p1->next;
    }
    p1->next = p->next;
    //free(p); 不确定会不会出问题
    return;
}
char t_name[1023] = {"t"};
char* trans_variable(char* name, int flag) //把变量变为t_id; 如果是常量就不变; flag表示是否要把cur_id加1
{
    assert(name != NULL);
    assert(name[0] == 'v' || name[0] == 't' || name[0] == '#');
    if (name[0] == '#') //常量
    {
        return name;
    }
    strcpy(t_name, "t");
    if(flag == 1)
    {
        cur_id++;
        char num[10];
        sprintf(num, "%d", cur_id);
        strcat(t_name, num);
        //更新V T数组信息
        if (name[0] == 'v') //vi
        {
            int id = atoi(name + 1);
            V[id] = cur_id;
        }
        else //ti
        {
            int id = atoi(name + 1);
            T[id] = cur_id;
        }
        return t_name;
    }
    else
    {
        if (name[0] == 'v') //vi
        {
            int id = atoi(name + 1);
            char num[10];
            sprintf(num, "%d", V[id]);
            strcat(t_name, num);
        }
        else //ti
        {
            int id = atoi(name + 1);
            char num[10];
            sprintf(num, "%d", T[id]);
            strcat(t_name, num);
        }
        return t_name;
    }
    assert(0);
    return NULL;
}

void Trans2SSA(char* buf)
{
    if (buf[0] == 'F' && buf[1] == 'U')//Fuction
    {
        char copy[1023] = {""};
        strncpy(copy,buf,strlen(buf));
        Node p = newNode(copy);
        insert(p);
    }
    else if (buf[0] == 'R' && buf[1] == 'E') //READ or Return
    {
        if(buf[2] == 'A') //READ
        {
            //从5--strlen(buf)-1是operand
            char operand[1023] = {""};
            strncpy(operand, buf+5,(strlen(buf)-1)-5+1);
            strcpy(operand, trans_variable(operand, 1));
            char read_[1023] = {"READ "};
            strcat(read_, operand);
            Node p = newNode(read_);
            insert(p);
        }
        else if(buf[2] == 'T') //RETURN
        {
            //从7--strlen(buf)-1是operand
            char operand[1023] = {""};
            strncpy(operand, buf+7,(strlen(buf)-1)-7+1);
            strcpy(operand, trans_variable(operand, 0));
            char return_[1023] = {"RETURN "};
            strcat(return_, operand);
            Node p = newNode(return_);
            insert(p);
        }
    }
    else if (buf[0] == 'W' && buf[1] == 'R') //WRITE
    {
        //从6--strlen(buf)-1是operand
        char operand[1023] = {""};
        strncpy(operand, buf+6,(strlen(buf)-1)-6+1);
        strcpy(operand, trans_variable(operand, 0));
        char write_[1023] = {"WRITE "};
        strcat(write_, operand);
        Node p = newNode(write_);
        insert(p);
    }
    else //等式
    {
        int colon_pos = -1; //冒号的位置
        for (int i = 0; i < strlen(buf); i++)
        {
            if (buf[i] == ':')
            {
                colon_pos = i;
                break;
            }
        }
        assert(colon_pos != -1);
        //从0--colon_pos-2是左边的变量；从colon_pos+3--strlen(buf)-1是右边的表达式
        char left[1023] = { "" };
        char right[1023] = { "" };
        strncpy(left, buf, colon_pos - 1);
        strncpy(right, buf + colon_pos + 3, (strlen(buf) - 1) - (colon_pos + 3) + 1);
        //对left操作
        strcpy(left, trans_variable(left, 1));
        //对right操作
        int op_pos = -1;
        for (int i = 0; i < strlen(right); i++)
        {
            if (right[i] == '+' || right[i] == '-' || right[i] == '*' || right[i] == '/')
            {
                op_pos = i;
                break;
            }
        }
        if (op_pos == -1) //说明是一个赋值语句
        {
            //整个right就是右边的operand
            strcpy(right, trans_variable(right, 0));
        }
        else
        {
            //从0--op_pos-2是operand1;从op_pos+2--strlen(right)-1是operand2
            char operand1[1023] = { "" };
            char operand2[1023] = { "" };
            strncpy(operand1, right, op_pos - 1);
            strncpy(operand2, right + op_pos + 2, (strlen(right) - 1) - (op_pos + 2) + 1);
            strcpy(operand1, trans_variable(operand1, 0));
            strcpy(operand2, trans_variable(operand2, 0));
            char op = right[op_pos];
            char Z[100] = "   ";
            Z[1] = op;
            strcat(Z, operand2);
            strcat(operand1, Z);
            strcpy(right, operand1);
        }
        char inter[100] = { " := " };
        strcat(inter, right);
        strcat(left, inter);
        Node p = newNode(left);
        insert(p);
        assert(p->next == NULL);
    }
    return;
}

void printSSA(FILE* file)
{
    Node p = SSA_head;
    while (p != NULL)
    {
        fputs(p->exp, file);
        if(p->next != NULL)
            fputs("\n", file);
        p = p->next;
    }
    return;
}

/*void remove_common_subexp() //消除公共子表达式
{
    Node p = SSA_head;
    while(p != NULL)
    {
        if(p->exp[0] == 'F')//FUCTION
        {
            //do nothing
        }
        else if(p->exp[0] == 'R' && p->exp[1] == 'E' && p->exp[2] == 'A') //READ
        {
            //do nothing
        }
        else if(p->exp[0] == 'R' && p->exp[1] == 'E' && p->exp[2] == 'T') //RETURN
        {
            //operand从7--strlen(exp)-1
            char operand[1023] = {""};
            strcpy(operand, p->exp+7);
            assert(operand[0] == '#' || operand[0] == 't' || operand[0] == 'v');
            if(operand[0] == '#')// 返回常数
            {
                //do nothing
            }
            else if(operand[0] == 't')
            {

            }
            else
            {

            }
        }
        else if(p->exp[0] == 'W') //WRITE
        {

        }
        else //赋值语句
        {

        }
        p = p->next;
    }
}*/

void IR_optimize(FILE* file1, FILE* file2)
{
    char buf[1023];
    while (fgets(buf, sizeof(buf), file1) != NULL)
    {
        char copy[1023] = {""};
        if(buf[strlen(buf)-1] == '\n')
        {
            strncpy(copy, buf, strlen(buf)-1);
        }
        else
        {
            strncpy(copy, buf, strlen(buf));
        }
        Trans2SSA(copy);
    }
    //remove_common_subexp();
    printSSA(file2); //仅作调试
    return;
}