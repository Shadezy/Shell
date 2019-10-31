#include "history.h"

/*struct history
{
   char * cmd;
   //int len;
};
typedef struct history History;*/

void cleanTypeHist(void * ptr)
{
   if (ptr == NULL)
   {
      exit(-99);
   }
   
   History * temp = (History*)(ptr);
   
   free(temp -> cmd);
   temp -> cmd = NULL;
   
   free(temp);
}

void * buildTypeHist(FILE * fin)
{
   if (fin == NULL)
   {
      exit(-99);
   }
   
   char temp[100];
   
   fgets(temp, 100, fin);
   strip(temp);
   
   History * hist = (History*)calloc(1, sizeof(History));
   hist -> cmd = (char*)calloc(strlen(temp)+1, sizeof(char));
   strcpy(hist -> cmd, temp);
   //hist -> len = strlen(temp);
   
   return hist;
}

void printTypeHist(void * passedIn)
{
   History * hist = (History*)(passedIn);
   
   printf("%s\n", hist -> cmd);
}
