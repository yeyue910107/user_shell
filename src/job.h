#include "command.h"

JOB* insertJob(pid_t pid);		//添加作业
JOB* findJob(pid_t pid);		//根据进程号寻找作业
void fgCmd(int pid);			//在前台执行进程号为pid的进程
void bgCmd(int pid);			//在后台执行进程号为pid的进程
void doJobs();				//执行jobs命令

