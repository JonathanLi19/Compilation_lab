#include <stdio.h>
#include "optimize.h"
int main(int argc, char** argv)
{
    if (argc <= 1) return 1;
    FILE* file_1 = fopen(argv[1], "r");
    if (!file_1)
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
    IR_optimize(file_1, file_2);

    fclose(file_1);
    fclose(file_2);
    return 0;
}