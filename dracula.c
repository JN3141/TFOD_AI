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

int locationPossible(LocationID *myTrail, LocationID nextLocation);
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

    printf("dracTrail includes:\n");
    for (i = 0; i < TRAIL_SIZE; i++) {
        printf("%d\n",dracTrail[i]);
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

        printf("the idToAbbrev output is %s",idToAbbrev(furthest));

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

        //printf("the numLocations value is %d\n", numLocations);

        // move along the array and decide which one has the greatest
        // distance total from every other hunter
        if (numLocations == 1) {
            furthest = possible[0];
        } else {
            for (i = 0; i < numLocations; i++) {
                if (sums[possible[i]] > sums[furthest] ||
                    furthest == UNKNOWN_LOCATION         ) {
                    furthest = possible[i];
                }
            }

            for (i = 0; i < numLocations; i++) {
                if ((sums[possible[i]] > sums[backup] ||
                    backup == UNKNOWN_LOCATION) &&
                    possible[i] != furthest) {
                    backup = possible[i];
                }
            }
        }

        printf("drac was at %s\n", idToAbbrev(dracLoc));
        printf("dracTrail[0] is %s\n", idToAbbrev(dracTrail[0]));
        printf("furthest is %s\n", idToAbbrev(furthest));
        printf("backup is %d\n", backup);

        if (dracTrail[0] == furthest &&
            locationPossible(dracTrail,furthest) &&
            (hidePossible(fakeTrail, dracLoc) ||
             doubleBackPossible(fakeTrail))) { // needs to be a DB1 or a HI
            printf("did a thing\n");
            if (hidePossible(fakeTrail, dracLoc)) {
                printf("dracTrail[0] is %d\n", dracTrail[0]);
                printf("furthest is %d\n", furthest);
                registerBestPlay("HI","");
            } else if (doubleBackPossible(fakeTrail)) {
                printf("i went here\n");
                registerBestPlay("D1","");
            }
        } else if (furthest != UNKNOWN_LOCATION &&
                   locationPossible(dracTrail,furthest)) { // just go to furthest
            printf("just did furthest lmao\n");
            registerBestPlay(idToAbbrev(furthest),"");
        } else if (backup != UNKNOWN_LOCATION &&
                   locationPossible(dracTrail,backup)) { // go to the backup
            printf("running on the backup\n");
            registerBestPlay(idToAbbrev(backup),"");
        } else if (doubleBackPossible(fakeTrail)) { // no choices i want? can I DB?
            if (dracTrail[4] != UNKNOWN_LOCATION &&
                findDist(europe,dracTrail[4],dracLoc) == 1) {
                registerBestPlay("D5","");
            } else if (dracTrail[3] != UNKNOWN_LOCATION &&
                       findDist(europe,dracTrail[3],dracLoc) == 1) {
                registerBestPlay("D4","");
            } else if (dracTrail[2] != UNKNOWN_LOCATION &&
                       findDist(europe,dracTrail[2],dracLoc) == 1) {
                registerBestPlay("D3","");
            } else if (dracTrail[1] != UNKNOWN_LOCATION &&
                       findDist(europe,dracTrail[1],dracLoc) == 1) {
                registerBestPlay("D2","");
            }
        } else if (numLocations == 0) { // so i can't DB, HI, go to backup, so that means i can only tele right?
            registerBestPlay("TP","");
        } else { // missed a case?
            //last minute need to do something
	        srand(time(NULL));
	        int r = rand();
	        registerBestPlay(idToAbbrev(possible[r%numLocations]),"");
        }
    }
}

int locationPossible(LocationID *myTrail, LocationID nextLocation) {
	int i;

	for (i = 0; i < TRAIL_SIZE - 1; i++) {
		if (myTrail[i] == nextLocation) {
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
    for (i = 0; i < TRAIL_SIZE - 1; i++) { // can't have a hide in trail
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
