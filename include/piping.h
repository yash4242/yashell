#ifndef _PIPING_
#define _PIPING_

void reset_streams();

exec_status_t do_this(int, int, char*);

exec_status_t pipe_parser(char*);

exec_status_t spawn_proc(int in, int out, char* command);

#endif