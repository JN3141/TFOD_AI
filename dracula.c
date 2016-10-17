// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"
#include "Map.h"
#include <time.h>
#include "Places.h"

#define TRUE 1
#define FALSE 0

// THE STRAT IS TO JUST KEEP AS MUCH DISTANCE AS POSSIBLE FROM OTHER
// HUNTERS

int hidePossible(LocationID *fakeTrail, LocationID currentLocation);
int doubleBackPossible(LocationID *fakeTrail);

void decideDraculaMove(DracView gameState) {

    // collecting info...
    // grabbing hunter locations...
    LocationID godalmingLoc = whereIs(gameState, PLAYER_LORD_GODALMING);
    LocationID sewardLoc = whereIs(gameState, PLAYER_DR_SEWARD);
    LocationID helsingLoc = whereIs(gameState, PLAYER_VAN_HELSING);
    LocationID harkerLoc = whereIs(gameState, PLAYER_MINA_HARKER);

    // grabbing dracula's trail
    int dracTrail[TRAIL_SIZE];
    int fakeTrail[TRAIL_SIZE];
    giveMeTheTrail(gameState, PLAYER_DRACULA, dracTrail);
    giveMeTheFakeTrail(gameState, fakeTrail);

    int i;

    printf("fakeTrail includes:\n");
    for (i = 0; i < TRAIL_SIZE; i++) {
        printf("%d\n",fakeTrail[i]);
    }

    // gettng basic data...
    int round = giveMeTheRound(gameState);
    LocationID dracLoc = whereIs(gameState, PLAYER_DRACULA);
    // int hp = howHealthyIs(gameState, PLAYER_DRACULA);
    Map europe = newMap();

    // set up all the arrays for keeping track of distances between
    // each hunter and the possible locations

    int furthest = UNKNOWN_LOCATION;
    int backup = UNKNOWN_LOCATION;
    int godalming[NUM_MAP_LOCATIONS];
    for (i = 0; i <= MAX_MAP_LOCATION; i++) {
        godalming[i] = findDist(europe, godalmingLoc, i);
    }

    int seward[NUM_MAP_LOCATIONS];
    for (i = 0; i <= MAX_MAP_LOCATION; i++) {
        seward[i] = findDist(europe, sewardLoc, i);
    }

    int helsing[NUM_MAP_LOCATIONS];
    for (i = 0; i <= MAX_MAP_LOCATION; i++) {
        helsing[i] = findDist(europe, helsingLoc, i);
    }

    int harker[NUM_MAP_LOCATIONS];
    for (i = 0; i <= MAX_MAP_LOCATION; i++) {
        harker[i] = findDist(europe, harkerLoc, i);
    }

    // sums the distances to each place for every hunter
    int sums[NUM_MAP_LOCATIONS];
    for (i = 0; i <= MAX_MAP_LOCATION; i++) {
        sums[i] = godalming[i] + seward[i] + helsing[i] + harker[i];
    }

    if (round == 0) {
        // just find the place furthest from everyone and stick dracula
        // there
        for (i = 0; i <= MAX_MAP_LOCATION; i++) {
            if (sums[i] > sums[furthest]) {
                furthest = i;
            }
        }

        //printf("the idToAbbrev output is %s",idToAbbrev(furthest));

        registerBestPlay(idToAbbrev(furthest),"");

    } else {
        // keep the greatest distance possible as per usual
        // get an array of possible locations
        int numLocations;
        LocationID *possible = whereCanIgo(gameState, &numLocations,
                                                TRUE, TRUE);
        printf("possibilities include:\n");
        for (i = 0; i < numLocations; i++) {
            printf("%s\n",idToAbbrev(possible[i]));
        }

        printf("the numLocations value is %d\n", numLocations);
        // move along the array and decide which one has the greatest
        // distance total from every other hunter
        for (i = 0; i < numLocations; i++) {
            if (sums[possible[i]] > sums[furthest] ||
                furthest == UNKNOWN_LOCATION         ) {
                furthest = possible[i];
            }

            if (backup == UNKNOWN_LOCATION &&
                possible[i] != furthest) {
                backup = possible[i];
                printf("backup is %d\n", backup);
            }

        }

        printf("drac was at %s\n", idToAbbrev(dracLoc));
        printf("dracTrail[0] is %s\n", idToAbbrev(dracTrail[0]));
        printf("furthest is %s\n", idToAbbrev(furthest));

        if (dracTrail[0] == furthest) {
            printf("did a thing\n");
            if (hidePossible(fakeTrail, dracLoc)) {
                printf("dracTrail[0] is %d\n", dracTrail[0]);
                printf("furthest is %d\n", furthest);
                registerBestPlay("HI","");
            } else if (backup != UNKNOWN_LOCATION) {
                printf("dracTrail[0] is %d\n", dracTrail[0]);
                printf("furthest is %d\n", furthest);
                printf("could not hide, so restoring to backup %d\n", backup);
                registerBestPlay(idToAbbrev(backup),"");
            } else if (doubleBackPossible(fakeTrail)) {
                registerBestPlay("D1","");
            }
        } else {
            printf("just did furthest lmao\n");
            registerBestPlay(idToAbbrev(furthest),"");
        }

    }
}

int locationPossible(LocationID *myTrail, LocationID nextLocation) {
	int i;

	for (i=0; i < TRAIL_SIZE - 1; i++){
		if (myTrail[i] == nextLocation){
			return FALSE;
		}
	}

    return TRUE;
}

int hidePossible(LocationID *fakeTrail, LocationID currentLocation) {
	if (currentLocation == UNKNOWN_LOCATION){ // invalid position
        printf("I'm giving a false\n");
        return FALSE;
	} else if (isSea(currentLocation) == TRUE){ // can't hide at sea
        printf("I'm giving a false\n");
        return FALSE;
	} else if (currentLocation == SEA_UNKNOWN){ // invalid
        printf("I'm giving a false\n");
        return FALSE;
	}

	int i;

    /*
    for (i = 1; i < TRAIL_SIZE; i++) { // can't have a hide in trail
        printf ("%d\n",myTrail[i]);
        if (myTrail[i] == myTrail[i-1]) {
            printf("I'm giving a false\n");
            return FALSE;
        }
    }*/

    for (i = 0; i < TRAIL_SIZE; i++) { // can't have a hide in trail
        printf ("%d\n",fakeTrail[i]);
        if (fakeTrail[i] == HIDE) {
            printf("I'm giving a false\n");
            return FALSE;
        }
    }

    printf("I'm giving a true\n");
	return TRUE;
}

int doubleBackPossible(LocationID *fakeTrail) {
	int i;

	for (i = 0; i < TRAIL_SIZE - 1; i++) { // can't have doubleback
		if (fakeTrail[i] >= DOUBLE_BACK_1 &&
            fakeTrail[i] <= DOUBLE_BACK_5) {
			return FALSE;
		}
	}
    return TRUE;
}




/*
THIS IS THE OLD VERSION
void decideDraculaMove(DracView gameState) {

    // collecting info...
    // grabbing hunter locations...
    LocationID godalmingLoc = whereIs(gameState, PLAYER_LORD_GODALMING);
    LocationID sewardLoc = whereIs(gameState, PLAYER_DR_SEWARD);
    LocationID helsingLoc = whereIs(gameState, PLAYER_VAN_HELSING);
    LocationID harkerLoc = whereIs(gameState, PLAYER_MINA_HARKER);

    // gettng basic data...
    int round = giveMeTheRound(gameState);
    // int hp = howHealthyIs(gameState, PLAYER_DRACULA);
    Map europe = newMap();

    // set up all the arrays for keeping track of distances between
    // each hunter and the possible locations
    int i;
    int furthest = 0;
    int godalming[NUM_MAP_LOCATIONS];
    for (i = 0; i <= MAX_MAP_LOCATION; i++) {
        godalming[i] = findDist(europe, godalmingLoc, i);
    }

    int seward[NUM_MAP_LOCATIONS];
    for (i = 0; i <= MAX_MAP_LOCATION; i++) {
        seward[i] = findDist(europe, sewardLoc, i);
    }

    int helsing[NUM_MAP_LOCATIONS];
    for (i = 0; i <= MAX_MAP_LOCATION; i++) {
        helsing[i] = findDist(europe, helsingLoc, i);
    }

    int harker[NUM_MAP_LOCATIONS];
    for (i = 0; i <= MAX_MAP_LOCATION; i++) {
        harker[i] = findDist(europe, harkerLoc, i);
    }

    // sums the distances to each place for every hunter
    int sums[NUM_MAP_LOCATIONS];
    for (i = 0; i <= MAX_MAP_LOCATION; i++) {
        sums[i] = godalming[i] + seward[i] + helsing[i] + harker[i];
    }



    if (round == 0) {
        // just find the place furthest from everyone and stick dracula
        // there
        for (i = 0; i <= MAX_MAP_LOCATION; i++) {
            if (sums[i] > sums[furthest]) {
                furthest = i;
            }
        }

        registerBestPlay(idToAbbrev(furthest),"");

    } else {
        // keep the greatest distance possible as per usual
        int numLocations;
        LocationID *possible = whereCanIgo(gameState, &numLocations,
                                                TRUE, FALSE);
        for (i = 0; i < numLocations; i++) {
            if (sums[possible[i]] > sums[furthest]) {
                furthest = possible[i];
            }
        }
        registerBestPlay(idToAbbrev(furthest),"");
    }
    registerBestPlay("CD","Mwuhahahaha");

	//Initialise variables
	LocationID bestPlay = CASTLE_DRACULA;
	int teleported = FALSE;

	int trail[TRAIL_SIZE];
	giveMeTheTrail(gameState, PLAYER_DRACULA, trail);

	if (teleportInTrail(trail) == FALSE && giveMeTheRound(gameState) > 0) {
		registerBestPlay("CD","Mwuhahahaha");
		teleported = TRUE;
	} //else{...}

	// last minute need to do something
	int numLocations;
	LocationID *next = whereCanIgo(gameState, &numLocations, TRUE, FALSE);
	srand(time(NULL));
	int r = rand();

	registerBestPlay(idToAbbrev(next[r%numLocations]),"Mwuhahahaha");
}
*/
