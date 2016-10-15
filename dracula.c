// dracula.c
// Implementation of your "Fury of Dracula" Dracula AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "DracView.h"
#include "Map.h"
#include <time.h>
#include "Places.h"

void decideDraculaMove(DracView gameState)
{
	// TODO ...
	// Replace the line below by something better
	registerBestPlay("CD","Mwuhahahaha");

	//Initialise variables
	LocationID bestPlay = CASTLE_DRACULA;
	int teleported = FALSE;

	//Initialise map
	GameView gameView = newGameView(NUM_MAP_LOCATIONS);
	addRoadConnections(gameView);

	int trail[TRAIL_SIZE];
	giveMeTheTrail(gameState, PLAYER_DRACULAR, trail);

	if (teleportInTrail(trail) == FALSE && giveMeTheRound(gameState) > 0) {
		registerBestPlay("CD","Mwuhahahaha");
		teleported = TRUE;
	} //else{...}

	
	//last minute need to do something
	int numLocations;
	LocationID *next = whereCanIgo(gameState, &numLocations, TRUE, FALSE);
	srand(time(NULL));
	int r = rand();
	
	registerBestPlay(idToAbbrev(next[r%numLocations]),"Mwuhahahaha");
	


}

int locationPossible(LocationID *myTrail, LocationID nextLocation){
	int i;

	for(i=0; i < TRAIL_SIZE - 1; i++){
		if(myTrail[i] == nextLocation){
			return FALSE;
		}
	}
}

int hidePossible(LocationID *myTrail, LocationID currentLocation){
	if(currentLocation == UNKNOWN_LOCATION){ //invalid position
		return FALSE;
	} else if(isSea(currentLocation) == TRUE){ //can't hide at sea
		return FALSE;
	} else if (currentLocation = SEA_UNKNOWN){ //invalid
		return FALSE;
	}

	int i;

	for(i=0; i < TRAIL_SIZE - 1; i++){ //can't have a hide in trail
		if(myTrail[i] == HIDE){
			return FALSE;
		}
	}

	return TRUE;
}

int doubleBackPossible(LocationID *myTrail, LocationID currentLocation){
	int i;

	for(i=0; i < TRAIL_SIZE - 1; i++){ //can't have doubleback
		if(myTrail[i] == doubleBack){
			return FALSE;
		}
	}
}