%{
	#include "config.h"
	int yylex();
	void yyerror();
	int offset, length;
%}

//文法
/*
命令	： 空 ｜ 复杂命令；
复杂命令	： 简单命令 ｜ 简单命令 ‘｜’ 复杂命令；
简单命令	： 命令 ｜ 命令 ‘＆’；
命令	： 命令字符串 输入重定向 输出重定向；
输入重定向： 空 ｜ ‘<’ 终结符；
输出重定向： 空 ｜ ‘>’ 终结符；
命令参数	： 空 ｜ 命令参数 终结符；
*/
%token TOKEN
%%
cmdLine		: /* empty */ | complexCmd { executeCmd(); };
complexCmd	: simpleCmd | simpleCmd '|' complexCmd;
simpleCmd	: innerCmd | innerCmd '&';
innerCmd	: progInvocate inputRedirect outputRedirect;
progInvocate	: TOKEN args;
inputRedirect	: /* empty */ | '<' TOKEN;
outputRedirect	: /* empty */ | '>' TOKEN;
args		: /* empty */ | args TOKEN;
%%

//词法分析器
int yylex()
{
	//char inputWords[MAX_COMMAND_INPUT], filename[MAX_FILE_NAME_LENGTH];
	int flag = 0;
	//scanf ("%s", inputWords);
	
	while (offset < length) {
		if (inputWords[offset] != ' ' && inputWords[offset] != '\t')		
			break;
		offset++;
	}

	while (offset < length)
	{
		
		if (inputWords[offset] == ' ' || inputWords[offset] == '\t') {
			offset++;
			return TOKEN;
		}
		if (inputWords[offset] == '<' || inputWords[offset] == '>' || inputWords[offset] == '&') {
			if (flag) {
				flag = 0;			
				return TOKEN;
			}
			return inputWords[offset++];
		}
		flag = 1;
		offset++;
	}
	if (flag)
		return TOKEN;
	else
		return 0;
}

//错误处理
void yyerror() {
	printf("Illegal command : %s\n", inputWords);
}

int main(int argc, char** argv) {
	int i;
	char c;
	init();
	printf("user-sh@%s>", get_current_dir_name());

	while (1) {
		i = 0;
		//scanf("%s", inputWords);
		//length = strlen(inputWords);
		while ((c = getchar()) != '\n')
			inputWords[i++] = c;
		inputWords[i] = '\0';
		length = i;
		
		//length = strlen(inputWords);
		offset = 0;
		yyparse();
		addHistory(inputWords);
		printf("user-sh@%s>", get_current_dir_name());
	}

	return (0);
}
