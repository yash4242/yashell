#ifndef _MACROS_
#define _MACROS_

#define clear() printf("\033[H\033[J")

#define HOSTNAME_LEN 64
#define PATH_LEN 256
#define MAX_CHILDREN 16
#define MAX_NO_ARGS 12
#define COMMAND_LEN 256
#define MAX_PIPED 5

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define BOLD               "\033[1m"
#define NOTBOLD            "\033[0m"

#endif