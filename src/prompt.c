#include "../include/libs.h"


extern char* username;
extern char hostname[HOSTNAME_LEN];
extern char homePath[PATH_LEN];
extern char currentPath[PATH_LEN];

void promptSetup()
{
    username = getenv("USER");
    gethostname(hostname, HOSTNAME_LEN);
    getcwd(homePath, PATH_LEN);
    getcwd(currentPath, PATH_LEN);
}

void showPrompt(int status)
{
   
    int withinHome = 0;
   getcwd(currentPath, PATH_LEN);
   if(strstr(currentPath, homePath) == currentPath)
   {
       withinHome = 1;
   }

    if(status == 0)
        printf(BOLD ANSI_COLOR_GREEN ":-)" ANSI_COLOR_RESET NOTBOLD);
    else
        printf(BOLD ANSI_COLOR_RED ":-(" ANSI_COLOR_RESET NOTBOLD);
    
    if(!withinHome)
    printf("[%s@%s %s]$ ", username, hostname, currentPath);
    else
    printf("[%s@%s ~%s]$ ", username, hostname, currentPath+strlen(homePath));

}