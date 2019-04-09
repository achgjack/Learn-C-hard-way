/*
read all the files in "ex26dir" and search the specified words
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>

#define MAX_LEN 1024

int grep_or(char *target[], char *line, int num)
{
    int i;

    for (i = 0; i < num; i++)
    {
        if (strstr(line, target[i]) != NULL)
        {
            return 1;
        }
    }

    return 0;  // not found
}

int grep_and(char *target[], char *line, int num)
{
    int i;

    for (i = 0; i < num; i++)
    {
        if (strstr(line, target[i]) == NULL)
        {
            return 0;
        }
    }

    return 1;
}

void usage()
{
    printf("Usage: ex26 [-o] s1 s2\n");
}

int main(int argc, char* argv[])
{
    DIR *dirp; 
    struct dirent *dp;
    FILE *fp;
    char buf[MAX_LEN];
    
    int orflag = 0;
    int i;

    if (argc < 2)
    {
        usage();
        return -1;
    }

    // parse the argv, optional -o
    if (strcmp("-o", argv[1]) == 0)
    {
        orflag =  1;
    }

    if (orflag)
    {
        i = 2;
        if (argc <= 2)
        {
            usage();
            return -1;
        }
    }
    else
    {
        i = 1;
    }

    printf("or flag: %d\n", orflag);
     
    if ((dirp = opendir("./ex26dir")) == NULL) // open the dir
    {
        fprintf(stderr, "can't open the directory\n");
        return -1;
    }
    
    while ((dp = readdir(dirp)) != NULL) {
        if ((strcmp(dp->d_name, ".") == 0) || (strcmp(dp->d_name, "..") == 0))
            continue; // skip self and parent
        
        if ((fp = fopen(dp->d_name, "r")) == NULL)
        {
            fprintf(stderr, "can't open %s\n", dp->d_name);
            return -1;
        }
        
        while (fgets(buf, sizeof(buf), fp))
        {
            if (orflag)
            {
                if(grep_or(argv+2, buf, argc-2))
                {
                    printf("%s: %s\n", dp->d_name, buf);
                }
            }
            else
            {
                if (grep_and(argv+1, buf, argc-1))
                {
                    printf("%s: %s\n", dp->d_name, buf);
                }
            }
            //if (strstr(buf, "struct") != NULL)
                //printf("%s: %s\n", dp->d_name, buf);
        }
        fclose(fp);
    }
    
    (void) closedir(dirp); // close the dir

    return 0;
}


