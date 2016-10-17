// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "HunterView.h"
#include <time.h>
#include "Map.h"

static LocationID whereIsDrac(HunterView gameState);
static LocationID wheretoGo(HunterView gameState, LocationID *Plays, LocationID dracLoc, int numLocations);
static int randomMove(int numLocations,LocationID *playerTrail, LocationID *Plays);

void decideHunterMove(HunterView gameState) {

    // collecting info...
    PlayerID hunter = whoAmI(gameState);
  //  LocationID hunterLoc = whereIs(gameState, hunter);
    int round = giveMeTheRound(gameState);
    int hp = howHealthyIs(gameState, hunter);
	
	LocationID playerTrail[TRAIL_SIZE];
	giveMeTheTrail(gameState,hunter ,
                    playerTrail);
	
	LocationID dracTrail[TRAIL_SIZE];
	giveMeTheTrail(gameState, PLAYER_DRACULA,
                    dracTrail);
	int x=0;
	for (x=0; x<TRAIL_SIZE; x++){
		printf("dracTrail %d %d \n", x, dracTrail[x]);
	}
	 //just trying to not get error message from var
	 //printf("%d", hunterLoc);


    if (round == 0){
		if (hunter == PLAYER_LORD_GODALMING) {
			registerBestPlay("ST","");
		} else if (hunter == PLAYER_DR_SEWARD) {
			registerBestPlay("CD","");
		} else if (hunter == PLAYER_VAN_HELSING) {
			registerBestPlay("SR","");
		} else {
            registerBestPlay("LV","");
        }

	}else{
		
		LocationID hunterLoc = whereIs(gameState, hunter);
		int numLocations;
		LocationID *Plays= whereCanIgo(gameState, &numLocations,
                        TRUE, TRUE, TRUE);
							
		LocationID dracLoc=whereIsDrac(gameState);
		if (dracLoc != -1){
			LocationID bestPlay=wheretoGo(gameState, Plays, dracLoc,numLocations);
			registerBestPlay(idToAbbrev(bestPlay),"Dracula's at" );
			
		}else{
			
			int r=randomMove(numLocations,playerTrail,Plays );
			registerBestPlay(idToAbbrev(Plays[r]),"");
			
		}
		
		//if health is low, rest!
		if (hp <6){
			registerBestPlay(idToAbbrev(whereIs(gameState, hunter)),"");
		}
			
		
		//player guarding dracula's castle
		if (hunter == PLAYER_DR_SEWARD){
			//if he's at castle, gotta move
			if (whereIs(gameState,PLAYER_DRACULA)==CASTLE_DRACULA ){
				int r=randomMove(numLocations,playerTrail,Plays );
				registerBestPlay(idToAbbrev(Plays[r]),"");
			}else if (hunterLoc==CASTLE_DRACULA){
				//stay put
				registerBestPlay("CD","chillin");
			}else{
				int r=randomMove(numLocations,playerTrail,Plays );
				registerBestPlay(idToAbbrev(Plays[r]),"");
			}
		}
		//research
		//if (round==7)
		
	}
}


//whether we know whether drac is somewhere or not
//If unknown, returns -1

static LocationID whereIsDrac(HunterView gameState){
	LocationID drac=-1;
	if (whereIs(gameState,PLAYER_DRACULA)<= MAX_MAP_LOCATION && whereIs(gameState,PLAYER_DRACULA)>=MIN_MAP_LOCATION){
		drac=whereIs(gameState,PLAYER_DRACULA);
	}
	printf ("Drac location %d", drac);
	return drac;
}

static LocationID wheretoGo(HunterView gameState, LocationID *Plays , LocationID dracLoc, int numLocations){
	
	Map europe = newMap();
	int i=0;
	LocationID whereToGo= Plays[0];
			
	int min= findDist(europe, Plays[i], dracLoc);
	for (i=0; i<numLocations; i++){
		int comp=findDist(europe, Plays[i], dracLoc);	
		if (comp < min){
			whereToGo=Plays[i];
		}
	}
	return whereToGo;
			
}

//doesn't go backwards
static int randomMove(int numLocations,LocationID *playerTrail,LocationID *Plays ){
	srand(time(NULL));
	int r = rand()%numLocations;
	if (Plays[r]==playerTrail[0]) r++;
   r=r%numLocations;	
	
	return r;
	
}

/*
void decideHunterMove(HunterView gameState) {

    // collecting info...
    PlayerID hunter = whoAmI(gameState);
    // LocationID hunterLoc = whereIs(gameState, hunter);
    int round = giveMeTheRound(gameState);
    //int hp = howHealthyIs(gameState, hunter);



    if (round == 0){
		if (hunter == PLAYER_LORD_GODALMING) {
			registerBestPlay("ST","");
		} else if (hunter == PLAYER_DR_SEWARD) {
			registerBestPlay("SZ","");
		} else if (hunter == PLAYER_VAN_HELSING) {
			registerBestPlay("SR","");
		} else {
            registerBestPlay("LV","");
        }

	} else if (hunter == PLAYER_LORD_GODALMING || hunter == PLAYER_DR_SEWARD) {
        // adjusts location of lord goldalming and dr seward
		PlayerID curr;

        //setting the current player
		if (whoAmI(gameState)== PLAYER_LORD_GODALMING) curr = PLAYER_LORD_GODALMING;
		if (whoAmI(gameState)== PLAYER_DR_SEWARD) curr = PLAYER_DR_SEWARD;

		if (whereIs(gameState, curr)== ST_JOSEPH_AND_ST_MARYS){
				registerBestPlay("SZ","");

		}else if (whereIs(gameState, curr)== SZEGED){
				registerBestPlay("KL","");
			
		}else if (giveMeTheRound(gameState)%2 == 0 && giveMeTheRound(gameState) != 0){


			// if the player is in galatz, moves to klausenburg
			if (whereIs(gameState, curr)== GALATZ){
				registerBestPlay("KL","");

			//if player is in klausenburg, moves to
			}else if (whereIs(gameState, curr)== KLAUSENBURG){
				registerBestPlay("GA","");

			}

		} else {
			// if the player is in galatz, moves to klausenburg
			if (whereIs(gameState, curr)== GALATZ) {
				registerBestPlay("GA","");

			// if player is in klausenburg, moves to
			} else if (whereIs(gameState, curr)== KLAUSENBURG) {
				registerBestPlay("KL","");

			} else if (whereIs(gameState, curr)== KLAUSENBURG &&
                       curr== PLAYER_LORD_GODALMING) {
				registerBestPlay("GA","");
			}
		}

	} else {
		registerBestPlay("GE","");
	}
}*/
