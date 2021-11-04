#include "../include/libs.h"

extern char currentPath[PATH_LEN];
extern char homePath[PATH_LEN];
// extern char oldDir[PATH_LEN];

exec_status_t builtin_cd(char* path)
{
    // getcwd(oldDir, PATH_LEN);
    if(path == NULL || strcmp(path,"~") == 0)
    {
        chdir(homePath); 
        return 0;
    }
    // if(!strcmp(path, "-"))
    // {
    //     chdir(oldDir);
    //     return 0;
    // }
    if(chdir(path) != 0)
    {perror("Error doing cd"); return 1;}
    return 0;
}

exec_status_t builtin_echo(char* args)
{
    char* save;
    char* token = strtok_r(args, " \n", &save);
    while(token != NULL)
    {
        printf("%s ", token);
        token = strtok_r(NULL, " \n", &save);
    }
    printf("\n");
    return 0;
}

exec_status_t builtin_pwd(char* extraargs)
{
    if(extraargs != NULL)
    {printf("too many arguments\n"); return 1;}
    getcwd(currentPath, PATH_LEN);
    printf("%s\n", currentPath);
    return 0;
}