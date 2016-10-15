// This is the ADT for a Queue

#include "Places.h"

typedef struct _Queue *qPtr;

qPtr newQ(); // create an empty queue
void freeQ(qPtr q); // frees a queue
void addToQ(qPtr q, LocationID location); // add a location to queue end
LocationID removeFromQ(qPtr q); // remove a location from queue front
int isQEmpty(qPtr q); // check whether a queue is empty
