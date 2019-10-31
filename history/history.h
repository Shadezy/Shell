#ifndef HISTORY_H
#define HISTORY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct history
{
   char * cmd;
   //int len;
};
typedef struct history History;

void cleanTypeHist(void * ptr);
void * buildTypeHist(FILE * fin);
void printTypeHist(void * passedIn);
#endif /* HISTORY_H */