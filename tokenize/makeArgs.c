#include "makeArgs.h"

void clean(int argc, char **argv)
{
   int x;
   
   for (x = 0; x < argc; x++)
   {
      free(argv[x]);
      argv[x] = NULL;
   }
   free(argv);
   argv = NULL;


}// end clean

void printargs(int argc, char **argv)
{
	int x;
	for(x = 0; x < argc; x++)
		printf("%s\n", argv[x]);

}// end printargs

int makeargs(char *s, char *** argv)
{    
   int total = 0, x;
   char copy[100];
   char * savePtr = copy;
   strcpy(copy, s);
   
   char * temp = strtok(copy, " "); //maybe " "
   
   while (temp != NULL)
   {
      temp = strtok(NULL, " "); //maybe " "
      total++;
   }
   if (total == 0)
      return -1;
      
   strcpy(copy, s);
   
   *argv = (char**)calloc(total+1, sizeof(char*));
   char * temp2 = strtok_r(copy, " ", &savePtr);
   (*argv)[0] = (char*)calloc(strlen(temp2)+1, sizeof(char));
   strcpy((*argv)[0], temp2);
   
   for (x = 1; x < total; x++)
   {
   temp2 = strtok_r(NULL, " ", &savePtr);
   (*argv)[x] = (char*)calloc(strlen(temp2)+1, sizeof(char));
   strcpy((*argv)[x], temp2);
   }
   
   return total;

}// end makeArgs
