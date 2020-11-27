/*
@author Patrick Kupcha (netid kupcha)
CS214, Section 04
Asst1 - ++Malloc
*/

#include "mymalloc.h"

//given block size per assignment
#define BLOCK_SIZE 4096

//our representation of main memory -- char bc: 1 byte(sizeof(char)) * total bytes == sizeof our main memory - the '!' char at myblock[0] will represent an unitialized block and will be recognized the first time that malloc() is called
static char myblock[BLOCK_SIZE] = {'!'};

/*
This function is used to iterate through our block of memory to find the next unallocated block with sufficient size to malloc
@param a ptr to the metadata of our current fragment of memory
@return a ptr to the metadata of the next fragment of memory
*/
meta* findNextMeta(meta* curr){
   char* ptr = (char*) curr;
   ptr = ptr + curr->size + sizeof(meta);
   return (meta*) ptr;
}

/*
This function is used to find the fragment of memory immediately before a fragment that we are freeing. We use this to check if two consecutive fragments are both unallocated so they may be combined if necessary. This uses a sequential search to iterate through our memory array. I chose to use this function instead of storing the previous fragment in the metadata in order to reduce the size of the metadata.
@param a ptr to the metadata of the current fragment of memory
@return a ptr to the metadata of the previous fragment of memory
*/
meta* findPreviousMeta(meta* ptr){
   meta* curr = (meta*) myblock;
   meta* prev;
   while (curr != ptr){
      prev = curr;
      curr = findNextMeta(curr);
   }
   return prev;

}

/*
My versoin of malloc(). On success the function will find an unallocated fragment of memory in our main memory with sufficient size and fragment a chunk of that memory, create a metadata struct that describes the new fragment of memory. If there is insufficient memory available, or the requested size is null or not positive, the function will return null.
@param size (in bytes) to be malloced
@return ptr to the beginning of the allocated fragment of memory OR NULL if error
*/
void *mymalloc(size_t size, char* file, int line){
  //ptr to the beginning of our main memory
   meta *head = (meta*) myblock;
   //check for proper input/can we even fit this size block
   if (size + sizeof(meta) > BLOCK_SIZE){
      printf("ERROR in %s, line %d: Not enough remaining memory to complete malloc.\n", file, line);
   }
   if (size == 0){
      return NULL;
   }
   //our block must have metaData, this will not exist before first call of mymalloc
   if (myblock[0] == '!'){
     //printf("Intitializing block...\n");
      head->allocated = '0';
      head->size = BLOCK_SIZE - sizeof(meta);
   }
   //we are using first available algo -- start at beginning of memory and find first chunk big enough to return
   meta *curr = head;
   //iterates through array in sequential order until it finds a unallocated block big enough
   while ((char*)curr < &myblock[BLOCK_SIZE]){
      //if current block is unallocated and large enough, we split into fragment to return and another unallocated fragment for remaining memory
      if (curr->allocated == '0' && curr->size >= size){
         curr->allocated = '1';
         int balance = curr->size - size;
         curr->size = size;
         char* ptr = (char*)curr + sizeof(meta) + size;
         //if balance is not big enough for another meta block + space, we will just add it to the current block
         if (balance <= sizeof(meta)){
           curr->size += balance;
         }
         //if we can fit another block with remainder then we must initialize the metadata
         else if (ptr + sizeof(meta) < &myblock[BLOCK_SIZE]){
           meta* temp = (meta*)ptr;
           temp->size = balance - sizeof(meta);
           temp->allocated = '0';
         }
         else{
           curr->size += balance;
         }
         ptr = (char*) curr + sizeof(meta);
         return (void*)ptr;
      }else{
         curr = findNextMeta(curr);
      }
   }
   printf("ERROR in %s, line %d: Not enough remaining memory to complete malloc.\n", file, line);
   return NULL;
}

/*
My version of free(). This function will unallocate the input fragment of memory, and consolidate it with any unallocated fragments immediately before or after the input fragment. If the input ptr is null, outside our designated memory array, or not the beginning of an allocated fragment, then it will print an error message and return.
@param ptr to the beginning of an allocated memory block
*/
void myfree(void *ptr, char* file, int line){
  //check if null
   if (ptr == NULL){
      return;
   }
   char* getBlock = (char*) ptr - sizeof(meta);
   meta* curr = (meta*) getBlock;
   //if ptr given is not within our memory block then we should not be freeing it
   if ((char*) curr < myblock || (char*)curr > myblock + (BLOCK_SIZE)){
      printf("ERROR in %s, line %d: Pointed to memory is outside of our working memory.\n", file, line);
      return;
   }
   //if memory hasn't been allocated yet we should not be freeing
   else if (curr->allocated == '0'){
      printf("ERROR in %s, line %d: Cannot free unallocated memory.\n", file, line);
      return;
   }
   //check to make sure we are pointing to start of a block
   else if (curr->allocated != '1'){
     printf("ERROR in %s, line %d: Pointed to memory is within working memory but not pointing to beginning of a valid memory block.\n", file, line);
   }
   //otherwise the memory is within our block and has been allocated -- we can free
   else{
      meta* prev; meta* next;
      // if ptr is first block then there will be no prev, check for next
      if((char*)curr == myblock){
        //printf("Freeing first block of memory.\n");
         next = findNextMeta(curr);
         if ((char*) next > &myblock[BLOCK_SIZE - 1]){
           curr->allocated = '0';
         }
         else if (next->allocated == '0'){
           curr->size += sizeof(meta) + next->size;
           curr->allocated = '0';
         }
         else{
           curr->allocated = '0';
         }
         return;
      }
      // if ptr is last block of array there will be no next meta
      else if ((char*)curr + sizeof(meta) + curr->size == &myblock[BLOCK_SIZE]){
        //printf("Freeing block at the end of array.\n");
         prev = findPreviousMeta(curr);
         if (prev->allocated == '0'){
            prev->size += sizeof(meta) + curr->size;
         }else{
            curr->allocated = '0';
         }
         return;
      }
      //memory is somewhere in the middle of our block, we should check prev and next block for possible combinations
      else{
        //printf("Freeing memory somewhere in the middle of our block.\n");
         prev = findPreviousMeta(curr);
         next = findNextMeta(curr);
         if (prev->allocated == '0'){
            prev->size += sizeof(meta) + curr->size;
            if (next->allocated == '0'){
               prev->size += sizeof(meta) + next->size;
            }
            return;
         }
         else if (next->allocated == '0'){
            curr->size += sizeof(meta) + next->size;
            curr->allocated = '0';
            return;
         }
         else{
            curr->allocated = '0';
            return;
         }
      }
   }
 }

/*
This function was used during testing to better visualize the status of our memory block. Calling this function will print out line by line each fragment of memory, whether or not it is allocated, and the size of the fragment. This function can only be called once myblock has been initialized (malloc() must be called at least once)
*/
void printCurrentMemory(){
  meta* curr = (meta*) myblock;
  int fragmentCount = 0;
  int bytesTravelled = 0;
  while ((char*) curr < &myblock[BLOCK_SIZE - 1]){
    fragmentCount++;
    printf("Memory fragment #%d : Allocated: ", fragmentCount);
    if (curr->allocated == '0'){
      printf("N, Size: %d\n", curr->size);
    }else{
      printf("Y, Size: %d\n", curr->size);
    }
    bytesTravelled += sizeof(meta) + curr->size;
    curr = findNextMeta(curr);
  }
  printf("Total bytes in memory: %d\n", bytesTravelled);
  printf("-------------------------\n");
}
