#include <stdio.h>
#include "objectcode.h"
extern int syserr, myerr, no_error;
extern struct Node* Root;
extern int depth_;
int main(int argc, char** argv)
{
    if (argc <= 1) return 1;
    FILE* f = fopen(argv[1], "r");
    if (!f)
    {
        perror(argv[1]);
        return 1;
    }
    FILE *file_2 = fopen(argv[2],"w+");
    if(!file_2)
    {
        perror(argv[2]);
        return 1;
    }
    yyrestart(f);
    yyparse();
    if (no_error)
    {
        Program(Root);
        depth_ = __INT_MAX__;
        translate_start(Root, file_2);
        Objectcode_gen(file_2);
    }
    return 0;
}