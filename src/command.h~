#include "config.h"
#define MAX 10

pid_t fgPid;				//前台进程号
int sigchld, cont;			//sigchld和continue信号
JOB *head;				//历史命令头指针
char *cmdPath[MAX];			//初始化从user-sh.conf中获取的外部命令路径
HISTORY history;			//历史命令记录
CMD cmds[MAX];				//输入的命令

int parseCmd();				//解析简单命令
void executeCmd();			//执行命令
void executeCommonCmd(CMD cmd);		//执行普通命令
char* getCommonCmd(char *cmd);		//获取完整命令
void doCd(CMD cmd);			//执行cd命令
void doFg(CMD cmd);			//执行fg命令
void doBg(CMD cmd);			//执行bg命令
void inputRedirect(CMD cmd);		//输入重定向
void outputRedirect(CMD cmd);		//输出重定向

