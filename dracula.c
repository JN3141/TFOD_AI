// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"
#include "Map.h"
#include <time.h>
#include "Places.h"

// THE STRAT IS TO JUST KEEP AS MUCH DISTANCE AS POSSIBLE FROM OTHER
// HUNTERS

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
        LocationID *possibilities = whereCanIgo(gameState, &numLocations, TRUE, FALSE);
        for (i = 0; i < numLocations; i++) {
            if (sums[possibilities[i]] > sums[furthest]) {
                furthest = possibilities[i];
            }
        }
        registerBestPlay(idToAbbrev(furthest),"");
    }

    registerBestPlay("CD","Mwuhahahaha");
/*
	//Initialise variables
	LocationID bestPlay = CASTLE_DRACULA;
	int teleported = FALSE;

	int trail[TRAIL_SIZE];
	giveMeTheTrail(gameState, PLAYER_DRACULA, trail);

	if (teleportInTrail(trail) == FALSE && giveMeTheRound(gameState) > 0) {
		registerBestPlay("CD","Mwuhahahaha");
		teleported = TRUE;
	} //else{...}
*/
	// last minute need to do something
	int numLocations;
	LocationID *next = whereCanIgo(gameState, &numLocations, TRUE, FALSE);
	srand(time(NULL));
	int r = rand();

	registerBestPlay(idToAbbrev(next[r%numLocations]),"Mwuhahahaha");
}

int locationPossible(LocationID *myTrail, LocationID nextLocation) {
	int i;

	for(i=0; i < TRAIL_SIZE - 1; i++){
		if(myTrail[i] == nextLocation){
			return FALSE;
		}
	}

    return TRUE;
}

int hidePossible(LocationID *myTrail, LocationID currentLocation){
	if (currentLocation == UNKNOWN_LOCATION){ // invalid position
		return FALSE;
	} else if (isSea(currentLocation) == TRUE){ //c an't hide at sea
		return FALSE;
	} else if (currentLocation == SEA_UNKNOWN){ // invalid
		return FALSE;
	}

	int i;

	for (i = 0; i < TRAIL_SIZE - 1; i++){ // can't have a hide in trail
		if (myTrail[i] == HIDE){
			return FALSE;
		}
	}

	return TRUE;
}

int doubleBackPossible(LocationID *myTrail, LocationID currentLocation) {
	int i;

	for (i = 0; i < TRAIL_SIZE - 1; i++){ // can't have doubleback
		if (myTrail[i] >= DOUBLE_BACK_1 && myTrail[i] <= DOUBLE_BACK_5){
			return FALSE;
		}
	}
    return TRUE;
}
