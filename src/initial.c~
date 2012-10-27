#include "command.h"

//初始化
void init(){
	int i=0,k=0,pathFlag=0;
	FILE *in;
        char *buf,tmpStr[80];

        if((in = fopen("user-sh.conf","r")) == NULL){
                printf("Initialize failed.\n");
                exit(1);
        }

	cont = sigchld = 0;
        //初始化history链表
	history.begin = 0;
        history.end = -1;
        //将路径文件内容读入到buf[]中
	buf=(char*)malloc(sizeof(char)*(80));
        fgets(buf,79,in);

        //将user-sh.conf中包含的环境路径存入cmdPath[]
	while(i<strlen(buf)){
		k = 0;
		while(i<strlen(buf) && buf[i]!=':'){
			tmpStr[k++]=buf[i++];			
		}
		if(i<strlen(buf) && buf[i]==':'){
			if(tmpStr[k-1]!='/'){
                        	tmpStr[k++]='/';
				tmpStr[k]='\0';
				i++;
                	}
			cmdPath[pathFlag]=(char*)malloc(sizeof(char)*(strlen(tmpStr)+1));
			strcpy(cmdPath[pathFlag++],tmpStr);	
		}
	}

        //注册信号
       	struct sigaction action;
	action.sa_sigaction = removeJob;
	sigfillset(&action.sa_mask);
	action.sa_flags = SA_SIGINFO;
	sigaction(SIGCHLD,&action,NULL);
	
	signal(SIGTSTP,ctrlZ);		//注册ctrlZ函数信号
	signal(SIGINT,ctrlC);		//注册ctrlC函数信号
	signal(SIGUSR1, modCont);	//注册modcout函数信号
}

