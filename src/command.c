/*
command.c
命令解析和执行
*/
#include "command.h"

#define MAX 10
#define LEN 50
#define MAX_PIPE 10

//解析简单命令
int parseCmd() {
	int i = 0, j = 0, k = 0, p = 0, i0, len = strlen(inputWords), hasInput = 0, hasOutput = 0, flag = 0;
	char *args[MAX], *inputFile, *outputFile, buf[LEN], *addr;
	
	cmds[0].command = cmds[0].inFile = cmds[0].outFile = NULL; cmds[0].isBg = 0;
	//滤过空白符
	while (i < len && (inputWords[i] == ' ' || inputWords[i] == '\t')) {
		i++;
	}

	args[j] = (char *)malloc(LEN + 1);
	addr = args[j];
	
	//解析命令
	while (i < len) {
		//处理管道命令
		if (inputWords[i] == '|') {
			//写入命令及参数
			cmds[p].command = (char *)malloc(strlen(args[0]) + 1);
			strcpy(cmds[p].command, args[0]);
			for (i0 = 0; i0 < j; i0++) {
				cmds[p].params[i0] = (char *)malloc(strlen(args[i0]) + 1);
				strcpy(cmds[p].params[i0], args[i0]);
			}
			cmds[p].params[i0] = NULL;
			//获取输入重定向文件
			if (hasInput) {
				cmds[p].inFile = (char *)malloc(strlen(inputFile) + 1);
				strcpy(cmds[p].inFile, inputFile);
				hasInput = 0;
			}
			//获取输出重定向文件
			if (hasOutput) {
				cmds[p].outFile = (char *)malloc(strlen(outputFile) + 1);
				strcpy(cmds[p].outFile, outputFile);
				hasOutput = 0;
			}
			i++;
			p++;
			j = 0;
			addr = args[j];
			cmds[p].command = cmds[p].inFile = cmds[p].outFile = NULL; cmds[p].isBg = 0;
			while (i < len && (inputWords[i] == ' ' || inputWords[i] == '\t')) {
				i++;
			}
			continue;
		}
		//处理空白符
		else if (inputWords[i] == ' ' || inputWords[i] == '\t') {
			addr[k] = '\0';
			k = 0;
			if (!flag) {
				args[++j] = (char *)malloc(LEN + 1);
				addr = args[j];
			}
		}
		//处理重定向符和后台运行标记
		else if (inputWords[i] == '<' || inputWords[i] == '>' || inputWords[i] == '&') {
			if (k > 0) {
				addr[k] = '\0';
				k = 0;
				if (!flag) {
					args[++j] = (char *)malloc(LEN + 1);
					addr = args[j];
				}				
			}
			if (inputWords[i] == '<') {
				inputFile = (char *)malloc(LEN + 1);
				addr = inputFile;
				hasInput = 1;
			}
			else if (inputWords[i] == '>') {
				outputFile = (char *)malloc(LEN + 1);
				addr = outputFile;
				hasOutput = 1;
			}
			else if (inputWords[i] == '&') {
				cmds[p].isBg = 1;
			}
			flag = 1;
			i++;
		}
		else {
			addr[k++] = inputWords[i++];
			continue;
		}
		while (i < len && (inputWords[i] == ' ' || inputWords[i] == '\t')) {
			i++;
		}
	}
	if (inputWords[len - 1] != ' ' && inputWords[len - 1] != '\t' && inputWords[len - 1] != '&') {
		if (!flag) {
			j++;
		}
		addr[k] == '\0';
	}

	//写入命令及参数
	cmds[p].command = (char *)malloc(strlen(args[0]) + 1);
	strcpy(cmds[p].command, args[0]);
	for (i = 0; i < j; i++) {
		cmds[p].params[i] = (char *)malloc(strlen(args[i]) + 1);
		strcpy(cmds[p].params[i], args[i]);
	}
	cmds[p].params[i] = NULL;
	//获取输入重定向文件
	if (hasInput) {
		cmds[p].inFile = (char *)malloc(strlen(inputFile) + 1);
		strcpy(cmds[p].inFile, inputFile);
	}
	//获取输出重定向文件
	if (hasOutput) {
		cmds[p].outFile = (char *)malloc(strlen(outputFile) + 1);
		strcpy(cmds[p].outFile, outputFile);
	}
	
	return (p + 1);
}


//执行命令
void executeCmd() {
	int cmdNum = parseCmd();

	//判断是否为复杂命令
	if (cmdNum > 1) {
		//处理管道命令
		executePipe(cmdNum);
		return ;
	}
	//执行内部命令
	if (strcmp(cmds[0].command, "exit") == 0) {
		exit(0);
	}
	else if (strcmp(cmds[0].command, "history") == 0) {
		doHistory();
	}
	else if (strcmp(cmds[0].command, "jobs") == 0) {
		doJobs();
	}
	else if (strcmp(cmds[0].command, "cd") == 0) {
		doCd(cmds[0]);
	}
	else if (strcmp(cmds[0].command, "fg") == 0) {
		doFg(cmds[0]);
	}
	else if (strcmp(cmds[0].command, "bg") == 0) {
		doBg(cmds[0]);
	}
	else {
		//执行外部命令
		executeCommonCmd(cmds[0]);
	}

}

//执行普通命令
void executeCommonCmd(CMD cmd) {
	int i, j, len = strlen(cmd.command);
	pid_t pid;
	char *cmdStr = getCommonCmd(cmd.command);
	
	if (cmdStr != NULL) {
		pid = fork();
		
		if (pid < 0) {
			return ;
		}		
		//子进程
		if (pid == 0) {
			//处理输入重定向
			if (cmd.inFile != NULL) {
				inputRedirect(cmd);
			}
			//处理输出重定向
			if (cmd.outFile != NULL) {
				outputRedirect(cmd);
			}
			//处理后台命令
			if (cmd.isBg) {
				//若父进程更改continue信号，可跳出下面的循环继续执行
				signal(SIGUSR1, modCont);
				while (!cont)
					;
				cont = 0;
				printf("%d\t%s\t%s\n", getpid(), "running", inputWords);
				kill(getppid(), SIGUSR1);
			}
			//调整命令格式
			i = 0;
			while (i < len && cmd.command[i] != '/')
				i++;
			if (i != len) {
				for (j = 0; i < len; j++, i++)
					cmd.command[j] = cmd.command[i];
				cmd.command[j] = '\0';
			}
			strcpy(cmd.params[0], cmd.command);

			//执行命令
			if (execv(cmdStr, cmd.params) < 0) {
				printf("Command : %s cannot be exceuted.\n", cmd.command);
				return ;
			}
		}
		
		else{
			//处理后台命令
			if (cmd.isBg) {
				fgPid = 0;
				insertJob(pid);
				kill(pid, SIGUSR1);

				//若子进程更改continue信号，可跳出下面的循环继续执行
				signal(SIGUSR1, modCont);
				while (!cont)
					;
				cont = 0;
			}
			//处理前台命令
			else {
				fgPid = pid;
				//在前台等待
				waitpid(pid, NULL, 0);
			}
		}
		
	}
	else {
		printf("Command : %s cannot be executed.\n", cmd.command);
	}
}

//执行cd命令
void doCd(CMD cmd) {
	char *dir = cmd.params[1];
	
	if (dir != NULL) {
		if (chdir(dir) < 0)
			printf("cd: %s: No such file or directory.\n", dir);
	}
}

//执行fg命令
void doFg(CMD cmd) {
	char *arg = cmd.params[1];

	if (arg != NULL && arg[0] == '%') {
		arg++;
		if (strcmp(arg, "-1") != 0) {
			fgCmd(atoi(arg));
		}
	}
	else {
		printf("fg: illegal arguments.\n");
	}
}

//执行bg命令
void doBg(CMD cmd) {
	char *arg = cmd.params[1];

	if (arg != NULL && arg[0] == '%') {
		arg++;
		if (strcmp(arg, "-1") != 0) {
			bgCmd(atoi(arg));
		}
	}
	else {
		printf("bg: illegal arguments.\n");
	}
}

//输入重定向
void inputRedirect(CMD cmd) {
	int pipe;
	
	if ((pipe = open(cmd.inFile, O_RDONLY, S_IRUSR|S_IWUSR)) == -1) {
		printf("Cannot open file : %s.\n", cmd.inFile);
		return ;
	}
	if (dup2(pipe, 0) == -1) {
		printf("Input redirect error.\n");
	}
	close(pipe);
}

//输出重定向
void outputRedirect(CMD cmd) {
	int pipe;
	
	if ((pipe = open(cmd.outFile, O_WRONLY|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR)) == -1) {
		printf("Cannot open file : %s.\n", cmd.outFile);
		return ;
	}
	if (dup2(pipe, 1) == -1) {
		printf("Output redirect error.\n");
	}
	close(pipe);
}

//获取完整命令
char* getCommonCmd(char *cmd) {
	int i;
	char *res = (char *)malloc(40);
	
	if (access(cmd, F_OK) == 0) {
		strcpy(res, cmd);
		return res;
	}
	else {
		for (i = 0; cmdPath[i] != NULL; i++) {
			strcpy(res, cmdPath[i]);
			strcat(res, cmd);
			if (access(res, F_OK) == 0)
				return res;
		}
	}
	return NULL;
}

//更改continue信号量
void modCont() {
	cont = 1;
}


