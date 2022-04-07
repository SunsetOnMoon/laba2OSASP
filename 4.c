#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    FILE *srcFile;
    int symb, counter = 0, N = atoi(argv[2]);

    if (argc > 3)
    {
        printf("To many parameters.\n");
        return 1;
    }
    else if (argc < 3)
    {
        printf("Not enough parameters.Maybe you remember to add file name or number of lines (N).\n");
        return 1;
    }

    srcFile = fopen(argv[1], "r");
    if (srcFile == NULL)
    {
        printf("Can't open file.\n");
        return 1;
    }
    if (N != 0)
    {
        while (!feof(srcFile))
        {
            symb = getc(srcFile);
            if (symb != -1)
                putc(symb, stdout);
            if (symb == '\n')
                counter++;
            if (counter == N)
            {
                getc(stdin);
                counter = 0;
            }
        }
    }
    else
    {
        while (!feof(srcFile))
        {
            symb = getc(srcFile);
            if (symb != -1)
                putc(symb, stdout);
        }
    }

    if (fclose(srcFile) != 0)
    {
        printf("Can't close file.\n");
        return 1;
    }
    return 0;
}
