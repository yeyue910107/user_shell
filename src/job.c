#include "job.h"

//添加作业
JOB* insertJob(pid_t pid){
	JOB*cur;
	JOB*prev;
	JOB*job=(JOB*)malloc(sizeof(JOB));
	job->pid=pid;
	job->command=(char*)malloc(20);
	job->state=(char*)malloc(10);
	strcpy(job->command,inputWords);
	strcpy(job->state,"running");
	job->next=NULL;
	
	if(head==NULL)
		head=job;
	else{
		cur=head;
		while(cur!=NULL&&cur->pid<pid){
			prev=cur;
			cur=cur->next;
		}
		prev->next=job;
		job->next=NULL;
	}
	return job;
}


//删除作业
void removeJob(int signal,siginfo_t* si){
	pid_t pid;
	JOB*cur=NULL;
	JOB*prev=NULL;
	if(sigchld==1){
		sigchld=0;
		return;
	}
	
	pid=si->si_pid;

	cur=head;
	while(cur!=NULL&&cur->pid!=pid){
		prev=cur;
		cur=cur->next;
	}
	if(cur==NULL){
		//NOT FOUND
		return;		
	}
	if(cur==head)
		head=cur->next;
	else
		prev->next=cur->next;
	free(cur);
}

//根据进程号寻找作业
JOB* findJob(pid_t pid){
	JOB*cur=NULL;
	cur=head;
	while(cur!=NULL&&cur->pid!=pid){
		cur=cur->next;
	}
	return cur;

}

//ctrl+z命令挂起前台进程
void ctrlZ(){
	JOB *cur = NULL;
	if(fgPid == 0)
		return;
   	sigchld = 1;
	cur=findJob(fgPid);
    
   	if(cur == NULL)
		cur = insertJob(fgPid);
	//cur->state=(char*)malloc(STLEN);
   	strcpy(cur->state, "stopped"); 
	//cur->command=(char*)malloc(COMMLEN);
  	cur->command[strlen(cur->command)] = '&';
        cur->command[strlen(cur->command) + 1] = '\0';
   	printf("%d\t%s\t%s\n", cur->pid, cur->state, cur->command);
    
	//发送SIGTERM信号挂起前台进程
   	kill(fgPid, SIGSTOP);
        fgPid = 0;

}

//ctrl+c命令终止前台进程
void ctrlC(){
	JOB *cur = NULL;
	if(fgPid == 0)
		return;
   	sigchld = 0;
	cur=findJob(fgPid);
    
   	if(cur == NULL)
		return ; 
	
	//发送SIGTERM信号终止前台进程
   	kill(fgPid, SIGTERM);
        //fgPid = 0;
}


//在前台执行进程号为pid的进程
void fgCmd(int pid){
	JOB *cur= NULL; 
	sigchld = 1;
	int i;
	cur=findJob(pid);
	if(cur== NULL){  
        	printf("Job %d do not exsit.\n", pid);
        	return;
	}
    	fgPid = cur->pid;
	//cur->state=(char*)malloc(STLEN);
    	strcpy(cur->state, "running");
 	//设置signal信号，为下一次按下组合键Ctrl+Z做准备
	signal(SIGTSTP, ctrlZ);  
	//去掉后台程序运行标志 
	i = strlen(cur->command) - 1;
	while(i >= 0 && cur->command[i] != '&')
		i--;
    	cur->command[i] = '\0';
	printf("%s\n", cur->command);
	//发送SIGCONT信号使进程号为cur->pid的进程继续执行
    	kill(cur->pid, SIGCONT); 
	//在前台作业执行完之前处于等待
    	waitpid(fgPid, NULL, 0); 
}

//在后台执行进程号为pid的进程
void bgCmd(int pid){
	JOB*cur=NULL;
	sigchld = 1;
	cur=findJob(pid);
	if(cur==NULL){  
		printf("Job %d do not exsit.\n", pid);
		return;
	}	 
	//由于已经挂起，所以已经获得了＆标记
	strcpy(cur->state, "running");
	printf("%d\t%s\t%s\n", cur->pid, cur->state, cur->command);
	//发送SIGCONT信号使进程号为cur->pid的进程继续执行
	kill(cur->pid, SIGCONT); 
}

//执行jobs命令
void doJobs() {
	JOB *cur = NULL;
	
	if (head == NULL) {
		printf("No jobs.\n");
	}
	else {
		printf("pid\tstate\tcommand\n");
		for (cur = head; cur != NULL; cur = cur->next) {
			printf("%d\t%s\t%s\n", cur->pid, cur->state, cur->command);
		}
	}
}

