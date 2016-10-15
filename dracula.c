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
/*
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
	*/

	//last minute need to do something
	int numLocations;
	LocationID *next = whereCanIgo(gameState, &numLocations, TRUE, FALSE);
	srand(time(NULL));
	int r = rand();

	registerBestPlay(idToAbbrev(next[r%numLocations]),"Mwuhahahaha");

}
