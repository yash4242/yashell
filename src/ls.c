#include "../include/libs.h"
#define R printf("r")
#define W printf("w")
#define X printf("x")
#define NAH printf("-")
extern char currentPath[PATH_LEN];
char filePath[PATH_LEN];

void printFileType(char *filePath)
{
    struct stat fileStat;
    stat(filePath, &fileStat);

    if (S_ISDIR(fileStat.st_mode))
    {
        printf("d");
    }
    else if (S_ISCHR(fileStat.st_mode))
    {
        printf("c");
    }
    else if (S_ISBLK(fileStat.st_mode))
    {
        printf("b");
    }
    else if (S_ISREG(fileStat.st_mode))
    {
        printf("-");
    }
    else if (S_ISFIFO(fileStat.st_mode))
    {
        printf("p");
    }
    else if (S_ISLNK(fileStat.st_mode))
    {
        printf("l");
    }
    else if (S_ISSOCK(fileStat.st_mode))
    {
        printf("s");
    }
    else
    {
        printf("-");
    }
}

void printFilePerms(char *filePath)
{
    struct stat fileStat;
    stat(filePath, &fileStat);

    (fileStat.st_mode & S_IRUSR) ? R : NAH;
    (fileStat.st_mode & S_IWUSR) ? W : NAH;
    (fileStat.st_mode & S_IXUSR) ? X : NAH;
    (fileStat.st_mode & S_IRGRP) ? R : NAH;
    (fileStat.st_mode & S_IWGRP) ? W : NAH;
    (fileStat.st_mode & S_IXGRP) ? X : NAH;
    (fileStat.st_mode & S_IROTH) ? R : NAH;
    (fileStat.st_mode & S_IWOTH) ? W : NAH;
    (fileStat.st_mode & S_IXOTH) ? X : NAH;

    printf(" ");

    printf("%-3ld ", fileStat.st_nlink);

    printf("%-12s ", getpwuid(fileStat.st_uid)->pw_name);

    printf("%-12s ", getgrgid(fileStat.st_gid)->gr_name);

    printf("%7ld ", fileStat.st_size);

    char datemodified[14];

    time_t t;
    time(&t);
    if (t - fileStat.st_mtim.tv_sec > 15000000)
    {strftime(datemodified, 14, "%b %2d  %Y", localtime(&(fileStat.st_mtim.tv_sec)));}
    else
        strftime(datemodified, 14, "%b %2d %H:%M", localtime(&(fileStat.st_mtim.tv_sec)));

    printf("%s ", datemodified);
}

exec_status_t builtin_ls(char *input)
{
    int return_value = 0;
    getcwd(currentPath, PATH_LEN);

    //figuring out the args and flags

    int a = 0;
    int a_arg_index = -1;
    int l_arg_index = -1;
    int l = 0;

    struct stat fileStat;

    char *token;
    char *save;

    char argv[12][PATH_LEN];
    for (int i = 0; i < MAX_NO_ARGS; i++)
    {
        strcpy(argv[i], "");
    }

    int argc = 0;

    if (input == NULL)
    {
        strcpy(argv[0], currentPath);
        input = currentPath;
    }

    token = strtok_r(input, " \n", &save);
    while (token != NULL)
    {
        strcpy(argv[argc], token);

        token = strtok_r(NULL, " \n", &save);
        argc++;
    }

    for (int i = 0; i < argc; i++)
    {
        char *ret = strstr(argv[i], "-l");
        if (ret != NULL && ret == argv[i])
        {
            l = 1;
            l_arg_index = i;
        }

        ret = strstr(argv[i], "-a");
        if (ret != NULL && ret == argv[i])
        {
            a = 1;
            a_arg_index = i;
        }

        ret = strstr(argv[i], "-la");
        if (ret != NULL && ret == argv[i])
        {
            l = 1;
            a = 1;
            l_arg_index = i;
            a_arg_index = i;
        }
        ret = strstr(argv[i], "-al");

        if (ret != NULL && ret == argv[i])
        {
            l = 1;
            a = 1;
            l_arg_index = i;
            a_arg_index = i;
        }
    }

    if (l && a && argc <= 2)
    {
        strcpy(argv[argc], currentPath);
        argc++;
    }

    if (!l && a && argc == 1)
    {
        strcpy(argv[argc], currentPath);
        argc++;
    }

    if (l && !a && argc == 1)
    {
        strcpy(argv[argc], currentPath);
        argc++;
    }

    if (!l && !a && argc == 0)
    {
        strcpy(argv[argc], currentPath);
        argc++;
    }

    for (int i = 0; i < argc; i++)
    {
        if (i == a_arg_index)
        {
            continue;
        }
        if (i == l_arg_index)
        {
            continue;
        }

        struct stat tempstat;
        if (stat(argv[i], &tempstat) != 0)
        {
            perror("file not presence error:");
            continue;
        }

        struct dirent *d;
        DIR *dh = opendir(argv[i]);
        if (dh == NULL)
        {
            printFileType(argv[i]);
            printFilePerms(argv[i]);
            printf("%s\n", argv[i]);
            continue;
        }

        if (l)
        {
            stat(argv[i], &fileStat);
            printf("total %ld\n", fileStat.st_blocks);
        }
        while ((d = readdir(dh)) != NULL)
        {
            if (!a && d->d_name[0] == '.')
            {
                continue;
            }
            if (l)
            {
                strcpy(filePath, argv[i]);
                strcat(filePath, "/");
                strcat(filePath, d->d_name);

                //now filePath has the actual path of the file
                printFileType(filePath);
                printFilePerms(filePath);
            }

            printf("%s\n", d->d_name);
        }
    }

    return return_value;
}