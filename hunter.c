// hunter.c
// Implementation of your "Fury of Dracula" hunter AI

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "HunterView.h"

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
			/*
		}else if (giveMeTheRound(gameState)%2 == 0 && giveMeTheRound(gameState) != 0){


			// if the player is in galatz, moves to klausenburg
			if (whereIs(gameState, curr)== GALATZ){
				registerBestPlay("KL","");

			//if player is in klausenburg, moves to
			}else if (whereIs(gameState, curr)== KLAUSENBURG){
				registerBestPlay("GA","");

			}
			*/
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
			/*
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
