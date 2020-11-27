/*
@author Patrick Kupcha (netid kupcha)
CS214, Section 04
Asst1 - ++Malloc
*/
#ifndef _mymalloc_h_
#define _mymalloc_h_

#include <stdlib.h>
#include <stdio.h>

#define malloc( x ) mymalloc( x, __FILE__, __LINE__ )
#define free( x ) myfree( x, __FILE__, __LINE__ )

//holds metadata describing each fragment of memory (sizeof(meta) = 8 bytes)
typedef struct metaData{
   char allocated; //'0' not allocated; '1' allocated -- we are using char bc less bytes than int
   int size; //size in bytes of the fragment of memory associated with this specific meta data
}meta;


meta* findNextMeta(meta* curr);
meta* findPreviousMeta(meta* curr);
void *mymalloc(size_t size, char* file, int line);
void myfree(void *ptr, char* file, int line);
void printCurrentMemory();

#endif
