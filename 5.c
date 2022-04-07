#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
    FILE *srcFile, *destFile;
    int symb;
    struct stat buf;

    if (argc > 3)
    {
        printf("To many parameters.\n");
        return 1;
    }
    else if (argc < 3)
    {
        printf("Not enough parameters.Maybe you remember to add source or destination file name.\n");
        return 1;
    }

    srcFile = fopen(argv[1], "r");
    destFile = fopen(argv[2], "w");
    if ((srcFile == NULL) || (destFile == NULL))
    {
        printf("Can't open files.\n");
        return 1;
    }

    while (!feof(srcFile))
    {
        symb = getc(srcFile);
        if (symb != -1)
            putc(symb, destFile);
    }
    stat(argv[1], &buf);
    chmod(argv[2], buf.st_mode);
    if ((fclose(srcFile) || fclose(destFile)) != 0)
    {
        printf("Can't close files.\n");
        return 1;
    }
    return 0;
}
