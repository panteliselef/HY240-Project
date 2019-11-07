/*************************************************************
 * @file   Gaugamela.h                                       *
 * @author Nikolaos Batsaras <batsaras@csd.uoc.gr>  	     *
 *                                                    	     *
 * @brief  Header file for the needs of cs-240a project 2018 *
 ************************************************************/

#ifndef __GAUGAMELA_H__
#define __GAUGAMELA_H__


struct Army {
	int id;
	int type;
	int general;
	struct Army *next;
};

struct GA_Battle {
	int soldier_id;
	int horse_id;
	int general;
	struct GA_Battle *next;
};

struct Satrapy {
	int id;
	struct Army *s_army;
	struct Satrapy *next;
	struct Satrapy *prev;
	struct Army *sentinel;
};

struct S_Battle {
	int soldier_id;
	int satrapy_id;
	int type;
	struct S_Battle *next;
};

/*
 * Global variable
 *
 * A 2-Cell array containing the following pointers:
 *
 * 	* 1rst cell is a pointer to the head of Alexander's list of soldiers
 * 	* 2nd  cell is a pointer to the head of Alexander's list of horses
 */
struct Army *GA_army[2];

/* global variable, pointer to the head of Alexander's battle list */
struct GA_Battle *GA_battle_list;

/* global variable, pointer to the head of the list of Satrapies */
struct Satrapy *satrapies_list;

/* global variable, pointer to the head of the Satrapies' battle list */
struct S_Battle *S_battle_list;

/* global variable, array of pointers to struct GA_Battle sublists */
struct GA_Battle *GA_Generals[5];


#endif /* __GAUGAMELA_H__ */
