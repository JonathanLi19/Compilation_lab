#include <stdio.h>
extern int syserr, myerr, no_error;
extern struct Node* Root;
void tree_search(struct Node* root,int depth);
int main(int argc, char** argv)
{
    if (argc <= 1) return 1;
    FILE* f = fopen(argv[1], "r");
    if (!f)
    {
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
    yyparse();
    if (no_error)
    {
        tree_search(Root,0);
    }
    else if (syserr > myerr)
    {
        printf("\n");
    }
    return 0;
}