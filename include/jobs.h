#ifndef _JOBS_
#define _JOBS_

int cmpfunc(const void * a, const void * b);

exec_status_t jobs(char*);

exec_status_t sig(char *);

exec_status_t bg(char *);

exec_status_t my_fg(char *);

#endif