#include "linkedList.h"

/*struct node
{
    void * data;
    struct node * next;
    struct node * prev;
};
typedef struct node Node;*/

/*struct linkedlist
{
    Node * head;
    int size;
};
typedef struct linkedlist LinkedList;*/

LinkedList * linkedList()
{
   LinkedList * list = (LinkedList*)calloc(1, sizeof(LinkedList));
   list -> head = (Node*)calloc(1, sizeof(Node));
   list -> size = 0;
   
   return list;
}

void addLast(LinkedList * theList, Node * nn)
{
   if (theList == NULL || nn == NULL)
   {
      exit(-99);
   }
   
   Node * cur = theList -> head -> next;
   if (theList -> size == 0)
   {
      addFirst(theList, nn);
   }
   else
   {
      while (cur -> next != NULL)
      {
         cur = cur -> next;
      }
      
      cur -> next = nn;
      nn -> prev = cur;
      nn -> next = NULL;
      theList -> size = theList -> size + 1;
   }
   
}

void addFirst(LinkedList * theList, Node * nn)
{
   if (theList == NULL || nn == NULL)
   {
      exit(-99);
   }
   
   nn -> prev = theList -> head;
   nn -> next = theList -> head -> next;
   theList -> head -> next = nn;
   theList -> size = theList -> size + 1;
}

void removeFirst(LinkedList * theList, void (*removeData)(void *))
{
   if (theList == NULL)
   {
      exit(-99);
   }
   
   if (theList -> size == 0)
   {
      printf("The list is empty");
   }
   else
   {   
      Node * node = theList -> head -> next;
      
      removeData(theList -> head -> next -> data);
      theList -> head -> next -> next -> prev = theList -> head;
      theList -> head -> next = theList -> head -> next -> next;
      theList -> size = theList -> size - 1;
      
      free(node);
      node = NULL;
   }
}

void removeLast(LinkedList * theList, void (*removeData)(void *))
{
   if (theList == NULL)
   {
      exit(-99);
   }
   
   if (theList -> size == 0)
   {
      printf("The list is empty");
   }
   else
   {
      Node * cur = theList -> head;
      
      while (cur -> next != NULL)
      {
         cur = cur -> next;
      }
      
      removeData(cur -> data);
      cur -> prev -> next = NULL;
      free(cur); //Probably
      cur = NULL;
      theList -> size = theList -> size - 1;
   }
}

void removeItem(LinkedList * theList, Node * nn, void (*removeData)(void *), int (*compare)(const void *, const void *))
{
   Node * cur = theList -> head -> next;
   int count = 0;
   
   while (cur != NULL && count == 0)
   {
      if (compare(nn -> data, cur -> data) == 0)
      {
         if (cur -> next == NULL) //edge case; removing from end of list
         {
            removeData(cur -> data);
            cur -> prev -> next = NULL;
            theList -> size = theList -> size - 1;
            count++;
         }
         else
         {
            removeData(cur -> data);
            cur -> next -> prev = cur -> prev;
            cur -> prev -> next = cur -> next;
            theList -> size = theList -> size - 1;
            count++;
         }
      }
      if (count == 0)
      {
         cur = cur -> next;
      }
   }
   if (count == 0)
   {
      printf("\nData was not found in list\n");
   }
   
   removeData(nn -> data);
   free(nn);
   free(cur);
   cur = NULL;
   nn = NULL;
}

void clearList(LinkedList * theList, void (*removeData)(void *))
{
   if (theList -> size != 0)
   {
      Node * cur = theList -> head -> next;
      Node * temp;
      
      while (cur != NULL)
      {
         temp = cur;
         cur = cur -> next;
         if (temp -> data != NULL)
         {
            removeData(temp -> data);
            free(temp);
         }
      }
   }
   free(theList -> head);
   theList -> head = NULL;
   theList -> size = 0;
}

void removeAll(LinkedList * theList, void (*removeData)(void *))
{
   if (theList -> size != 0)
   {
      Node * cur = theList -> head -> next;
      Node * temp;
      
      while (cur != NULL)
      {
         temp = cur;
         cur = cur -> next;
         if (temp -> data != NULL)
         {
            removeData(temp -> data);
            free(temp);
         }
      }
   }
}

void printList(const LinkedList * theList, void (*convertData)(void *))
{
   if (theList == NULL)
   {
      exit(-99);
   }
   
   if (theList -> size == 0)
   {
      printf("Empty List\n");
   }
   else
   {
      Node * cur = theList -> head -> next;
      //printf("\nSize: %d\n\n", theList -> size);
      while ( cur != NULL)
      {
         //printWord(cur -> data);
         convertData(cur -> data);
         cur = cur -> next;
      }
      
      printf("\n");
   }
}