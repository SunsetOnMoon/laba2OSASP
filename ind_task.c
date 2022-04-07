#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <libgen.h>

typedef struct {
    char path[PATH_MAX];
    char name[FILENAME_MAX];
    int  size;
} fileType;

char utilName[FILENAME_MAX];
fileType *fileList;
int fileListLen = 0;

int (*CompareFiles)(fileType file1, fileType file2);
int CompareFileNames(fileType file1, fileType file2);
int CompareFileSizes(fileType file1, fileType file2);
char* AddSlash(char* path);
int DirectoryTraversal(const char *dirName);
void FilesSort(int sortKind);

int main(int argc, char const *argv[]) {
    strcpy(utilName, (char*) basename(argv[0]));

    if (argc != 4) {
        fprintf(stderr, "%s: Wrong number of arguments\n", utilName);
        return -1;
    }

    char destPath[PATH_MAX];
    realpath(argv[3], destPath);
    AddSlash(destPath);

    char sortKind = atoi(argv[2]);
    if (sortKind != 1 && sortKind != 2) {
        fprintf(stderr,"%s: Wrong sort option (Must be: 1 or 2)\n", utilName);
        return -1;
    }

    char srcDir[PATH_MAX];
    strcpy(srcDir, argv[1]);
    DirectoryTraversal(AddSlash(srcDir));

    FilesSort(sortKind);

    if (mkdir(destPath, 0777) == -1 && errno != EEXIST) {
        fprintf(stderr, "%s: %s: %s\n", utilName, destPath, strerror(errno));
        return -1;
    }

    int i, j;
    for(i = 0; i < fileListLen; i++) {
        j = i;
        while (j++ < fileListLen &&!strcmp(fileList[i].name, fileList[j].name)) {
            char addIndex[5];
            sprintf(addIndex," (%d)", j-i);
            strcat(fileList[j].name, addIndex);
        }

        char destFile[PATH_MAX + 1];
        strcpy(destFile, destPath);
        strcat(destFile, fileList[i].name);
        if (symlink(fileList[i].path, destFile) == -1) {
            fprintf(stderr, "%s: %s: %s\n", utilName, destFile, strerror(errno));
        };
    }
    return 0;
}

int CompareFileNames(fileType file1, fileType file2) {
    return (strcmp(file1.name, file2.name) > 0);
}

int CompareFileSizes(fileType file1, fileType file2) {
    return ((file1.size - file2.size) < 0);
}

char* AddSlash(char* path) {
    if (path[strlen(path) - 1] != '/')
        strcat(path, "/");
    return path;
}

int DirectoryTraversal(const char *dirName) {
    DIR *directory;

    if (!(directory = opendir(dirName))) {
        fprintf(stderr, "%s: %s: %s\n", utilName, dirName, strerror(errno));
        return -1;
    }

    struct dirent *dirItem;
    while((dirItem = readdir(directory)) != NULL) {
        char nextItem[PATH_MAX];
        strcpy(nextItem, dirName);
        strcat(nextItem, dirItem->d_name);
        if(strcmp(".", dirItem->d_name) == 0 ||
           strcmp("..", dirItem->d_name) == 0)
            continue;
        struct stat buffer;
        lstat(nextItem, &buffer);
        if(S_ISDIR(buffer.st_mode)) {
            DirectoryTraversal(AddSlash(nextItem));
        } else if(S_ISREG(buffer.st_mode)) {
            fileType tempFile;
            strcpy(tempFile.name, dirItem->d_name);
            strcpy(tempFile.path, nextItem);
            tempFile.size = (int) buffer.st_size;
            fileList = realloc(fileList, (++fileListLen)*sizeof(fileType));
            fileList[fileListLen - 1] = tempFile;
        }
    }
    closedir(directory);
}

void FilesSort(int sortKind){
    if (sortKind == 1)
        CompareFiles = CompareFileNames;
    else
        CompareFiles = CompareFileSizes;

    int i, j;
    for (i = 0; i < fileListLen; i++) {
        for (j = 0; j < fileListLen - i - 1; j++) {
            if (CompareFiles(fileList[j], fileList[j+1])) {
                fileType tempFile = fileList[j];
                fileList[j] = fileList[j+1];
                fileList[j+1] = tempFile;
            }
        }
    }
}

