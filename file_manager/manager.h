#ifndef MANAGER_H_INCLUDED
#define MANAGER_H_INCLUDED

#include <stdio.h>

#include "../struct/sdisk.h"

char * getDirectoryPath(char * filename)
{
    char * cpyPath = (char *)malloc(strlen(filename));
    strcpy(cpyPath, filename);
    char * temp = (char *)malloc(strlen(filename));
    memset(temp, 0, strlen(filename));
    char * fileDirectory = (char *)malloc(strlen(filename));
    memset(fileDirectory, 0, strlen(fileDirectory));
    strcpy(fileDirectory, "/");

    temp = strtok(cpyPath, "/");
    while (*temp)
    {
        strcat(fileDirectory, temp);
        strcat(fileDirectory, "/");
        temp = strtok(NULL, "/");
        // .png | .jpg | .jpeg | .disk | .dsk
        if (temp != NULL &&
        ((temp[strlen(temp) - 4] == '.') ||
        (temp[strlen(temp) - 5] == '.')))
            break;
    }

    return fileDirectory;
}

int createFolder(char * filename)
{
    char * fileDirectory = getDirectoryPath(filename);
    size_t size = strlen(fileDirectory) + sizeof(char) * 10;
    char * cmd = (char *)malloc(size);
    memset(cmd, 0, size);
    strcpy(cmd, "mkdir -p ");
    strcat(cmd, fileDirectory);

    return (system(cmd) == 0) ? 1 : 0;
}

int createDisk(char * filename, int size)
{
    FILE * file;
    
    if ((file = fopen(filename, "wb")) == NULL)
    {
        perror("createDisk\n");
        return 0;
    }

    char buffer[KILOBYTE];
    memset(buffer, '\0', KILOBYTE);
    
    for(int i = 0; i < size / KILOBYTE; i++)
    {
        fwrite(buffer, sizeof(buffer), 1, file);
        fflush(file);
    }
    fclose(file);
    return 1;    
}

int updateMBR(char * filename, MBR * data)
{
    FILE * file;
    if ((file = fopen(filename, "rb+")) == NULL)
    {
        perror("updateMBR\n");
        return 0;
    }

    fwrite(data, sizeof(MBR), 1, file);
    fclose(file);
    return 1;
}

int deleteDisk(char * filename)
{

}

int updateEBR(char * filename, EBR * data, int start)
{

}

int deleteEBR(char * filename, int start)
{

}

#endif // MANAGER_H_INCLUDED