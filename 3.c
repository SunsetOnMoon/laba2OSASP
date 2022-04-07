#include <stdio.h>

int main(int argc, char* argv[]) {
    FILE *destFile;
    int symb;

    if (argc > 2)
    {
        printf("To many parameters.\n");
        return 1;
    }
    else if (argc < 2)
    {
        printf("Not enough parameters.Maybe you remember to add file name.\n");
        return 1;
    }

    destFile = fopen(argv[1], "w");
    if (destFile == NULL)
    {
        printf("Can't open file.\n");
        return 1;
    }

    while ((symb = getc(stdin)) != 6)
    {
        putc(symb, destFile);
    }

    if (fclose(destFile) != 0)
    {
        printf("Can't close file.\n");
        return 1;
    }
    return 0;
}
