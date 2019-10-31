#ifndef ALIAS_H
#define ALIAS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct alias
{
   char * cmd;
   char ** argv;
   int argc;
};
typedef struct alias Alias;

void printTypeAlias();
void * buildTypeAlias(FILE * fin);
void * buildTypeUnalias(FILE * fin);
void cleanTypeAlias(void * ptr);
int compareAlias(const void * p1, const void * p2);
#endif /* ALIAS_H */