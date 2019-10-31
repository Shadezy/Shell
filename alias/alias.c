#include "alias.h"

/*struct alias
{
   char * cmd;
   char ** argv;
   int argc;
};
typedef struct alias Alias;*/

void printTypeAlias(void * passedIn)
{
   if (passedIn == NULL)
   {
      exit(-99);
   }
   
   int x;
   Alias * aAlias = (Alias*)(passedIn);
   
   printf("alias %s='", aAlias -> cmd);
   
   for (x = 0; x < aAlias -> argc; x++)
   {
      printf("%s", aAlias -> argv[x]);
   }
   
   printf("'\n");
}

void * buildTypeAlias(FILE * fin)
{
   if (fin == NULL)
   {
      exit(-99);
   }
   
   int total = 0, x;
   char temp[100], copy[100];
   char * savePtr = copy;
   Alias * aAlias = (Alias*)calloc(1, sizeof(Alias));
   
   fgets(temp, 100, fin); //should be first alias in .mshrc
   strip(temp);
   
   //assume command: alias ll='ls -l'
   char * tok = strtok(temp, " "); //ll='ls -l'; tok = alias
   tok = strtok(NULL, "="); //'ls -l'; tok = ll
   
   aAlias -> cmd = (char*)calloc(strlen(tok)+1, sizeof(char));
   strcpy(aAlias -> cmd, tok);
   
   char * tok2 = strtok(NULL, "'"); //ls -l'; tok2 = '
   strcpy(copy, tok2);
   
   char * tok3 = strtok(copy, " ");
   
   while (tok3 != NULL)
   {
      tok3 = strtok(NULL, " ");
      total++;
   }
   if (total == 0)
      return NULL;
      
   strcpy(copy, tok2);
   aAlias -> argv = (char**)calloc(total+1, sizeof(char*));
   char * tok4 = strtok_r(copy, " ", &savePtr);
   aAlias -> argv[0] = (char*)calloc(strlen(tok4)+1, sizeof(char));
   strcpy(aAlias -> argv[0], tok4);
   
   for (x = 1; x < total; x++)
   {
      tok4 = strtok_r(NULL, " ", &savePtr);
      aAlias -> argv[x] = (char*)calloc(strlen(tok4)+1, sizeof(char));
      strcpy(aAlias -> argv[x], tok4);
   }
   
   aAlias -> argc = total;
   
   return aAlias;
}

void * buildTypeUnalias(FILE * fin) //i'm a bad programmer
{
   if (fin == NULL)
   {
      exit(-99);
   }
   
   int total = 0, x;
   char temp[100], copy[100];
   char * savePtr = copy;
   Alias * aAlias = (Alias*)calloc(1, sizeof(Alias));
   
   fgets(temp, 100, fin); //should be first alias in .mshrc
   strip(temp);
   
   //assume command: unalias ll
   char * tok = strtok(temp, " "); //ll; tok = alias
   tok = strtok(NULL, " "); //ll; tok = ll
   
   aAlias -> cmd = (char*)calloc(strlen(tok)+1, sizeof(char));
   strcpy(aAlias -> cmd, tok);
   
   aAlias -> argv = (char**)calloc(1, sizeof(char*));
   aAlias -> argv[0] = (char*)calloc(strlen(tok)+1, sizeof(char));
   strcpy(aAlias -> argv[0], tok);
   aAlias -> argc = 1;
}

void cleanTypeAlias(void * ptr)
{
   if (ptr == NULL)
   {
      exit(-99);
   }
   
   int x;
   Alias * temp = (Alias*)(ptr);
   free(temp -> cmd);
   temp -> cmd = NULL;
   
   for (x = 0; x < temp -> argc; x++)
   {
      free(temp -> argv[x]);
      temp -> argv[x] = NULL;
   }
   free(temp -> argv);
   temp -> argv = NULL;
   
   temp -> argc = 0;
   free(temp);
}

int compareAlias(const void * p1, const void * p2)
{
   if (p1 == NULL || p2 == NULL)
   {
      exit(-99);
   }
   
   Alias * a1 = (Alias*)(p1);
   Alias * a2 = (Alias*)(p2);
   
   return strcmp(a1 -> cmd, a2 -> cmd);
}