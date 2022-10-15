%{
 #include <stdio.h>
 #include <stdarg.h>
 int no_error = 1;
 int syserr = 0;
 int myerr = 0;
 struct Node *add_bisonnode(char* Name,int column);
 void build_tree(struct Node *parent,int num_args,...);
 void tree_search(struct Node* root,int depth);
 extern struct Node* Root;
%}
%locations

/* declared types */
%union 
{
  struct Node* node;
}

/* declared tokens */
%token <node> INT
%token <node> OCT HEX
%token <node> FLOAT
%token <node> SEMI COMMA LC RC STRUCT RETURN IF WHILE TYPE ID
%token <node> LOWER_THAN_ELSE
%token <node> ELSE
%token <node> ASSIGNOP
%token <node>  OR
%token <node>  AND
%token <node>  RELOP
%token <node>  PLUS MINUS
%token <node>  STAR DIV
%token <node>  NOT
%token <node>  LP RP LB RB DOT
/* declared non-terminals */
%type <node> Program ExtDefList ExtDef Specifier ExtDecList FunDec CompSt VarDec StructSpecifier OptTag DefList Tag VarList ParamDec StmtList Exp Stmt Def DecList Dec Args
%start Program
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%right  ASSIGNOP
%left   OR
%left   AND
%left   RELOP
%left   PLUS MINUS
%left   STAR DIV
%right  NOT
%left   LP RP LB RB DOT
%%
// High-Level Definitions
Program : ExtDefList {$$=add_bisonnode("Program",@$.first_line); build_tree($$,1,$1); Root=$$;}
    ;
ExtDefList :    {$$=NULL;}    
    | ExtDef ExtDefList  {$$=add_bisonnode("ExtDefList",@$.first_line);build_tree($$,2,$1,$2);}
    ;
ExtDef : Specifier ExtDecList SEMI  {$$=add_bisonnode("ExtDef",@$.first_line);build_tree($$,3,$1,$2,$3);}
    | Specifier error               {yyerror("Missing \";\" at this or last line"); }
    | Specifier SEMI                {$$=add_bisonnode("ExtDef",@$.first_line);build_tree($$,2,$1,$2);}
    | error SEMI                    {yyerror("Missing or Wrong Specifier"); }
    | Specifier FunDec CompSt       {$$=add_bisonnode("ExtDef",@$.first_line);build_tree($$,3,$1,$2,$3);}
    ;
ExtDecList : VarDec                 {$$=add_bisonnode("ExtDecList",@$.first_line);build_tree($$,1,$1);}
    | VarDec COMMA ExtDecList       {$$=add_bisonnode("ExtDecList",@$.first_line);build_tree($$,3,$1,$2,$3); }
    | VarDec error ExtDecList   {yyerror("Missing \",\""); }
    | VarDec error              {yyerror("Missing \";\" at this or last line"); } 
    ;

// Specifiers
Specifier : TYPE                    {$$=add_bisonnode("Specifier",@$.first_line);build_tree($$,1,$1);}
    | StructSpecifier               {$$=add_bisonnode("Specifier",@$.first_line);build_tree($$,1,$1);}
    ;
StructSpecifier : STRUCT OptTag LC DefList RC   {$$=add_bisonnode("StructSpecifier",@$.first_line);build_tree($$,5,$1,$2,$3,$4,$5);}
    | STRUCT Tag                                {$$=add_bisonnode("StructSpecifier",@$.first_line);build_tree($$,2,$1,$2); }
    | STRUCT OptTag LC error RC                 {yyerror("Wrong struct definition"); }                           
    ;
OptTag :    {$$=NULL;}
    | ID    {$$=add_bisonnode("OptTag",@$.first_line);build_tree($$,1,$1);}
    ;
Tag : ID    {$$=add_bisonnode("Tag",@$.first_line);build_tree($$,1,$1);}
    ;

// Declarators
VarDec : ID                 {$$=add_bisonnode("VarDec",@$.first_line);build_tree($$,1,$1);}
    | VarDec LB INT RB      {$$=add_bisonnode("VarDec",@$.first_line);build_tree($$,4,$1,$2,$3,$4);}
    | VarDec LB error RB    {yyerror("Missing \"]\""); }
    | VarDec LB error       {yyerror("Missing \"]\""); }   
    ;
FunDec : ID LP VarList RP   {$$=add_bisonnode("FunDec",@$.first_line);build_tree($$,4,$1,$2,$3,$4);}
    | ID LP RP              {$$=add_bisonnode("FunDec",@$.first_line);build_tree($$,3,$1,$2,$3); }
    | ID LP error RP        {yyerror("Wrong argument(s)"); }
    | error LP VarList RP   {yyerror("Wrong function name"); }
    ;
VarList : ParamDec COMMA VarList    {$$=add_bisonnode("VarList",@$.first_line);build_tree($$,3,$1,$2,$3);}
    | ParamDec                      {$$=add_bisonnode("VarList",@$.first_line);build_tree($$,1,$1);} 
    ;
ParamDec : Specifier VarDec         {$$=add_bisonnode("ParamDec",@$.first_line);build_tree($$,2,$1,$2);}
    ;

// Statements
CompSt : LC DefList StmtList RC    {$$=add_bisonnode("CompSt",@$.first_line);build_tree($$,4,$1,$2,$3,$4);}
    | LC error RC                  {yyerror("Wrong statement(s)"); }
    ;
StmtList :                          {$$=NULL;}
    | Stmt StmtList                 {$$=add_bisonnode("StmtList",@$.first_line);build_tree($$,2,$1,$2);}
    ;
Stmt : Exp SEMI                     {$$=add_bisonnode("Stmt",@$.first_line);build_tree($$,2,$1,$2);}
    | Exp error SEMI                {yyerror("Missing \";\" at this or last line"); }
    | CompSt                        {$$=add_bisonnode("Stmt",@$.first_line);build_tree($$,1,$1);}
    | RETURN Exp SEMI               {$$=add_bisonnode("Stmt",@$.first_line);build_tree($$,3,$1,$2,$3);}
    | IF LP Exp RP Stmt  %prec LOWER_THAN_ELSE {$$=add_bisonnode("Stmt",@$.first_line);build_tree($$,5,$1,$2,$3,$4,$5);}          
    | IF LP Exp RP Stmt ELSE Stmt   {$$=add_bisonnode("Stmt",@$.first_line);build_tree($$,7,$1,$2,$3,$4,$5,$6,$7);}
    | WHILE LP Exp RP Stmt          {$$=add_bisonnode("Stmt",@$.first_line);build_tree($$,5,$1,$2,$3,$4,$5);}
    | error SEMI                    {yyerror("Wrong expression or Definition after statement"); }
    | Exp error                     {yyerror("Missing \";\" at this or last line"); }
    | RETURN Exp error              {yyerror("Missing \";\" at this or last line"); }
    | RETURN error SEMI             {yyerror("Wrong return value");}         
    ;

// Local Definitions
DefList :                       {$$=NULL;}
    | Def DefList               {$$=add_bisonnode("DefList",@$.first_line);build_tree($$,2,$1,$2);}
    ;
Def : Specifier DecList SEMI    {$$=add_bisonnode("Def",@$.first_line);build_tree($$,3,$1,$2,$3);}
    | Specifier error SEMI      {yyerror("Missing ID"); }
    ;
DecList : Dec           {$$=add_bisonnode("DecList",@$.first_line);build_tree($$,1,$1);}
    | Dec COMMA DecList {$$=add_bisonnode("DecList",@$.first_line);build_tree($$,3,$1,$2,$3);}    
    | Dec error DecList {yyerror("Missing \",\"");}
    | Dec error         {yyerror("Missing \";\" at this or last line"); }  
    ;
Dec : VarDec            { $$=add_bisonnode("Dec",@$.first_line);build_tree($$,1,$1);}
    | VarDec ASSIGNOP Exp  { $$=add_bisonnode("Dec",@$.first_line);build_tree($$,3,$1,$2,$3);}
    | VarDec ASSIGNOP error   {yyerror("Wrong expression"); }
    ;

// Expressions
Exp : Exp ASSIGNOP Exp      {$$=add_bisonnode("Exp",@$.first_line);build_tree($$,3,$1,$2,$3);}
    | Exp AND Exp           {$$=add_bisonnode("Exp",@$.first_line);build_tree($$,3,$1,$2,$3);}
    | Exp OR Exp            {$$=add_bisonnode("Exp",@$.first_line);build_tree($$,3,$1,$2,$3);}
    | Exp RELOP Exp         { $$=add_bisonnode("Exp",@$.first_line);build_tree($$,3,$1,$2,$3);}
    | Exp PLUS Exp           { $$=add_bisonnode("Exp",@$.first_line);build_tree($$,3,$1,$2,$3);}
    | Exp MINUS Exp         { $$=add_bisonnode("Exp",@$.first_line);build_tree($$,3,$1,$2,$3);}
    | Exp STAR Exp         { $$=add_bisonnode("Exp",@$.first_line);build_tree($$,3,$1,$2,$3);}
    | Exp DIV Exp           { $$=add_bisonnode("Exp",@$.first_line);build_tree($$,3,$1,$2,$3);}
    | LP Exp RP             { $$=add_bisonnode("Exp",@$.first_line);build_tree($$,3,$1,$2,$3);}
    | MINUS Exp             {$$=add_bisonnode("Exp",@$.first_line);build_tree($$,2,$1,$2);}
    | NOT Exp               {$$=add_bisonnode("Exp",@$.first_line);build_tree($$,2,$1,$2);}
    | ID LP Args RP         {$$=add_bisonnode("Exp",@$.first_line);build_tree($$,4,$1,$2,$3,$4);}
    | ID LP RP              {$$=add_bisonnode("Exp",@$.first_line);build_tree($$,3,$1,$2,$3);}
    | Exp LB Exp RB         {$$=add_bisonnode("Exp",@$.first_line);build_tree($$,4,$1,$2,$3,$4);}
    | Exp DOT ID            {$$=add_bisonnode("Exp",@$.first_line);build_tree($$,3,$1,$2,$3);}
    | ID                    {$$=add_bisonnode("Exp",@$.first_line);build_tree($$,1,$1);}
    | INT                   {$$=add_bisonnode("Exp",@$.first_line);build_tree($$,1,$1);}
    | FLOAT                 {$$=add_bisonnode("Exp",@$.first_line);build_tree($$,1,$1);}
    | OCT                   {$$=add_bisonnode("Exp",@$.first_line);build_tree($$,1,$1);}
    | HEX                   {$$=add_bisonnode("Exp",@$.first_line);build_tree($$,1,$1);}
    | Exp ASSIGNOP error    {yyerror("Wrong expression"); }
    | Exp AND error         {yyerror("Wrong expression"); }
    | Exp OR error          {yyerror("Wrong expression"); }
    | Exp RELOP error       {yyerror("Wrong expression"); }
    | Exp PLUS error         {yyerror("Wrong expression"); }
    | Exp MINUS error         {yyerror("Wrong expression"); }
    | Exp STAR error         {yyerror("Wrong expression"); }
    | Exp DIV error         {yyerror("Wrong expression"); }
    | LP error RP           {yyerror("Wrong expression"); }
    | MINUS error             {yyerror("Wrong expression"); }
    | NOT error             {yyerror("Wrong expression"); }
    | ID LP error RP        {yyerror("Wrong argument(s)"); }
    | ID LP error SEMI      {yyerror("Missing \")\"");}
    | Exp LB error RB       {yyerror("Missing \"]\"");}
    | Exp LB error SEMI     {yyerror("Missing \"]\"");}          
    ;
Args : Exp COMMA Args       {$$=add_bisonnode("Args",@$.first_line);build_tree($$,3,$1,$2,$3);}
    | Exp                   {$$=add_bisonnode("Args",@$.first_line);build_tree($$,1,$1);}
    ;

%%
#include "lex.yy.c"
void yyerror(const char *msg)
{
    no_error = 0;
    if (msg == "syntax error")
    {
        printf("Error type B at Line %d: %s.", yylineno, msg);
        syserr++;
    }
    else
    {
        printf(" %s.\n", msg);
        myerr++;
    }
}
struct Node *add_bisonnode(char* Name,int line_num)
{
    struct Node * bison_node=(struct Node *)malloc(sizeof(struct Node));
    bison_node->child = NULL;
    bison_node->next_sib = NULL;
    strcpy(bison_node->name, Name);
    bison_node->unit = 1;
    bison_node->type = OTHERS;
    bison_node->line_num = line_num;
    return bison_node;
}

void build_tree(struct Node *parent,int num_args,...)
{
    va_list ap;
    va_start(ap,num_args);
    struct Node * temp = NULL;
    for(int i=0 ;i<num_args;i++)
    {
        if(i == 0)
        {
            temp = va_arg(ap,struct Node*);
            parent->child = temp;
        }
        else
        {
            temp->next_sib=va_arg(ap,struct Node*);
            if(temp->next_sib != NULL)
            {
                temp = temp->next_sib;
            }
        }
    }
}
void tree_search(struct Node* root,int depth)
{
    if(root==NULL)
    {
        return;
    }
    for(int i=0;i<depth;i++)
    {
        printf("  ");
    }
    printf("%s",root->name);
    if(root->unit==1)  //bison
    {
        printf(" (%d)",root->line_num);
    }
    else if(root->unit==0)   //lex
    {
        if(root->type==LEX_INT)
        {
            printf(": %d",root->int_content);
        } 
        else if(root->type==LEX_FLOAT)
        {
            printf(": %f",root->float_content);
        }
        else if(root->type==LEX_ID||root->type==LEX_TYPE)
        {
            printf(": %s",root->string_content);
        }
    }
    printf("\n");
    tree_search(root->child,depth+1);
    tree_search(root->next_sib,depth);
}