#include "optimize.h"
Node SSA_head = NULL; //存放所有的SSA形式的中间代码,是一个双向链表

int V[1024] = { 0 }; //记录从三地址码转化为SSA的过程中vi变量对应的t_id
int T[1024] = { 0 }; //记录从三地址码转化为SSA的过程中ti变量对应的t_id
int cur_id = 0; //记录当前SSA转换过程中记录到t_id
struct T_state states[1023] = {0}; //记录当前的SSA中每个t_id的状态,用于消除公共子表达式
struct exp_ sub_exp[1023] = {0}; //存放子表达式的表
int sub_exp_num = 0;//当前存了几个子表达式
int use[1023] = {0}; //当前每个变量的使用次数，用于消除死代码，自底向上遍历
Node newNode(char* buf)
{
    Node p = (Node)(malloc(sizeof(struct Node_)));
    strcpy(p->exp, buf);
    p->next = NULL;
    p->prev = NULL;
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
    p->prev = p1;
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
    p->next->prev = p1;
    return;
}

char t_name[1023] = {"t"};
char star_name[1023] = {"*"};
char and_name[1023] = {"&"};
char hashtag_name[1023] = {"#"};
char* trans_variable(char* name, int flag) //把变量变为t_id; 如果是常量就不变; flag表示是否要把cur_id加1
{
    assert(name != NULL);
    assert(name[0] == 'v' || name[0] == 't' || name[0] == '#' || name[0] == '*' || name[0] == '&');
    strcpy(t_name, "t");
    strcpy(star_name, "*");
    strcpy(and_name, "&");
    if (name[0] == '#') //常量
    {
        return name;
    }
    else if(name[0] == '*')
    {
        char operand[1023] = {""};
        strcpy(operand, trans_variable(name+1, flag));
        strcat(star_name, operand);
        return star_name;
    }
    else if(name[0] == '&')
    {
        char operand[1023] = {""};
        strcpy(operand, trans_variable(name+1, flag));
        strcat(and_name, operand);
        return and_name;
    }
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
    else if(buf[0] == 'D' && buf[1] == 'E') //DEC
    {
        int space_pos = -1;
        for(int i=4;i<strlen(buf);i++)
        {
            if(buf[i] == ' ')
            {
                space_pos = i;
                break;
            }
        }
        assert(space_pos != -1);
        //operand从4--space_pos-1
        char operand[1023] = {""};
        strncpy(operand, buf+4, (space_pos-1)-4+1);
        strcpy(operand, trans_variable(operand,1));
        strcat(operand, buf+space_pos);
        char dec_[1023] = {"DEC "};
        strcat(dec_, operand);
        Node p = newNode(dec_);
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
        assert(left[0] != '&'); //等式的左边不能有取地址符号
        //对left操作
        strcpy(left, trans_variable(left, 1));
        //对right操作
        int op_pos = -1;
        for (int i = 1; i < strlen(right); i++)//防止*号在第一位置被识别成乘号
        {
            if (right[i] == '+' || right[i] == '-' || right[i] == '*' || right[i] == '/')
            {
                op_pos = i;
                break;
            }
        }
        assert(op_pos != 0);
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

char* trans_T(char* operand) //将t_id转化为t_referid, 用于消除公共子表达式
{
    strcpy(star_name, "*");
    strcpy(and_name, "&");
    assert(operand[0] == '#' || operand[0] == 't' || operand[0] == '*' || operand[0] == '&');
    if(operand[0] == '#')// 返回常数
    {
        //do nothing
        return operand;
    }
    else if(operand[0] == '*')
    {
        char copy[1023] = {""};
        strcpy(copy, trans_T(operand+1));
        strcat(star_name, copy);
        return star_name;
    }
    else if(operand[0] == '&')
    {
        char copy[1023] = {""};
        strcpy(copy, trans_T(operand+1));
        strcat(and_name, copy);
        return and_name;
    }
    else
    {
        int id = atoi(operand + 1);
        if(states[id].is_dead == 0)
        {
            // do nothing
            return operand;
        }
        else
        {
            int refer_id = states[id].refer_id;
            char num[10];
            sprintf(num, "%d", refer_id);
            strcpy(t_name, "t");
            strcat(t_name, num);
            strcpy(operand, t_name);
            return operand;
        }
    }
    assert(0);
    return NULL;
}

void remove_common_subexp() //消除公共子表达式
{
    Node p = SSA_head;
    while(p != NULL)
    {
        if(p->exp[0] == 'F')//FUCTION
        {
            //do nothing
        }
        else if(p->exp[0] == 'D')//DEC
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
            strcpy(operand, trans_T(operand));
            char return_[1023] = {"RETURN "};
            strcat(return_, operand);
            strcpy(p->exp, return_);
        }
        else if(p->exp[0] == 'W') //WRITE
        {
            //operand从6--strlen(exp)-1
            char operand[1023] = {""};
            strcpy(operand, p->exp+6);
            strcpy(operand, trans_T(operand));
            char write_[1023] = {"WRITE "};
            strcat(write_, operand);
            strcpy(p->exp, write_);
        }
        else //赋值语句
        {
            int colon_pos = -1; //冒号的位置
            for (int i = 0; i < strlen(p->exp); i++)
            {
                if (p->exp[i] == ':')
                {
                    colon_pos = i;
                    break;
                }
            }
            assert(colon_pos != -1);
            //从0--colon_pos-2是左边的变量；从colon_pos+3--strlen(exp)-1是右边的表达式
            char left[1023] = { "" };
            char right[1023] = { "" };
            strncpy(left, p->exp, colon_pos - 1);
            strncpy(right, p->exp + colon_pos + 3, (strlen(p->exp) - 1) - (colon_pos + 3) + 1);
            assert(left[0] != '&');
            //对left不用操作, 提取出operand_id
            int left_id = -1;
            if(left[0] == '*')
                left_id = atoi(left + 2);
            else
                left_id = atoi(left + 1);
            //对right检查是否是公共子表达式
            int found = 0; //是否找到重复的子表达式
            for(int i=0;i<sub_exp_num;i++)
            {
                if(strcmp(sub_exp[i].exp, right) == 0) //是公共子表达式
                {
                    found = 1;
                    //更新states
                    states[left_id].is_dead = 1;
                    states[left_id].refer_id = sub_exp[i].refer_id;
                    //删除这行SSA中间代码
                    delete_node(p);
                    break;
                }
            }
            if(found == 0)
            {
                
                //把这个表达式信息添加到子表达式表中
                strcpy(sub_exp[sub_exp_num].exp, right);
                sub_exp[sub_exp_num].refer_id = left_id;
                sub_exp_num++;
                //检查right,看是否有变量需要替换成refer_id
                int op_pos = -1;
                for (int i = 1; i < strlen(right); i++)
                {
                    if (right[i] == '+' || right[i] == '-' || right[i] == '*' || right[i] == '/')
                    {
                        op_pos = i;
                        break;
                    }
                }
                assert(op_pos != 0);
                if (op_pos == -1) //说明是一个赋值语句
                {
                    //整个right就是右边的operand
                    strcpy(right, trans_T(right));
                }
                else
                {
                    //从0--op_pos-2是operand1;从op_pos+2--strlen(right)-1是operand2
                    char operand1[1023] = { "" };
                    char operand2[1023] = { "" };
                    strncpy(operand1, right, op_pos - 1);
                    strncpy(operand2, right + op_pos + 2, (strlen(right) - 1) - (op_pos + 2) + 1);
                    strcpy(operand1, trans_T(operand1));
                    strcpy(operand2, trans_T(operand2));
                    char op = right[op_pos];
                    char Z[100] = "   ";
                    Z[1] = op;
                    strcat(Z, operand2);
                    strcat(operand1, Z);
                    strcpy(right, operand1);
                }
                //现在right被更新完毕，更新p节点
                char inter[1023] = { " := " };
                strcat(inter, right);
                strcat(left, inter);
                strcpy(p->exp, left);
            }
            else
            {
                // do nothing
            }
        }
        assert(p != NULL);
        p = p->next;
    }
    return;
}

void add_use(char* operand) //将当前operand使用次数加1
{
    assert(operand[0] == '#' || operand[0] == 't' || operand[0] == '*' || operand[0] == '&');
    if(operand[0] == '#')// 常数
    {
        //do nothing
        return;
    }
    else if(operand[0] == '*' || operand[0] == '&')
    {
        add_use(operand+1);
        return;
    }
    else
    {
        int id = atoi(operand + 1);
        use[id]++;
        return;
    }
    assert(0);
    return;
}

void remove_dead_code()
{
    Node p = SSA_head;
    while(p->next != NULL)
    {
        p = p->next;
    }
    while(p != NULL) //自下向上遍历
    {
        if(p->exp[0] == 'F')//FUCTION
        {
            //do nothing
        }
        else if(p->exp[0] == 'D')//DEC
        {
            int space_pos = -1;
            for(int i=4;i<strlen(p->exp);i++)
            {
                if(p->exp[i] == ' ')
                {
                    space_pos = i;
                    break;
                }
            }
            assert(space_pos != -1);
            //operand从4--space_pos-1
            char operand[1023] = {""};
            strncpy(operand, p->exp+4, (space_pos-1)-4+1);
            int left_id = atoi(operand + 1);
            if(use[left_id] == 0) // 后续没有被使用过，说明这是死代码，应该消除
            {
                delete_node(p);
            }
            else
            {
                //do nothing
            }
        }
        else if(p->exp[0] == 'R' && p->exp[1] == 'E' && p->exp[2] == 'A') //READ
        {
            //do nothing
        }
        else if(p->exp[0] == 'R' && p->exp[1] == 'E' && p->exp[2] == 'T') //RETURN
        {
            //使用了当前operand
            //operand从7--strlen(exp)-1
            char operand[1023] = {""};
            strcpy(operand, p->exp+7);
            add_use(operand);
        }
        else if(p->exp[0] == 'W') //WRITE
        {
            //使用了当前operand
            //operand从6--strlen(exp)-1
            char operand[1023] = {""};
            strcpy(operand, p->exp+6);
            add_use(operand);
        }
        else //赋值语句
        {
            int colon_pos = -1; //冒号的位置
            for (int i = 0; i < strlen(p->exp); i++)
            {
                if (p->exp[i] == ':')
                {
                    colon_pos = i;
                    break;
                }
            }
            assert(colon_pos != -1);
            //从0--colon_pos-2是左边的变量；从colon_pos+3--strlen(exp)-1是右边的表达式
            char left[1023] = { "" };
            char right[1023] = { "" };
            strncpy(left, p->exp, colon_pos - 1);
            strncpy(right, p->exp + colon_pos + 3, (strlen(p->exp) - 1) - (colon_pos + 3) + 1);

            assert(left[0] != '&');
            //对left不用操作, 提取出operand_id,看看是否在之前遍历的时候被使用过
            int left_id = -1;
            if(left[0] == '*')
                left_id = atoi(left + 2);
            else
                left_id = atoi(left + 1);
            
            if(left[0] == '*') //这个时候该left变量一定被使用了，因为被取值了
            {
                //do nothing
            }
            if(left[0] != '*' && use[left_id] == 0) // 后续没有被使用过，说明这是死代码，应该消除
            {
                delete_node(p);
            }
            else
            {
                //对right的operand添加use
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
                    add_use(right);
                }
                else
                {
                    //从0--op_pos-2是operand1;从op_pos+2--strlen(right)-1是operand2
                    char operand1[1023] = { "" };
                    char operand2[1023] = { "" };
                    strncpy(operand1, right, op_pos - 1);
                    strncpy(operand2, right + op_pos + 2, (strlen(right) - 1) - (op_pos + 2) + 1);
                    add_use(operand1);
                    add_use(operand2);
                }
            }
        }
        p = p->prev;
    }
    return;
}

char* trans2const(char* operand)//将（可以转换的）操作数转换为常数的形式，用于常量折叠
{
    assert(operand[0] == '#' || operand[0] == 't' || operand[0] == '*' || operand[0] == '&');
    if(operand[0] == '*' || operand[0] == '&' || operand[0] == '#')
        return operand;
    else
    {
        int id = atoi(operand + 1);
        if(states[id].is_const == 0)
        {
            // do nothing
            return operand;
        }
        else
        {
            int val = states[id].const_val;
            char num[10];
            sprintf(num, "%d", val);
            strcpy(hashtag_name, "#");
            strcat(hashtag_name, num);
            strcpy(operand, hashtag_name);
            return operand;
        }
    }
    assert(0);
    return NULL;
}

void const_folding()
{
    Node p = SSA_head;
    while(p != NULL)
    {
        if(p->exp[0] == 'F')//FUCTION
        {
            //do nothing
        }
        else if(p->exp[0] == 'D')//DEC
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
            strcpy(operand, trans2const(operand));
            char return_[1023] = {"RETURN "};
            strcat(return_, operand);
            strcpy(p->exp, return_);
        }
        else if(p->exp[0] == 'W') //WRITE
        {
            //operand从6--strlen(exp)-1
            char operand[1023] = {""};
            strcpy(operand, p->exp+6);
            strcpy(operand, trans2const(operand));
            char write_[1023] = {"WRITE "};
            strcat(write_, operand);
            strcpy(p->exp, write_);
        }
        else //赋值语句
        {
            int colon_pos = -1; //冒号的位置
            for (int i = 0; i < strlen(p->exp); i++)
            {
                if (p->exp[i] == ':')
                {
                    colon_pos = i;
                    break;
                }
            }
            assert(colon_pos != -1);
            //从0--colon_pos-2是左边的变量；从colon_pos+3--strlen(exp)-1是右边的表达式
            char left[1023] = { "" };
            char right[1023] = { "" };
            strncpy(left, p->exp, colon_pos - 1);
            strncpy(right, p->exp + colon_pos + 3, (strlen(p->exp) - 1) - (colon_pos + 3) + 1);
            assert(left[0] != '&');
            //对left不用操作, 提取出operand_id
            int left_id = -1;
            if(left[0] == '*')
                left_id = atoi(left + 2);
            else
                left_id = atoi(left + 1);

            //检查right,看是否有变量可以替换为常量，以及如果替换之后检查是否可以进一步代数恒等式计算
            int op_pos = -1;
            for (int i = 1; i < strlen(right); i++)
            {
                if (right[i] == '+' || right[i] == '-' || right[i] == '*' || right[i] == '/')
                {
                    op_pos = i;
                    break;
                }
            }
            assert(op_pos != 0);
            if (op_pos == -1) //说明是一个赋值语句
            {
                //整个right就是右边的operand
                strcpy(right, trans2const(right));
                if(right[0] == '#') //查看right是否是常数
                {
                    if(left[0] == '*')
                    {
                        //do nothing
                    }
                    else
                    {
                        //这个时候left对应的变量就是常数了，更新states
                        states[left_id].is_const = 1;
                        states[left_id].const_val = atoi(right+1);
                    }
                }
            }
            else
            {
                //从0--op_pos-2是operand1;从op_pos+2--strlen(right)-1是operand2
                char operand1[1023] = { "" };
                char operand2[1023] = { "" };
                strncpy(operand1, right, op_pos - 1);
                strncpy(operand2, right + op_pos + 2, (strlen(right) - 1) - (op_pos + 2) + 1);
                strcpy(operand1, trans2const(operand1));
                strcpy(operand2, trans2const(operand2));
                char op = right[op_pos];
                if(operand1[0] == '#' && operand2[0] == '#')
                {
                    //考虑代数恒等式合并
                    int val1 = atoi(operand1+1);
                    int val2 = atoi(operand2+1);
                    int res = -1;
                    if(op == '+')
                    {
                        res = val1 + val2;
                    }
                    else if(op == '-')
                    {
                        res = val1 - val2;
                    }
                    else if(op == '*')
                    {
                        res = val1 * val2;
                    }
                    else if(op == '/')
                    {
                        res = val1 / val2;
                    }
                    strcpy(hashtag_name, "#");
                    char num[10];
                    sprintf(num, "%d", res);
                    strcat(hashtag_name, num);
                    strcpy(right, hashtag_name);
                    //这个时候left变量也是常数，所以也要更新states
                    states[left_id].is_const = 1;
                    states[left_id].const_val = atoi(right+1);
                }
                else
                {
                    char Z[100] = "   ";
                    Z[1] = op;
                    strcat(Z, operand2);
                    strcat(operand1, Z);
                    strcpy(right, operand1);
                }
            }
            //现在right被更新完毕，更新p节点
            char inter[1023] = { " := " };
            strcat(inter, right);
            strcat(left, inter);
            strcpy(p->exp, left);
        }
        p = p->next;
    }
}

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
    remove_common_subexp();//消除公共子表达式
    remove_dead_code(); //消除死代码
    const_folding(); //常量折叠
    printSSA(file2); //仅作调试
    return;
}