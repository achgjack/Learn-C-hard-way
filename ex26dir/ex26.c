/*
read all the *.txt file and search the specified words
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include<dirent.h>

const int MAX_LEN = 1024;
const int MAX_CNT = 1024;
char wordbuffer[MAX_CNT][MAX_LEN]; // store the target words
int targetnum = 0;

int grep_or(char *target[], char *line)
{
    int i;

    for (i = 0; i < targetnum; i++)
    {
        if (strstr(target[i], line) != NULL)
        {
            return 1;
        }
    }

    return 0;  // not found
}

int grep_and(char *target[], char *line)
{
    int i;

    for (i = 0; i < targetnum; i++)
    {
        if (strstr(target[i], line) == NULL)
        {
            return 0;
        }
    }

    return 1;
}

int main(int argc, char* argv[])
{
    DIR *dirp; 
    struct dirent *dp;
    dirp = opendir("./exer23dir"); // open the dir
    while ((dp = readdir(dirp)) != NULL) {
        printf("%s\n", dp->d_name);
    }      
    (void) closedir(dirp); // close the dir

    return 0;
}


