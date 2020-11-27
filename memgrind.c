/*
@author Patrick Kupcha (netid kupcha)
CS214, Section 04
Asst1 - ++Malloc
*/

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "mymalloc.h"

/*
Workload A: malloc() 1 byte and immediately free it - do this 120 times
*/
void workloadA(){
   int i = 0;
   for (i = 0; i < 120; i++){
      char* tester = (char*)malloc(sizeof(char));
      free(tester);
   }
}

/*
Workload B: malloc() 1 byte, store the pointer in an array - do this 120 times.
 Once you've malloc()ed 120 byte chunks, then free() the 120 1 byte pointers one by one.
*/
void workloadB(){
  int i = 0;
  char* testing[120];
  for (i = 0; i < 120; i++){
    testing[i] = (char*) malloc(sizeof(char));
  }
  for (i = 0; i < 120; i++){
    free(testing[i]);
  }
}

/*
Workload C: 240 times, randomly choose between a 1 byte malloc() or free()ing one of the malloc()ed
 pointers
- Keep track of each operation so that you eventually malloc() 120 bytes, in total
- Keep track of each operation so that you eventually free() all pointers
(don't allow a free() if you have no pointers to free())
*/
void workloadC(){
  int totalMallocs = 0;
  int currentMallocs = 0;
  int frees = 0;
  char* test[120];
  int operation;
  srand(time(0));
  while(totalMallocs < 120){
    operation = rand() % 2;
    if (currentMallocs == 0){
      //printf("Mallocing...\n");
      test[currentMallocs] = (char*)malloc(sizeof(char));
      currentMallocs++;
      totalMallocs++;
    }
    else if(operation == 0){
      //printf("Mallocing...\n");
      test[currentMallocs] = (char*)malloc(sizeof(char));
      currentMallocs++;
      totalMallocs++;
    }
    else if(operation == 1){
      //printf("Freeing...\n");
      currentMallocs--;
      free(test[currentMallocs]);
      frees++;
    }else{
      continue;
    }
  }
  while(frees < 120){
    currentMallocs--;
    free(test[currentMallocs]);
    frees++;
  }


}

/*
Custom Workload D: described in testcases.txt file
If the repetitive errors printing are too sloppy, you can change the value of int runErrorChecks to 0, and the program will not call any mallocs or frees that will cause errors.
*/
void workloadD(){
   int runErrorChecks = 1;
   int totalOperations = 0;
   int* causeError = &runErrorChecks;
   char* tooBig[4090];
   int* alsoTooBig[2];
   char* nullPtr = NULL;
   free(nullPtr);
   nullPtr = (char*)malloc(0);
   free(nullPtr);
   //malloc largest possible block, then free
   tooBig[0] = (char*)malloc(4088);
   if(runErrorChecks == 1){
      free(tooBig[0] + 1);
      free(tooBig[0]);
   }
   free(tooBig[0]);
  //ERROR CHECK: try to malloc more bytes than in block
  if (runErrorChecks == 1){
     tooBig[0] = (char*)malloc(4089);
     //ptr but not malloc()ed
     free(causeError);
  }
  int* intPtrs[196];
  int intCounter = 0;
  char* charPtrs[196];
  int charCounter = 0;
  int currentOperation;
  int mallocsToFillArrayPlusOne = 391; //how many times we will malloc to fill up myblock, plus one additional to test error checking
  //fill up myblock - ERROR CHECK: the 391st malloc should fail due to insufficient memory
  //if we do not want to cause errors(int runErrorChecks == 0) then we run one less malloc while filling our array
  if (runErrorChecks == 0){
     mallocsToFillArrayPlusOne--;
 }
  while(totalOperations < mallocsToFillArrayPlusOne){
     currentOperation = totalOperations % 2;
     if (currentOperation == 0){
        intPtrs[intCounter] = (int*) malloc(sizeof(int));
        intCounter++;
        totalOperations++;
     }else{
        charPtrs[charCounter] = (char*) malloc(sizeof(char));
        charCounter++;
        totalOperations++;
     }
  }
  //free ints to create unallocated fragments of 4 bytes
  while(intCounter > 0){
     intCounter--;
     //ERROR CHECK: the first free attempted will be a NULL input because our last malloc() failed
     //(if runErrorChecks == 1), otherwise there will be no error here
     free(intPtrs[intCounter]);
  }
  if (runErrorChecks == 1){
     //there are many unallocated blocks of 4 bytes, but none that contain 8 bytes -- this should fail
     alsoTooBig[0] = (int*)malloc(2 * sizeof(int));
 }
 alsoTooBig[0] = (int*) malloc(sizeof(int));
 free(alsoTooBig[0]);
 //free all remaining allocated blocks
  for (int i = 0; i < charCounter; i++){
     free(charPtrs[i]);
  }

}

/*
Custom Workload E: described in testcases.txt file
*/
void workloadE(){
   char* test[456];
   int* holder[172];
   int i ;
   int totalMallocs = 0;
   for (i = 0; i < 455; i++){
      test[i] = (char*) malloc(sizeof(char));
      totalMallocs++;
   }
   while(totalMallocs > 0){
      totalMallocs--;
      free(test[totalMallocs]);
   }
   for (i = 0; i < 455; i++){
      test[i] = (char*) malloc(sizeof(char));
      totalMallocs++;
   }
   i = 0;
   int alternator = 0;
   while(i < totalMallocs){
      if (alternator % 2 == 0){
         free(test[i]);
         i++;
         alternator++;
      }else{
         totalMallocs--;
         free(test[totalMallocs]);
         alternator++;
      }
   }
   for (i = 0; i < 227; i++){
      test[i] = (char*)malloc(sizeof(char));
   }
   for(i = 0; i < 171; i++){
      holder[i] = (int*) malloc(sizeof(int));
   }
   for (i = 0; i < 171; i++){
      *holder[i] = i;
   }
   for (i = 0; i < 171; i++){
      printf("%d ", *holder[i]);
   }
   printf("\n");
   for (i = 0; i < 171; i++){
      free(holder[i]);
   }
   for(i = 227; i < 455; i++){
      test[i] = (char*) malloc(sizeof(char));
   }
   for (i = 0; i < 455; i++){
      *test[i] = '1';
   }
   for (i = 0; i < 455; i++){
      printf("%c ", *test[i]);
   }
   printf("\n");
   for(i = 0; i < 455; i++){
      free(test[i]);
   }
}


/*
Main function/memgrind driver. Each workloads will be run in the order A->B->C->D->E 50 times. The runtime sum will be recorded for each workload and then averaged and printed after completion.
*/
int main(){
  int i = 0;
  struct timeval start;
  struct timeval finish;
  long runtimeSumA = 0;
  long runtimeSumB = 0;
  long runtimeSumC = 0;
  long runtimeSumD = 0;
  long runtimeSumE = 0;
  for (i = 0; i < 50; i++){
    gettimeofday(&start, 0);
    workloadA();
    gettimeofday(&finish, 0);
    runtimeSumA += (finish.tv_sec - start.tv_sec) * 1000000;
    runtimeSumA += (finish.tv_usec - start.tv_usec);
    gettimeofday(&start, 0);
    workloadB();
    gettimeofday(&finish, 0);
    runtimeSumB += (finish.tv_sec - start.tv_sec) * 1000000;
    runtimeSumB += (finish.tv_usec - start.tv_usec);
    gettimeofday(&start, 0);
    workloadC();
    gettimeofday(&finish, 0);
    runtimeSumC += (finish.tv_sec - start.tv_sec) * 1000000;
    runtimeSumC += (finish.tv_usec - start.tv_usec);
    gettimeofday(&start, 0);
    workloadD();
    gettimeofday(&finish, 0);
    runtimeSumD += (finish.tv_sec - start.tv_sec) * 1000000;
    runtimeSumD += (finish.tv_usec - start.tv_usec);
    gettimeofday(&start, 0);
    workloadE();
    gettimeofday(&finish, 0);
    runtimeSumE += (finish.tv_sec - start.tv_sec) * 1000000;
    runtimeSumE += (finish.tv_usec - start.tv_usec);
  }
  runtimeSumA /= 50;
  printf("Average runtime of workload A is : %lu microseconds\n", runtimeSumA);
  runtimeSumB /= 50;
  printf("Average runtime of workload B is : %lu microseconds\n", runtimeSumB);
  runtimeSumC /= 50;
  printf("Average runtime of workload C is : %lu microseconds\n", runtimeSumC);
  runtimeSumD /= 50;
  printf("Average runtime of workload D is : %lu microseconds\n", runtimeSumD);
  runtimeSumE /= 50;
  printf("Average runtime of workload E is : %lu microseconds\n", runtimeSumE);
  return 1;
}
