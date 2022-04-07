#include <stdio.h>
#include <dirent.h>

int main(int argc, char* argv[]) {
    DIR *currentDir, *rootDir;
    struct dirent *dirInfo;

    currentDir = opendir(".");
    if (currentDir == NULL)
    {
        printf("Can't open this directory.\n");
        return 1;
    }
    printf("Current directory:\n");
    while ((dirInfo = readdir(currentDir)) != NULL)
    {
        printf("%s\n", dirInfo->d_name);
    }

    if (closedir(currentDir) != 0)
    {
        printf("Can't close directory.\n");
        return 1;
    }

    rootDir = opendir("/");
    if (rootDir == NULL)
    {
        printf("Can't open this directory.\n");
        return 1;
    }

    printf("\n\nRoot directory:\n");
    while ((dirInfo = readdir(rootDir)) != NULL)
    {
        printf("%s\n", dirInfo->d_name);
    }

    if (closedir(rootDir) != 0)
    {
        printf("Can't close directory.\n");
        return 1;
    }
    return 0;
}
