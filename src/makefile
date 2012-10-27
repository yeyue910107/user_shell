user-sh : bison.tab.o command.o history.o initial.o job.o pipe.o
	gcc -o user-sh bison.tab.o command.o history.o initial.o job.o pipe.o
bison.tab.o : bison.tab.c config.h
	gcc -c bison.tab.c
history.o : history.c config.h history.h
	gcc -c history.c
initial.o : initial.c config.h 
	gcc -c initial.c
job.o : job.c config.h job.h
	gcc -c job.c
pipe.o : pipe.c config.h pipe.h
	gcc -c pipe.c
command.o : command.c config.h command.h
	gcc -c command.c
clean :
	rm user-sh bison.tab.o command.o history.o initial.o job.o pipe.o



