ush:	cscd340Lab6.c ./tokenize/makeArgs.o ./utils/myUtils.o ./process/process.o ./pipes/pipes.o ./alias/alias.o ./history/history.o ./linkedlist/linkedList.o ./linkedlist/listUtils.o
	gcc -g cscd340Lab6.c ./tokenize/makeArgs.o ./utils/myUtils.o ./process/process.o ./pipes/pipes.o ./alias/alias.o ./history/history.o ./linkedlist/linkedList.o ./linkedlist/listUtils.o -o ush

makeArgs.o:	./tokenize/makeArgs.c ./tokenize/makeArgs.h
	gcc -g -c ./tokenize/makeArgs.c

myUtils.o:	./utils/myUtils.o ./utils/myUtils.h
	gcc -g -c ./utils/myUtils.c

process.o:	./process/process.h ./process/process.c
	gcc -g -c ./process/process.c	

pipes.o:	./pipes/pipes.h ./pipes/pipes.c
	gcc -g -c ./pipes/pipes.c
   
alias.o:	./alias/alias.h ./alias/alias.c
	gcc -g -c ./alias/alias.c

history.o:	./history/history.h ./history/history.c
	gcc -g -c ./history/history.c

linkedList.o:	./linkedlist/linkedList.h ./linkedlist/linkedList.c ./linkedlist/requiredIncludes.h
	gcc -g -c ./linkedlist/linkedList.c
   
listUtils.o:	./linkedlist/listUtils.h ./linkedlist/listUtils.c
	gcc -g -c ./linkedlist/listUtils.c
   
clean:
	rm ./pipes/pipes.o	
	rm ./utils/myUtils.o
	rm ./process/process.o
	rm ./tokenize/makeArgs.o
	rm ./alias/alias.o
	rm ./history/history.o
	rm ./linkedlist/linkedList.o
	rm ./linkedlist/listUtils.o
	rm ush