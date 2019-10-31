#include "pipes.h"

int containsPipe(char *s)
{
   int x, count = 0;
   
   for (x = 0; x < strlen(s); x++)
   {
      if (s[x] == '|')
         count++;
   }
   
   //printf("Count: %d\n", count);
   
   return count;
}

char ** parsePrePipe(char *s, int * preCount)
{
   char ** argv = NULL;
   int x, y = 0, z;
   
   for (x = 0; x < strlen(s); x++)
   {
      if (y > 0){}
      
      else
      {
         if (s[x] == '|')
         {
            y = 1;
            z = x;
         }
      }
   }
   
   char temp[z+1];
   
   strncpy(temp, s, z+1);
   temp[z] = '\0';
   
   //printf("prePipe: %s\n", temp);
   
   int total = makeargs(temp, &argv);
   
   *preCount = total;
   
   return argv;
}

char ** parsePostPipe(char *s, int * postCount)
{
   char ** argv = NULL;
   char copy[100];
   
   strcpy(copy, s);
   
   char * temp = strtok(copy, "|");
   temp = strtok(NULL, "|");
   
   
   int total = makeargs(temp, &argv);
   *postCount = total;
   
   //printf("postPipe: %s\n", temp);
   
   return argv;
}

void pipeIt(char ** prePipe, char ** postPipe)
{
   pid_t pid = fork();
   int status;
   
   if (pid != 0) //parent
   {
      waitpid(pid, &status, 0);
   }
   else //child
   {
      int fd[2];
      int res = pipe(fd);
      
      if (fork() != 0) //again parent
      {
      close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		int resul = execvp(postPipe[0], postPipe);
      if (resul == -1)
         exit(-1);
      }
      else //again child
      {
      close(fd[0]);
		close(1);
		dup(fd[1]);
		close(fd[1]);
		int resu = execvp(prePipe[0], prePipe);
      if (resu == -1)
         exit(-1);
      }
   }
   /*pid_t pid;
	int fd[2], res, status;

	res = pipe(fd);

	if(res < 0)
	{
		printf("Pipe Failure\n");
		exit(-1);
	}// end if

	pid = fork();

	if(pid != 0) //parent
	{
      close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		execvp(postPipe[0], postPipe);
      //execlp("wc", "wc", "-w", NULL);
	}
	else //child
	{
		close(fd[0]);
		close(1);
		dup(fd[1]);
		close(fd[1]);
		execvp(prePipe[0], prePipe);
      //execlp("ls", "ls", "-l", NULL);*/
}