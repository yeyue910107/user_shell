#include "command.h"
#define MAX_PIPE 10
int pid[MAX_PIPE];
int pipe_fd[2];
void executePipe(int pipeNum);
void doPipe(int i, int pipeNum);
