%{
#include "syntax.tab.h"
int yycolumn = 1;
#define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;
extern int no_error;
typedef enum {
    LEX_INT=0,
    LEX_FLOAT=1,
    LEX_ID=2,
    LEX_TYPE=3,
    OTHERS     //syntax unit or other LEX type
}NODE_TYPE;
struct Node
{              // multi-way tree
    struct Node *child;
    struct Node* next_sib;
    char name[32];//节点名称
    union
    {
        int int_content;
        float float_content;
        char string_content[32];
    };//包含的内容
    int unit;//unit type: 0--lexical,1--syntax;
    int line_num;//表示行数;
    NODE_TYPE type;
};
struct Node* Root;
void add_lexnode(char * Name,NODE_TYPE Type);
%}
%option yylineno
digit       [0-9]
digits      [0-9]+
letter      [a-zA-Z_]
INT         0|[1-9]{digit}*
OCT         0[0-7]+
HEX         0[xX][0-9a-fA-F]+
ID	        {letter}+({letter}|{digit})*
RELOP 	    >|<|>=|<=|==|!=
TYPE        int|float
FLOAT       {digits}\.{digits}|(\.{digits}|{digits}\.|{digits}\.{digits}|{digits})([eE][+-]?{digits})
MULTI_LINE_COMMENT \/\*[\/]*([^\*\/][\/]*|[\*]|[^\*\/])*\*\/
%%
[ \t]+      {}
"//"        {char c = input(); while(c!='\n'){ c = input();}}
\n          {yycolumn = 1;}
{MULTI_LINE_COMMENT} {}
{OCT}       {add_lexnode("INT",LEX_INT);sscanf(yytext,"%o",&yylval.node->int_content);return OCT;}
{HEX}       {add_lexnode("INT",LEX_INT);sscanf(yytext,"%x",&yylval.node->int_content);return HEX;}
{INT}	    {add_lexnode("INT",LEX_INT);sscanf(yytext,"%d",&yylval.node->int_content);return INT;}
{FLOAT}	    {add_lexnode("FLOAT",LEX_FLOAT);sscanf(yytext,"%f",&yylval.node->float_content);return FLOAT;}
";"	        {add_lexnode("SEMI",OTHERS);return SEMI;}
","	        {add_lexnode("COMMA",OTHERS);return COMMA;}
{RELOP}	    {add_lexnode("RELOP",OTHERS);strcpy(yylval.node->string_content,yytext);return RELOP;}
{TYPE}      {add_lexnode("TYPE",LEX_TYPE);strcpy(yylval.node->string_content,yytext);return TYPE;}
"="	        {add_lexnode("ASSIGNOP",OTHERS);return ASSIGNOP;}
"+"	        {add_lexnode("PLUS",OTHERS);return PLUS;}
"-"	        {add_lexnode("MINUS",OTHERS);return MINUS;}
"*"	        {add_lexnode("STAR",OTHERS);return STAR;}
"/"	        {add_lexnode("DIV",OTHERS);return DIV;}
"&&"	    {add_lexnode("AND",OTHERS);return AND;}
"||"	    {add_lexnode("OR",OTHERS);return OR;}
"."	        {add_lexnode("DOT",OTHERS);return DOT;}
"!"	        {add_lexnode("NOT",OTHERS);return NOT;}
"("	        {add_lexnode("LP",OTHERS);return LP;}
")"	        {add_lexnode("RP",OTHERS);return RP;}
"["	        {add_lexnode("LB",OTHERS);return LB;}
"]"	        {add_lexnode("RB",OTHERS);return RB;}
"{"	        {add_lexnode("LC",OTHERS);return LC;}
"}"	        {add_lexnode("RC",OTHERS);return RC;}
"struct"    {add_lexnode("STRUCT",OTHERS);return STRUCT;}
"return"    {add_lexnode("RETURN",OTHERS);return RETURN;}
"if"	    {add_lexnode("IF",OTHERS);return IF;}
"else"	    {add_lexnode("ELSE",OTHERS);return ELSE;}
"while"	    {add_lexnode("WHILE",OTHERS);return WHILE;}
"int"	    {add_lexnode("TYPE",OTHERS);strcpy(yylval.node->string_content,yytext);return TYPE;}
"float"	    {add_lexnode("TYPE",OTHERS);strcpy(yylval.node->string_content,yytext);return TYPE;}
{ID}	    {add_lexnode("ID",LEX_ID);strcpy(yylval.node->string_content,yytext);return ID;}
.           {printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);no_error = 0;}
%%
void add_lexnode(char * Name,NODE_TYPE Type)
{
    yylval.node=(struct Node*)malloc(sizeof(struct Node));
    yylval.node->child = NULL;
    yylval.node->next_sib = NULL;
    yylval.node->line_num = yylineno;
    strcpy(yylval.node->name, Name);
    yylval.node->unit = 0;
    yylval.node->type = Type;
}