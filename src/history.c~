#include "history.h"

//添加历史记录
void addHistory(char *cmd){
	if(history.end == -1){
		history.end = 0;
		history.cmds[history.end] = (char *)malloc(strlen(cmd) + 1);
		strcpy(history.cmds[history.end],cmd);
		return ;
	}
	history.end = (history.end+1)%HISTORY_NUM;
	history.cmds[history.end] = (char *)malloc(strlen(cmd) + 1);
	strcpy(history.cmds[history.end],cmd);
	
	if(history.end == history.begin){
		history.begin = (history.begin + 1) % HISTORY_NUM;
	}
}

//执行历史记录命令
void doHistory() {
	int i;
	i = history.begin;

	if (history.end == -1) {
		printf("No command has been executed.\n");
		return ;
	}
	
	do {
		printf("%s\n", history.cmds[i]);
		i = (i + 1) % HISTORY_NUM;
	}
	while (i != ((history.end + 1) % HISTORY_NUM));
}

