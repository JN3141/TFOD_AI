// testGameView.c ... test the GameView ADT

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "GameView.h"

int main()
{
    int i;
    GameView gv;

    printf("Test basic empty initialisation\n");
    PlayerMessage messages1[] = {};
    gv = newGameView("", messages1);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 0);
    assert(getHealth(gv,PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    assert(getScore(gv) == GAME_START_SCORE);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == UNKNOWN_LOCATION);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula trail and basic functions\n");
    PlayerMessage messages2[] = {"Hello","Rubbish","Stuff","","Mwahahah"};
    gv = newGameView("GST.... SAO.... HZU.... MBB.... DC?....", messages2);
    assert(getCurrentPlayer(gv) == PLAYER_LORD_GODALMING);
    assert(getRound(gv) == 1);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == STRASBOURG);
    assert(getLocation(gv,PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
    assert(getLocation(gv,PLAYER_VAN_HELSING) == ZURICH);
    assert(getLocation(gv,PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
    assert(getLocation(gv,PLAYER_DRACULA) == CITY_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for encountering Dracula and hunter history\n");
    PlayerMessage messages3[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!"};
    gv = newGameView("GST.... SAO.... HCD.... MAO.... DGE.... GGED...", messages3);
    assert(getLocation(gv,PLAYER_DRACULA) == GENEVA);
    assert(getHealth(gv,PLAYER_LORD_GODALMING) == 5);
    assert(getHealth(gv,PLAYER_DRACULA) == 30);
    assert(getLocation(gv,PLAYER_LORD_GODALMING) == GENEVA);
    LocationID history[TRAIL_SIZE];
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == GENEVA);
    assert(history[1] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_LORD_GODALMING,history);
    assert(history[0] == GENEVA);
    assert(history[1] == STRASBOURG);
    assert(history[2] == UNKNOWN_LOCATION);
    getHistory(gv,PLAYER_DR_SEWARD,history);
    assert(history[0] == ATLANTIC_OCEAN);
    assert(history[1] == UNKNOWN_LOCATION);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula doubling back at sea, and losing blood points (Hunter View)\n");
    PlayerMessage messages4[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DS?.... "
                     "GST.... SST.... HST.... MST.... DD1....", messages4);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_1);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_1);
    assert(history[1] == SEA_UNKNOWN);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for Dracula doubling back at sea, and losing blood points (Drac View)\n");
    PlayerMessage messages5[] = {"Hello","Rubbish","Stuff","","Mwahahah","Aha!","","","","Back I go"};
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DEC.... "
                     "GST.... SST.... HST.... MST.... DD1....", messages5);
    assert(getLocation(gv,PLAYER_DRACULA) == DOUBLE_BACK_1);
    getHistory(gv,PLAYER_DRACULA,history);
    assert(history[0] == DOUBLE_BACK_1);
    assert(history[1] == ENGLISH_CHANNEL);
    assert(getHealth(gv,PLAYER_DRACULA) == GAME_START_BLOOD_POINTS - 4);
    assert(getCurrentPlayer(gv) == 0);
    printf("passed\n");
    disposeGameView(gv);

    printf("Test for connections\n");
    int size, seen[NUM_MAP_LOCATIONS], *edges;
    gv = newGameView("", messages1);

    printf("Checking Galatz road connections\n");
    edges = connectedLocations(gv,&size,GALATZ,PLAYER_LORD_GODALMING,0,1,0,0);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i< size ; i++) seen[edges[i]] = 1;
    assert(size == 5); assert(seen[GALATZ]); assert(seen[CONSTANTA]);
    assert(seen[BUCHAREST]); assert(seen[KLAUSENBURG]); assert(seen[CASTLE_DRACULA]);
    free(edges);

    printf("Checking Ionian Sea sea connections\n");
    edges = connectedLocations(gv,&size,IONIAN_SEA,PLAYER_LORD_GODALMING,0,0,0,1);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert(size == 7); assert(seen[IONIAN_SEA]); assert(seen[BLACK_SEA]);
    assert(seen[ADRIATIC_SEA]); assert(seen[TYRRHENIAN_SEA]);
    assert(seen[ATHENS]); assert(seen[VALONA]); assert(seen[SALONICA]);
    free(edges);

    printf("Checking Athens rail connections (none)\n");
    edges = connectedLocations(gv,&size,ATHENS,PLAYER_LORD_GODALMING,0,0,1,0);
    assert(size == 1);
    assert(edges[0] == ATHENS);
    free(edges);

    printf ("Checking 3 max rail jumps, Paris start\n");
    edges = connectedLocations(gv, &size, PARIS, PLAYER_LORD_GODALMING, 3,1,1,1);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert (size == 14); assert(seen[PARIS]); assert(seen[BRUSSELS]); assert(seen[LE_HAVRE]);
    assert(seen[NANTES]); assert(seen[CLERMONT_FERRAND]); assert(seen[GENEVA]);
    assert(seen[STRASBOURG]); assert(seen[MARSEILLES]); assert (seen[BORDEAUX]);
    assert (seen[SARAGOSSA]); assert (seen[BARCELONA]); assert (seen[MADRID]);
    assert (seen[COLOGNE]); assert (seen[FRANKFURT]);
    free(edges);

    printf ("Checking 2 max rail jumps, Szeged start, Mina Harker\n");
    edges = connectedLocations(gv, &size, SZEGED, PLAYER_MINA_HARKER, 7,1,1,1);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert (size == 11); assert (seen[SZEGED]); assert (seen[KLAUSENBURG]); assert (seen[BUDAPEST]);
    assert (seen[BELGRADE]); assert (seen[ST_JOSEPH_AND_ST_MARYS]);
    assert (seen[ZAGREB]); assert (seen[VIENNA]); assert (seen[SOFIA]); assert (seen[BUCHAREST]);
    assert (seen[GALATZ]); assert (seen[CONSTANTA]);
    free(edges);

    printf ("Checking 3 max rail jumps, London start, Van Helsing\n");
    edges = connectedLocations(gv, &size, LONDON, PLAYER_VAN_HELSING, 5,1,1,1);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert (size == 7); assert (seen[LONDON]); assert (seen[PLYMOUTH]); assert (seen[SWANSEA]);
    assert (seen[MANCHESTER]); assert (seen[LIVERPOOL]); assert (seen[EDINBURGH]);
    assert (seen[ENGLISH_CHANNEL]);
    free(edges);

    printf ("Checking 1 max rail jump, Alicante start, Dr Seward\n");
    edges = connectedLocations(gv, &size, ALICANTE, PLAYER_DR_SEWARD, 0,1,1,1);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert (size == 6); assert (seen[ALICANTE]); assert (seen[SARAGOSSA]); assert (seen[MADRID]);
    assert (seen[GRANADA]); assert (seen[MEDITERRANEAN_SEA]);
    free(edges);

    printf ("Checking 1 max rail jump, Atlantic Ocean start, Dr Seward\n");
    edges = connectedLocations(gv, &size, ATLANTIC_OCEAN, PLAYER_DR_SEWARD, 0,1,1,1);
    memset(seen, 0, NUM_MAP_LOCATIONS*sizeof(int));
    for (i = 0; i < size; i++) seen[edges[i]] = 1;
    assert (size == 9); assert (seen[ATLANTIC_OCEAN]); assert (seen[BAY_OF_BISCAY]);
    assert (seen[ENGLISH_CHANNEL]); assert (seen[GALWAY]); assert (seen[LISBON]);
    assert (seen[IRISH_SEA]); assert (seen[NORTH_SEA]); assert (seen[MEDITERRANEAN_SEA]);
    assert (seen[CADIZ]);
    free(edges);

    printf("passed\n");
    disposeGameView(gv);

	printf("Waiting to find Dracula\n");
	PlayerMessage messages6[] = {"", "", "", "", ""};
	gv = newGameView("GGE.... SGE.... HGE.... MGE.... DSJ.... "
					 "GST.... SST.... HST.... MST.... DC?.... "
					 "GFF.... SFF.... HFF.... MFF.... DC?.... "
					 "GCO.... SCO.... HCO.... MCO.... DHI.... "
					 "GBR.... SBR.... HBR.... MBR.... DC?.... "
					 "GBR.... SBR.... HBR.... MBR.... DC?T...", messages6);
	LocationID dracTrail[TRAIL_SIZE];
	getHistory(gv, PLAYER_DRACULA, dracTrail);
	assert(dracTrail[TRAIL_SIZE - 1] == SARAJEVO);
	printf("passed\n");
	disposeGameView(gv);

	printf("Getting Dracula with two players\n");
	gv = newGameView("GGE.... SGE.... HGE.... MGE.... DSJ.... "
					 "GST.... SST.... HST.... MST.... DC?.... "
					 "GFF.... SFF.... HFF.... MFF.... DC?.... "
					 "GCO.... SCO.... HCO.... MCO.... DHI.... "
					 "GBR.... SBR.... HBR.... MBR.... DC?.... "
					 "GBR.... SBR.... HBR.... MBR.... DSTT... "
					 "GSTTD.. SSTD... HCO.... MCO.... DPA....", messages6);
	assert(getScore(gv) == 359);
	assert(getLocation(gv, PLAYER_DRACULA) == PARIS);
	assert(getHealth(gv, PLAYER_LORD_GODALMING) == 3);
	assert(getHealth(gv, PLAYER_DR_SEWARD) == 5);
	assert(getHealth(gv, PLAYER_VAN_HELSING) == 9);
	assert(getHealth(gv, PLAYER_MINA_HARKER) == 9);
	assert(getHealth(gv, PLAYER_DRACULA) == 20);
	printf("passed\n");
	disposeGameView(gv);


	printf("Long game test\n");
	gv = newGameView("GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?.... "
					 "GGE.... SGE.... HGE.... MGE.... DC?....", messages6);
	assert(getScore(gv) == 0);
	assert(getRound(gv) == 366);
   printf("passed\n");
	disposeGameView(gv);


	 printf("Testing that the life points do not exceed 9\n");
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DC?.... "
                     "GGE.... SGE.... HGF.... MGE.... DC?...."
							"GGE.... SGE.... HGF.... MGE.... DC?....", messages5);
	assert(getHealth(gv, PLAYER_LORD_GODALMING) == 9);
	assert(getHealth(gv, PLAYER_DR_SEWARD) == 9);
	assert(getHealth(gv, PLAYER_VAN_HELSING) == 9);
	assert(getHealth(gv, PLAYER_MINA_HARKER) == 9);
	printf("Passed\n");
	disposeGameView(gv);

	printf("Testing What happens when hunters die\n");
   gv = newGameView("GGE.... SGE.... HGE.... MGE.... DPA.... "
                    "GPAD... SGE.... HGE.... MGE.... DBU.... "
                    "GBUD... SGE.... HGE.... MGE.... DST.... "
                    "GSTD...", messages5);

   assert(getHealth(gv, PLAYER_DRACULA) ==
           GAME_START_BLOOD_POINTS - 3 * LIFE_LOSS_HUNTER_ENCOUNTER);
   assert(getHealth(gv, PLAYER_LORD_GODALMING)  <= 0);
   assert(getScore(gv) == GAME_START_SCORE - SCORE_LOSS_HUNTER_HOSPITAL - 3);
   assert(getLocation(gv, PLAYER_LORD_GODALMING) == ST_JOSEPH_AND_ST_MARYS);
	printf("passed\n");
	disposeGameView(gv);



    printf("Testing post condition hunter dying\n");
    gv = newGameView("GGE.... SGE.... HGE.... MGE.... DPA.... "
                    "GPAD... SGE.... HGE.... MGE.... DBU.... "
                    "GBUD... SGE.... HGE.... MGE.... DST.... "
                    "GSTD... SGE.... HGE.... MGE.... DCO.... "
                    "GFR....", messages5);
    assert(getHealth(gv, PLAYER_DRACULA) ==
            GAME_START_BLOOD_POINTS - 3 * LIFE_LOSS_HUNTER_ENCOUNTER);
    assert(getHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
    assert(getScore(gv) == GAME_START_SCORE - SCORE_LOSS_HUNTER_HOSPITAL - 4);
    assert(getLocation(gv, PLAYER_LORD_GODALMING) == FRANKFURT);
    printf("passed\n");
    disposeGameView(gv);


	//GAME SCORE
	//Dracula vampire matures after 6 rounds: -13 points
	printf("Testing the score decreases for mature vampire\n");

    gv = newGameView("GST.... SAO.... HZU.... MBB.... DCF.V.. "
                     "GST.... SAO.... HZU.... MBB.... DBOT... "
                     "GST.... SAO.... HZU.... MBB.... DTOT... "
                     "GST.... SAO.... HZU.... MBB.... DBAT... "
                     "GST.... SAO.... HZU.... MBB.... DSRT... "
                     "GST.... SAO.... HZU.... MBB.... DCDT.V. "
				 	 "GST.... SAO.... HZU.... MBB.... DHI.V.. ", messages5);

    //printf("Score %d \n", getScore(gv));
    assert(getScore(gv) == GAME_START_SCORE - 7 - SCORE_LOSS_VAMPIRE_MATURES);
    printf("passed\n");
    disposeGameView(gv);
    printf("\n");

    //LOCATION
    //Hunter life points is 0: location is st joseph/ mary
    //Check hunter life points

    //DRACULA
    //Dracula starts with 40 blood points
    //Dracula encounters hunter: - 10 blood points
    //Dracula is at sea at end of turn: -2 points
    //Dracula is ends turn in Castle Dracula: +10 points
    //Dracula has no blood points: game is over Score= final score

    //No health lost while both are at sea/ trail stuffs

    return 0;
}
