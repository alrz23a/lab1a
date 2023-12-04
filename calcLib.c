#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "calcLib.h"

/* Array of char* that points to char arrays. */
char *arith[] = {"add", "div", "mul", "sub", "fadd", "fdiv", "fmul", "fsub"};

/* Used for random number */
time_t myData_seedValue;

int initCalcLib(void) {
  /* Initialize the random number generator with a seed based on the current time; should be random each time called */
  srand((unsigned)time(&myData_seedValue));
  return 0;
}

int initCalcLib_seed(unsigned int seed) {
  /* Initialize the random number generator with a FIXED seed, useful for debugging */
  myData_seedValue = seed;
  srand(seed);
  return 0;
}

char *randomType(void) {
  int Listitems = sizeof(arith) / sizeof(char *);
  /* Figure out HOW many entries there are in the list.
     First, we get the total size that the array of pointers uses, sizeof(arith).
     Then we divide by the size of a pointer (sizeof(char*)), giving us the number of pointers in the list. */
  int itemPos = rand() % Listitems;
  /* As we know the number of items, we can draw a random number and modulo it with the number 
     of items in the list, which gives us a random number between 0 and the number of items in the list.
     Using that information, we return the string found at that position arith[itemPos]. */
  return arith[itemPos];
}

int randomInt(void) {
  /* Generate a random integer between 0 and RAND_MAX, then modulo this with 100 to get a random number between 0 and 100. */
  return rand() % 100;
}

double randomFloat(void) {
  /* Generate a random floating-point number between 0.0 and 100.0 */
  double x = (double)rand() / (double)(RAND_MAX / 100.0);
  return x;
}
