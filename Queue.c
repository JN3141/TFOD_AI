// This is the implementation of the ADT for a Queue

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Queue.h"
#include "Places.h"

typedef struct _Node {
	LocationID location;    // holds the location this node refers to
	struct _Node *next;             // the next item in queue
} Node;

typedef struct _Queue {
	struct _Node *front;  // front of the queue
	struct _Node *end;    // end of the queue
} Queue;

// create an empty queue
qPtr newQ() {
	qPtr q = malloc(sizeof(Queue));
	assert(q != NULL);
	q->front = NULL;
	q->end = NULL;
	return q;
}

// frees a queue
void freeQ(qPtr q) {
    assert(q != NULL);

    Node *curr, *next;

	// free queue nodes
    for (curr = q->front; curr != NULL; curr = next) {
		next = curr->next;
		free(curr);
	}

	// free queue itself
	free(q);
}

// add a location to queue end
void addToQ(qPtr q, LocationID location) {
	assert(q != NULL);

	Node *new = malloc(sizeof(Node));
	assert(new != NULL);

    new->location = location;
	new->next = NULL;
	if (q->front == NULL) {
		q->front = new;
    }

	if (q->end != NULL) {
		q->end->next = new;
    }

	q->end = new;
}

// remove a location from queue front
LocationID removeFromQ(qPtr q) {
	assert(q != NULL);
	assert(q->front != NULL);

    // take the item out
	LocationID location = q->front->location;

    // adjust the queue
    Node *removed = q->front;
	q->front = removed->next;
	if (q->front == NULL) {
        q->end = NULL;
    }

	// free the node
	free(removed);

    return location;
}

// check whether a queue is empty
int isQEmpty(qPtr q) {
	return (q->front == NULL);
}
