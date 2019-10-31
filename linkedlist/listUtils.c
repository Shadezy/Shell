#include "listUtils.h"

Node * buildNode(FILE * fin, void *(*buildData)(FILE * in))
{
   Node * nn = (Node*)calloc(1, sizeof(Node));
   nn -> data = buildData(fin);
   
   return nn;
}

Node * buildNode_Type(void * passedIn)
{
   Node * nn = (Node*)calloc(1, sizeof(Node));
   nn -> data = passedIn;
   
   return nn;
}

void sort(LinkedList * theList, int (*compare)(const void *, const void *))//newly added items always go to back probably not a problem here though
{
   if (theList == NULL)
   {
      exit(-99);
   }
   
   int x, y;
   void * temp;
   Node * cur = theList -> head -> next;
   
   for (x = 0; x < theList -> size - 1; x++)
   {
      for (y = 0; y < theList -> size - x - 1; y++)
      {
         if (compare(cur -> data, cur -> next -> data) > 0)
         {
            temp = cur -> data;
            cur -> data = cur -> next -> data;
            cur -> next -> data = temp;
         }
         
         cur = cur -> next;
      }
      
      cur = theList-> head -> next;
   }
}

void buildListTotal(LinkedList * myList, int total, FILE * fin, void * (*buildData)(FILE * in))
{
   if (total <= 0 || myList == NULL)
   {
      exit (-99);
   }
   
   int x;
   //rewind(fin);
   
   for (x = 0; x < total; x++) //changing total to 1 here to make things simple in buildTypeHist
   {
      addLast(myList, buildNode(fin, *buildData));
   }
}