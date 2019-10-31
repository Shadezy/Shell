/*KNOWN ERRORS:
 * -When creating an alias or unaliasing one, you have to enter the command twice. Or rather you have to enter "alias blah" to get into the
 * if statement and then enter the command once you're there.
 * -Can't use pipes with aliases.
 * -! and then anything but a number will probably crash the program.
 * -!! or !ing won't work if the action calls another ! or !! --- I did this so I didn't have some infinite loop.
 * -Invalid commands print twice to .mshrc_history and, because of this, twice to the screen as well when history is called.
 * -Oh, and PATH doesn't work.
 *
 */


#include "./pipes/pipes.h"
#include "./utils/myUtils.h"
#include "./process/process.h"
#include "./tokenize/makeArgs.h"
#include "./linkedlist/linkedList.h"
#include "./linkedlist/listUtils.h"
#include "./linkedlist/requiredIncludes.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main()
{
   int argc, pipeCount, histCount = 100, histFileCount = 1000, lineCount = 0;;
   char **argv = NULL, s[MAX];
   int preCount = 0, postCount = 0;
   char ** prePipe = NULL, ** postPipe = NULL;
   char garbage[100];
   
   LinkedList * histList = linkedList();
   LinkedList * aliasList = linkedList();
   //LinkedList pathList = LinkedList();
   
   FILE * fin = fopen(".mshrc", "r");
   FILE * fout = NULL;
   FILE * finHist = fopen(".mshrc_history", "r");
   FILE * foutHist = NULL;
   
   if (fin == NULL)
   {
      fout = fopen(".mshrc", "w");
      
      fprintf(fout, "HISTCOUNT=%d\n", 100);
      fprintf(fout, "HISTFILECOUNT=%d\n\n", 1000);
      
      fprintf(fout, "alias ll='ls -l'\n");
      fprintf(fout, "alias la='ls -a'\n\n");
      
      //fprintf(fout, "PATH stuff\n");
      
      fclose(fout);
      fin = fopen(".mshrc", "r");
      fgets(garbage, 100, fin);
      fgets(garbage, 100, fin);
      fgets(garbage, 100, fin);
   }
   else
   {
      char temp[100], temp2[100];
      char * tok;
      char * tok2;
      char * tok3;
      char * tok4;
      
      fgets(temp, 100, fin);
      strip(temp);
      
      tok = strtok(temp, "=");
      tok2 = strtok(NULL, "="); //these might not work, but right idea
      
      fgets(temp2, 100, fin);
      strip(temp2);
      
      tok3 = strtok(temp2, "=");
      tok4 = strtok(NULL, "=");
      
      fgets(garbage, 100, fin);
      
      if (strcmp(tok, "HISTCOUNT") == 0) //tok might have '\0'
      {
         histCount = atoi(tok2);
         histFileCount = atoi(tok4);
      }
      else
      {
         histFileCount = atoi(tok2);
         histCount = atoi(tok4);
      }
   }
   
   buildListTotal(aliasList, 2, fin, buildTypeAlias);
   
   if (finHist == NULL)
   {
      foutHist = fopen(".mshrc_history", "a+");
      //fprintf(foutHist, "garbage\n");
   }
   else
   {
      char temp[100];
      
      //prime read
      fgets(temp, 100, finHist);
      while (!feof(finHist))
      {
         lineCount++; //count might be off by one
         fgets(temp, 100, finHist);
      }
      
      rewind(finHist);
      buildListTotal(histList, lineCount, finHist, buildTypeHist);
      fclose(finHist);
      foutHist = fopen(".mshrc_history", "a+");
   }
   
   fclose(fin);
   
   printf("command?: ");
   fgets(s, MAX, stdin);
   strip(s);
   
   while(strcmp(s, "exit") != 0)
   {
   
      fprintf(foutHist, "%s\n", s);
      lineCount++;
      
      if(strcmp(s, "alias") == 0)
      {
         printList(aliasList, printTypeAlias);
      } //end print alias
      
      else if (strstr(s, "unalias") != NULL)
      {       
         removeItem(aliasList, buildNode(stdin, buildTypeUnalias), cleanTypeAlias, compareAlias);
      }
      
      else if(strstr(s, "alias") != NULL && strlen(s) > 5)
      {
         addLast(aliasList, buildNode(stdin, buildTypeAlias)); //have to enter command twice
      } //end create alias
      
      else if(strstr(s, ">") != NULL)
      {
         char copy[100];
         char * temp;
         char * loc;
         FILE * dest = NULL;
         
         strcpy(copy, s);
         temp = strtok(copy, ">");
         loc = strtok(NULL, " ");
         //dest = fopen(loc, "w"); open later
         
         argc = makeargs(temp, &argv);
   	  	   if(argc != -1)
   	  	   {
               //int fd[2];
   	  		   int status;
               pid_t pid = fork();
               
               if (pid != 0) //parent
               {
                  waitpid(pid, &status, 0);
               }
               else //child
               {
                  int fd = open(loc, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
                  dup2(fd, 1);
                  close(fd);
                  int res = execvp(argv[0], argv);
                  if (res == -1)
                     exit(-1);
               }
               
   	  		   clean(argc, argv);
   	  		   argv = NULL;
   		   }// end if
            
      } //end redirect
      
      else if(strcmp(s, "cd") == 0)
      {
         chdir(getenv("HOME"));
      } //end cd to home directory
      
      else if(strstr(s, "cd") != NULL && strlen(s) > 2) //should've done this first; this was the easiest part of the assignment
      {
         char * temp = strtok(s, " ");
         temp = strtok(NULL, " ");
         
         if (strcmp(temp, "~") == 0)
            chdir(getenv("HOME")); //go home!!
         else
            chdir(temp);
      } //end cd somewhere
      
      else if(strcmp(s, "history") == 0) //this is soooo inefficient
      {
         clearList(histList, cleanTypeHist);
         free(histList);
         histList = linkedList();
         rewind(foutHist);
         
         if (lineCount >= histCount)
            buildListTotal(histList, histCount, foutHist, buildTypeHist);
         else
            buildListTotal(histList, lineCount, foutHist, buildTypeHist);
            
         printList(histList, printTypeHist);
      } //end history
      
      else if(strcmp(s, "!!") == 0) //eh, it works at least
      {
         static int count = 0;
         int x = 0, y = 0;
         char garb[100], temp[100];
         
         if (lineCount == 1 + count)
         {
            count++;
            printf("there ain't nothin' there boi\n");
         }
         else
         {
            rewind(foutHist);
            
            fgets(garb, 100, foutHist);
            while(x < lineCount-2)
            {
               x++;
               fgets(garb, 100, foutHist);
               printf("%s\n", garb);
            }
            
            strip(garb);
            printf("%s\n", garb);
            
            
            //**super repeated code..pretty much the entire main class**//!!
            if(strcmp(garb, "alias") == 0)
            {
               printList(aliasList, printTypeAlias);
            } //end print alias
            
            else if (strstr(garb, "unalias") != NULL)
            {       
               removeItem(aliasList, buildNode(stdin, buildTypeUnalias), cleanTypeAlias, compareAlias);
            }
            
            else if(strstr(garb, "alias") != NULL && strlen(s) > 5)
            {
               addLast(aliasList, buildNode(stdin, buildTypeAlias)); //have to enter command twice
            } //end create alias
            
            else if(strstr(garb, ">") != NULL)
            {
               char copy[100];
               char * temp;
               char * loc;
               FILE * dest = NULL;
               
               strcpy(copy, garb);
               temp = strtok(copy, ">");
               loc = strtok(NULL, " ");
               //dest = fopen(loc, "w"); open later
               
               argc = makeargs(temp, &argv);
         	  	   if(argc != -1)
         	  	   {
                     //int fd[2];
         	  		   int status;
                     pid_t pid = fork();
                     
                     if (pid != 0) //parent
                     {
                        waitpid(pid, &status, 0);
                     }
                     else //child
                     {
                        int fd = open(loc, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
                        dup2(fd, 1);
                        close(fd);
                        int res = execvp(argv[0], argv);
                        if (res == -1)
                           exit(-1);
                     }
                     
         	  		   clean(argc, argv);
         	  		   argv = NULL;
         		   }// end if
                  
            } //end redirect
            
            else if(strcmp(garb, "cd") == 0)
            {
               chdir(getenv("HOME"));
            } //end cd to home directory
            
            else if(strstr(garb, "cd") != NULL && strlen(s) > 2) //should've done this first; this was the easiest part of the assignment
            {
               char * temp = strtok(garb, " ");
               temp = strtok(NULL, " ");
               
               if (strcmp(temp, "~") == 0)
                  chdir(getenv("HOME")); //go home!!
               else
                  chdir(temp);
            } //end cd somewhere
            
            else if(strcmp(garb, "history") == 0) //this is soooo inefficient
            {
               clearList(histList, cleanTypeHist);
               free(histList);
               histList = linkedList();
               rewind(foutHist);
               
               if (lineCount >= histCount)
                  buildListTotal(histList, histCount, foutHist, buildTypeHist);
               else
                  buildListTotal(histList, lineCount, foutHist, buildTypeHist);
                  
               printList(histList, printTypeHist);
            } //end history
            
            else
            {
               int x, count = 0;
               Node * cur = aliasList -> head -> next;
               for (x = 0; x < aliasList -> size; x++)
               {
                  Alias * temp = (Alias*)(cur -> data);
                  if (strcmp(temp -> cmd, garb) == 0)
                  {
                     count++;
            	  	   if(temp -> argc != -1)
            	  	   {
            	  		   forkIt(temp -> argv);
            		   }
                  }
                  cur = cur -> next;
               }
               
               pipeCount = containsPipe(garb);
         	   if(pipeCount > 0)
         	   {
                  prePipe = parsePrePipe(garb, &preCount);
            		postPipe = parsePostPipe(garb, &postCount);
            		pipeIt(prePipe, postPipe);
            		clean(preCount, prePipe);
                  clean(postCount, postPipe);
         	   }// end if pipeCount
         
         	   else if (count == 0)
         	   {
         		   argc = makeargs(garb, &argv);
         	  	   if(argc != -1)
         	  	   {
         	  		   forkIt(argv);
                     //printf("\n\nhere\n\n");
         	  		   clean(argc, argv);
         	  		   argv = NULL;
         		   }// end if
         	   }
            }
         }
      }
      
      else if(strstr(s, "!") != NULL) //there's definitely a better way to do this
      {
         int num, help = 0;
         char garb[100];
         char * tok = strtok(s, "!");
         num = atoi(tok);
         
         if (num > histCount || num > lineCount) //|| num > histFileCount?
            printf("we don't keep info from that far back in here\n");
         else
         {
            rewind(foutHist);
            
            fgets(garb, 100, foutHist);
            while(help != num)
            {
               help++;
               fgets(garb, 100, foutHist);
            }
            strip(garb);
            printf("%s\n", garb);
            
            
            //**super repeated code..pretty much the entire main class**// AGAIN!
            if(strcmp(garb, "alias") == 0)
            {
               printList(aliasList, printTypeAlias);
            } //end print alias
            
            else if (strstr(garb, "unalias") != NULL)
            {       
               removeItem(aliasList, buildNode(stdin, buildTypeUnalias), cleanTypeAlias, compareAlias);
            }
            
            else if(strstr(garb, "alias") != NULL && strlen(s) > 5)
            {
               addLast(aliasList, buildNode(stdin, buildTypeAlias)); //have to enter command twice
            } //end create alias
            
            else if(strstr(garb, ">") != NULL)
            {
               char copy[100];
               char * temp;
               char * loc;
               FILE * dest = NULL;
               
               strcpy(copy, garb);
               temp = strtok(copy, ">");
               loc = strtok(NULL, " ");
               //dest = fopen(loc, "w"); open later
               
               argc = makeargs(temp, &argv);
         	  	   if(argc != -1)
         	  	   {
                     //int fd[2];
         	  		   int status;
                     pid_t pid = fork();
                     
                     if (pid != 0) //parent
                     {
                        waitpid(pid, &status, 0);
                     }
                     else //child
                     {
                        int fd = open(loc, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
                        dup2(fd, 1);
                        close(fd);
                        int res = execvp(argv[0], argv);
                        if (res == -1)
                           exit(-1);
                     }
                     
         	  		   clean(argc, argv);
         	  		   argv = NULL;
         		   }// end if
                  
            } //end redirect
            
            else if(strcmp(garb, "cd") == 0)
            {
               chdir(getenv("HOME"));
            } //end cd to home directory
            
            else if(strstr(garb, "cd") != NULL && strlen(s) > 2) //should've done this first; this was the easiest part of the assignment
            {
               char * temp = strtok(garb, " ");
               temp = strtok(NULL, " ");
               
               if (strcmp(temp, "~") == 0)
                  chdir(getenv("HOME")); //go home!!
               else
                  chdir(temp);
            } //end cd somewhere
            
            else if(strcmp(garb, "history") == 0) //this is soooo inefficient
            {
               clearList(histList, cleanTypeHist);
               free(histList);
               histList = linkedList();
               rewind(foutHist);
               
               if (lineCount >= histCount)
                  buildListTotal(histList, histCount, foutHist, buildTypeHist);
               else
                  buildListTotal(histList, lineCount, foutHist, buildTypeHist);
                  
               printList(histList, printTypeHist);
            } //end history
            
            else
            {
               int x, count = 0;
               Node * cur = aliasList -> head -> next;
               for (x = 0; x < aliasList -> size; x++)
               {
                  Alias * temp = (Alias*)(cur -> data);
                  if (strcmp(temp -> cmd, garb) == 0)
                  {
                     count++;
            	  	   if(temp -> argc != -1)
            	  	   {
            	  		   forkIt(temp -> argv);
            		   }
                  }
                  cur = cur -> next;
               }
               
               pipeCount = containsPipe(garb);
         	   if(pipeCount > 0)
         	   {
                  prePipe = parsePrePipe(garb, &preCount);
            		postPipe = parsePostPipe(garb, &postCount);
            		pipeIt(prePipe, postPipe);
            		clean(preCount, prePipe);
                  clean(postCount, postPipe);
         	   }// end if pipeCount
         
         	   else if (count == 0)
         	   {
         		   argc = makeargs(garb, &argv);
         	  	   if(argc != -1)
         	  	   {
         	  		   forkIt(argv);
                     //printf("\n\nhere\n\n");
         	  		   clean(argc, argv);
         	  		   argv = NULL;
         		   }// end if
         	   }
            }
         }
      }
      
      else //real else
      {
         int x, count = 0;
         Node * cur = aliasList -> head -> next;
         for (x = 0; x < aliasList -> size; x++)
         {
            Alias * temp = (Alias*)(cur -> data);
            if (strcmp(temp -> cmd, s) == 0)
            {
               count++;
      	  	   if(temp -> argc != -1)
      	  	   {
      	  		   forkIt(temp -> argv);
      		   }
            }
            cur = cur -> next;
         }
         
         pipeCount = containsPipe(s);
   	   if(pipeCount > 0)
   	   {
            prePipe = parsePrePipe(s, &preCount);
      		postPipe = parsePostPipe(s, &postCount);
      		pipeIt(prePipe, postPipe);
      		clean(preCount, prePipe);
            clean(postCount, postPipe);
   	   }// end if pipeCount
   
   	   else if (count == 0)
   	   {
   		   argc = makeargs(s, &argv);
   	  	   if(argc != -1)
   	  	   {
   	  		   forkIt(argv);
               //printf("\n\nhere\n\n");
   	  		   clean(argc, argv);
   	  		   argv = NULL;
   		   }// end if
   	   }
      }

	   printf("command?: ");
	   fgets(s, MAX, stdin);
      strip(s);

   }// end while
   
   fprintf(foutHist, "exit\n");
   
   clearList(aliasList, cleanTypeAlias);
   free(aliasList);
   aliasList = NULL;

   clearList(histList, cleanTypeHist);
   free(histList);
   histList = NULL;
   
   fclose(foutHist);
   return 0;

}// end main

