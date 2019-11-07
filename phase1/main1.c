/*************************************************************
 * @file   main.c                                            *
 * @author Nikolaos Batsaras <batsaras@csd.uoc.gr>  	     *
 *                                                    	     *
 * @brief  Source file for the needs of cs-240a project 2018 *
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Gaugamela.h"

#define BUFFER_SIZE 1024  /* Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints 
 * or comment to disable it */
#define DEBUG

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else  /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */





/*
 * IN THIS SECTION THERE ARE FUNCTIONS THAT I'VE CREATED
 * 
 */


/*
 * This method adds a new node to a struct Army List
 * and keeps the List sorted
 * @param first		The first node of the list
 * @param sid     The soldier's id
 * @param type    The soldier's type
 * @param general The general the soldier obeys
*/
int register_GA(struct Army **first, int sid, int type, int general) {
	struct Army *res = (struct Army *)malloc(sizeof(struct Army));
	struct Army *tmp = *first;
	struct Army *prevTmp = NULL;
	if(res == 0 ) return 0;
	res->id = sid;
	res->type = type;
	res->general = general;
	res->next = NULL;
	if( (tmp) == NULL) {
		(*first) = res;
	}else {
		while(tmp !=NULL && tmp->id < sid) {
			prevTmp = tmp;
			tmp= tmp->next;
		} 
		if(prevTmp == NULL) {
			res->next = (*first);
			(*first) = res;
		}
		else {
			res->next = tmp;
			prevTmp->next = res;
		}
	}
	return 1;
}



/**
 * This function adds a new struct Satrapy node to the list
 * @param h     The first (header) node from satrapies_list
 * @param sid   The satrapia id for the new node
 */
struct Satrapy* Add2Satrapy(struct Satrapy *h, int sid){

    struct Satrapy* res = (struct Satrapy*)malloc(sizeof(struct Satrapy));
		struct Satrapy* tmp;

		// ftiaxno ton kombo frouro
		struct Army *guardNode = (struct Army *)malloc(sizeof(struct Army));
		guardNode->id = -1;
		guardNode->type= -1;
		guardNode->general= -1;
		guardNode->next = NULL;

		
		res->id = sid;
		res->s_army = guardNode;
		res->sentinel = guardNode;

		if(h->next == NULL && h->prev == NULL) {
			h->prev = res;
			h->next = res;
			res->prev = h;
			res->next = h;
		}else {
			tmp = h->prev;
			res->next = h;
			h->prev = res;
			tmp->next = res;
			res->prev = tmp;
		}
		return h;
}


/**
 * This function inserts a new struct GA_Battle node at the
 * end of a list (GA_Generals[i])
 * @param first   		The first node of the GA_Generals[i]
 * @param last    		The last node of the GA_Generals[i]
 * @param sol_id		  The soldier's id
 * @param horse_id		The horse id of the soldier's horse
 * @param sol_general The soldier's general
 */
void add2Generals(struct GA_Battle **first,struct GA_Battle **last, int sol_id,
									int horse_id, int sol_general) {
	struct GA_Battle *res = (struct GA_Battle *)malloc(sizeof(struct GA_Battle));

	if(res == 0) printf("Memalloc fail\n");
	else {
		res->soldier_id = sol_id;
		res->horse_id = horse_id;
		res->general = sol_general;
		res->next = NULL;

		if((*first) == NULL) {
			(*first) = res;
			(*last) = res;
		}else {
			(*last)->next= res;
			(*last) =  res;
		}
	}
}

/**
 * This functions check if a soldier of a satrapy has type 1
 * @param sol_id The soldier's id
 * @param sat_id The satrapy's id
 * @return 1 if the type is 1
 * 				 0 otherwise
 */
int hasTypeOne(int sol_id,int sat_id) {
	
		struct Satrapy *s = satrapies_list->next;
		while(s != satrapies_list && s->id != sat_id) {
			s = s->next;
		}
		// s is now out selected node
		struct Army *q = s->s_army;
		while(q!=s->sentinel && q->id != sol_id) {
			q = q->next;
		}

		if (q->type == 1) return 1;
		return 0;
	
}


/**
 * This function prints the horse of the soldiers of GA
 * @param type		if is 0 then print soldiers if it's 1 print horses
 */
void printGA_army(int type) {
	struct Army *curr;
	struct Army *tmp;
	if(type == 0) {
		curr = GA_army[0];
		printf("\tGA Soldiers = ");
	}else if(type == 1) {
		curr = GA_army[1];
		printf("\tGA Horses = ");
	}
	tmp = curr;
	while(tmp!=NULL) {
		if(tmp==curr){
			printf("<%d,%d,%d>",tmp->id,tmp->type,tmp->general);
		}else {
			printf(", <%d,%d,%d>",tmp->id,tmp->type,tmp->general);
		}
		tmp= tmp->next;
	}
	printf("\n");
}

/**
 * This function prints all the soldiers of GA_battle_list
 * @param first		The first node of the list
 */
void printGA_Battle(struct GA_Battle *first) {
	struct GA_Battle *tmp = first;
	printf("\tGA Battle = ");
	while(tmp!=NULL) {
		if(tmp == first) printf("<%d,%d,%d>",tmp->soldier_id,tmp->horse_id,tmp->general);
		else printf(", <%d,%d,%d>",tmp->soldier_id,tmp->horse_id,tmp->general);
		tmp=tmp->next;
	}
	printf("\nDONE\n"); 
}


/**
 * This function inserts a new struct GA_Battle node at the
 * end of a list (GA_battle_list)
 * @param first   		The first node of the GA_battle_list
 * @param last    		The last node of the GA_battle_list
 * @param soldier_id  The soldier's id
 * @param soldier_gen The general the soldier obeys
 * @param soldier_gen The horse id of the horse the soldier has
 */
int Add2GA_Battle(struct GA_Battle **first,struct GA_Battle **last,
						 int soldier_id, int soldier_gen, int horse_id){

    struct GA_Battle* res = (struct GA_Battle*)malloc(sizeof(struct GA_Battle));
    struct GA_Battle* tmp;
		if (res == 0)
    {
        return 0;
        
    }else {
      res->soldier_id = soldier_id;
      res->horse_id = horse_id;
			res->general = soldier_gen; 
      res->next = NULL;

			if((*first) == NULL) {
          (*first) = res;
          (*last) = res;
      }else {
          (*last)->next= res;
          (*last) =  res;
      }
		}
		return 1;
}
/*
 * END
 * 
 */

/**
 * @brief Optional function to initialize data structures that 
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int initialize (void)
{
	GA_army[0] = NULL;
	GA_army[1] = NULL;
	GA_battle_list = NULL;
	S_battle_list = NULL;
	//insert a header node to satrapies_list
	satrapies_list = NULL;
	struct Satrapy *satrapy_header = (struct Satrapy *)malloc(sizeof(struct Satrapy));
	if(satrapy_header==0) return 0;
	satrapy_header->prev = NULL;
	satrapy_header->next = NULL;
	satrapy_header->s_army = NULL;
	satrapy_header->sentinel = NULL;
	satrapies_list = satrapy_header;

	GA_Generals[0] = NULL;
	GA_Generals[1] = NULL;
	GA_Generals[2] = NULL;
	GA_Generals[3] = NULL;
	GA_Generals[4] = NULL;






	return 1;
}

/**
 * @brief Register Alexander the Great soldier 
 *
 * @param sid     The soldier's id
 * @param type    The soldier's type
 * @param general The general the soldier obeys
 *
 * @return 1 on success
 *         0 on failure
 */
int register_GA_soldier (int sid, int type, int general)
{
	int v = register_GA(&GA_army[0], sid, type,general); //saves the result
	struct Army *q = GA_army[0];
	if( v == 1){
		//print the all the existing nodes
		printf("\t GA Soliers = ");
		while(q!=NULL) {
			if(q == GA_army[0]) printf("<%d,%d,%d>", q->id,q->type,q->general);
			else printf(", <%d,%d,%d>", q->id,q->type,q->general);
			q = q->next;
		}
		printf("\nDONE\n");
		return 1;
	}
	else return 0;
}

/**
 * @brief Register Alexander the Great horse 
 *
 * @param sid     The horse's id
 * @param type    Always the value of 0
 * @param general The general the soldier obeys
 *
 * @return 1 on success
 *         0 on failure
 */
int register_GA_horse (int hid, int type, int general)
{
	int v = register_GA(&GA_army[1], hid, type,general);//saves the result
	struct Army *q = GA_army[1];
	if( v == 1){
		//print the all the existing nodes
		printf("\t GA Horses = ");
		while(q!=NULL) {
			if(q == GA_army[1]) printf("<%d,%d,%d>", q->id,q->type,q->general);
			else printf(", <%d,%d,%d>", q->id,q->type,q->general);
			q = q->next;
		}
		printf("\nDONE\n");
		return 1;
	}
	else return 0;

}


/**
 * @brief Register Satrapy
 *
 * @param sid The Satrapy's id
 *
 * @return 1 on success
 *         0 on failure
 */
int register_satrapy (int sid) {
	struct Satrapy *tmp;
	satrapies_list = Add2Satrapy(satrapies_list,sid);
	
	// print all the struct Satrapy nodes in the lists
	tmp = satrapies_list->next;
	printf("\tSatrapies = ");
	while(tmp!=satrapies_list) {
		if(tmp == satrapies_list->next) printf("<%d>",tmp->id);
		else printf(", <%d>",tmp->id);
		tmp = tmp->next;
	} 
	printf("\nDONE\n");
	
	return 1;
}
/**
 * @brief Register Satrapy soldier
 *
 * @param sid     The id of the Satrapy the soldier belongs to
 * @param aid     The soldier's id
 * @param type    The type of soldier
 * @param general The general the soldier obeys
 *
 * @return 1 on success
 *         0 on failure
 */
int register_S_soldier (int sid, int aid, int type, int general)
{
	struct Satrapy *tmp  = satrapies_list->next;
	struct Army *tmp2;
	int counter = 0;
	while(tmp != satrapies_list && tmp->id != sid) {
		tmp= tmp->next;
	}
	if(tmp == satrapies_list) return 0;
	
	// Now *tmp is the node in satrapies_list with the id = sid
	struct Army *tmpSoldier = (struct Army *)malloc(sizeof(struct Army));
	tmpSoldier->id = aid;
	tmpSoldier->type = type;
	tmpSoldier->general = general;


	if(tmp->s_army == tmp->sentinel){
		tmpSoldier->next = tmp->sentinel;
		tmp->s_army = tmpSoldier;
	}else {	
		tmp2 = tmp->s_army;
		while(tmp2->next != tmp->sentinel) { // O(n)
			tmp2 = tmp2->next;
		}
		tmpSoldier->next = tmp->sentinel;
		tmp2->next = tmpSoldier;
	}


	//print army list for each satrapy
	tmp = satrapies_list->next;
	while(tmp != satrapies_list) {
		counter++;
		printf("\tSatrapy%d = ",counter);
		tmp2 = tmp->s_army;
		while(tmp2!=tmp->sentinel) {
			if(tmp2 == tmp->s_army) printf("<%d,%d,%d>",tmp2->id,tmp2->type,tmp2->general);
			else printf(", <%d,%d,%d>",tmp2->id,tmp2->type,tmp2->general);
			tmp2 = tmp2->next;
		}
		printf("\n");
		tmp= tmp->next;
	}
	printf("DONE\n");
	return 1;
}



/**
 * @brief Prepare Alexander's soldiers for battle
 * 
 * @return 1 on success
 *         0 on failure
 */
int prepare_for_battle_GA (void)
{

	GA_battle_list = NULL;
	int k;
	struct Army *soldiers = GA_army[0];
	struct Army *horses = GA_army[1];
	struct GA_Battle *GA_battle_end = NULL;
	struct GA_Battle *GA_type_one[2];
	GA_type_one[0] = NULL;
	GA_type_one[1] = NULL;
	struct GA_Battle *GA_type_two[2];
	GA_type_two[0] = NULL;
	GA_type_two[1] = NULL;
	struct GA_Battle *GA_type_three[2];
	GA_type_three[0] = NULL;
	GA_type_three[1] = NULL;
	struct GA_Battle *GA_type_four[2];
	GA_type_four[0] = NULL;
	GA_type_four[1] = NULL;

	while(soldiers!=NULL) {
		if(soldiers->type == 1 && horses !=NULL){
			k = Add2GA_Battle(&GA_type_one[0],&GA_type_one[1],soldiers->id,soldiers->general,horses->id);
			if(k==0)return 0;
			horses = horses->next;
		}else if(soldiers->type == 1 && horses == NULL) {
			k = Add2GA_Battle(&GA_type_one[0],&GA_type_one[1],soldiers->id,soldiers->general,-2);
			if(k==0)return 0;
		}
		else if(soldiers->type ==2 ) {
			k = Add2GA_Battle(&GA_type_two[0],&GA_type_two[1],soldiers->id,soldiers->general,-1);
			if(k==0)return 0;
		}
		else if(soldiers->type ==3 ) {
			k = Add2GA_Battle(&GA_type_three[0],&GA_type_three[1],soldiers->id,soldiers->general,-1);
			if(k==0)return 0;
		}
		else if(soldiers->type ==4 ) {
			k = Add2GA_Battle(&GA_type_four[0],&GA_type_four[1],soldiers->id,soldiers->general,-1);
			if(k==0)return 0;
		}
		soldiers = soldiers->next;
	}

	
	GA_type_one[1]->next = GA_type_two[0];
	GA_type_two[1]->next = GA_type_three[0];
	GA_type_three[1]->next = GA_type_four[0];

	GA_battle_list = GA_type_one[0];
	
	printGA_Battle(GA_battle_list);
	


	return 1;
}



/**
 * This function adds a new S_Battle node at the end of the list (S_battle_list)
 * @param first   		The last node of the S_battle_list
 * @param last   		  The last node of the S_battle_list
 * @param sat_id   		The satrapy id of the new node
 * @param sol_id   		The soldier id of the new node
 * @param sol_type   	The soldier type of the new node
 */
int Add2BattleS(struct S_Battle **first,struct S_Battle **last,
									int sat_id, int sol_id, int sol_type) {
	struct S_Battle *res = (struct S_Battle *)malloc(sizeof(struct S_Battle));
	if(res == 0) return 0;
	else {
		res->satrapy_id = sat_id;
		res->soldier_id = sol_id;
		res->type = sol_type;
		res->next = NULL;

		if((*first) == NULL) {
			(*first) = res;
			(*last) = res;
		}else {
			(*last)->next= res;
			(*last) =  res;
		}
	}
	return 1;
}


/**
 * This function prints all the nodes of the S_battle_list
 * @param first   		The last node of the S_battle_list
 */
void printS_Battle_List(struct S_Battle *first) {
	struct S_Battle *tmp = first;
	printf("\tSatrapy Battle = ");
	while(tmp!=NULL) {
		if(tmp == first) printf("<%d,%d,%d>",tmp->soldier_id,tmp->satrapy_id,tmp->type);
		else printf(", <%d,%d,%d>",tmp->soldier_id,tmp->satrapy_id,tmp->type);
		tmp=tmp->next;
	}
	printf("\nDONE\n");
}

/**
 * @brief Prepare Satrapies for battle
 *
 * @param sid The satrapy's id
 *
 * @return 1 on success
 *         0 on failure
 */
int prepare_for_battle_S (int sid)
{
	
	struct S_Battle *first = S_battle_list;
	struct S_Battle *last = S_battle_list;
	struct Satrapy *tmp;
	int counter = 0;
	int flag = 0;
	int cnt = 1; // helps to do zig-zag
	int i,k,t;
	struct Satrapy *s = satrapies_list->next;

	//find the number of nodes 
	while(s != satrapies_list) {
    counter++;
    s = s->next;
  }
	
	//find the node with the id = sid
	s = satrapies_list->next;
	while(s != satrapies_list && s->id != sid) {
    s = s->next;
  }
	// s is now out selected node
	struct Army *q = s->s_army;
	while(q!=s->sentinel) {
		//add the selected node to the S_battle_list
		t = Add2BattleS(&first,&last,s->id,q->id,q->type);
		if(t==0) return 0;
		q = q->next;
	}

	
	//this section implements "zig-zag"
	for(i = 0; i<counter; i++) {
		tmp = s;
		if(flag == 0) {
			//go left
			for(k = 0; k<cnt; k++) {
				s = s->prev;
				if(s == satrapies_list || s->s_army == s->sentinel) {
					s = s->prev;
				}
			}
			cnt++;
			flag = 1;
		}
		else if(flag == 1) {
			//go right
			for(k = 0; k<cnt; k++) {
				s = s->next;
				if(s == satrapies_list || s->s_army == s->sentinel) {
					s = s->next;
				}
			}
			cnt++;
			flag = 0;
		}
		q = s->s_army;
		while(q!=s->sentinel) {
			//add the selected node to the S_battle_list
			t = Add2BattleS(&first,&last,s->id,q->id,q->type);
			if(t==0) return 0;
			q = q->next;
		}
	}

	printS_Battle_List(first);
	S_battle_list = first;

	return 1;
}

/**
 * @brief The death of a soldier
 * 
 * @param sid  The soldier's id
 * @param flag If 0, the soldier belongs to a Satrapy
 *             If 1, the soldier belongs to Alexander the Great
 *
 * @return 1 on success
 *         0 on failure
 */
int soldiers_death (int sid, int flag)
{

	struct Army *q = NULL;
	struct Army *qPrev = NULL;
	struct Satrapy *sat_first =satrapies_list->next;

	struct Army *army_first = GA_army[0];
	struct Army *army_prev = GA_army[0];

	
	if(flag == 0) {
		struct S_Battle *tmp = S_battle_list;
		struct S_Battle *tmpPrev = NULL;
		while(tmp!=NULL && tmp->soldier_id != sid) {
			tmpPrev = tmp;
			tmp = tmp->next;
		}
		// tmp now is the node we want to delete
		if(tmp != NULL) {
			// delete node from S_battle_list
			if(tmpPrev == NULL) {
				tmp = tmp->next;
			}
			else{
				tmpPrev->next = tmp->next;
			}

			while(sat_first->id != tmp->satrapy_id  && sat_first!=satrapies_list) {
				sat_first = sat_first->next;
			}

			q = sat_first->s_army;
			while(q!=sat_first->sentinel && q->id != sid) {
				qPrev = q;
				q = q->next;
			}

			if(q != sat_first->sentinel){
				// delete node from satrapies List	
				if(qPrev == NULL) {
					q = q->next;
				}
				else{
					qPrev->next = q->next;
				}
			}
		}

		
	}else if(flag == 1) {
		struct GA_Battle *tmp = GA_battle_list;
		struct GA_Battle *tmpPrev = NULL;
		//find the node with the solder_id = sid
		while(tmp!=NULL && tmp->soldier_id != sid) {
			tmpPrev = tmp;
			tmp = tmp->next;
		}
		// tmp now is the node we want to delete
		if(tmp != NULL) {
			// delete node from GA_battle_list
			if(tmpPrev == NULL) {
				tmp = tmp->next;
			}
			else{
				tmpPrev->next = tmp->next;
			}
		}
		while(army_first!=NULL && army_first->id != sid) {
			army_prev = army_first;
			army_first = army_first->next;
		}

		if(army_first != NULL) {
			// delete node from GA_battle_list
			army_prev->next = army_first->next;
		}

	}

	printGA_Battle(GA_battle_list);
	printS_Battle_List(S_battle_list);
	return 1;
}


/**
 * @brief Victory of Alexander the Great's horsemen
 *
 * @return 1 on success
 *         0 on failure
 */
int GA_horsemen_victory (void)
{
	struct GA_Battle *firstGA;
	struct GA_Battle *prevGA;
	struct S_Battle *first;
	struct S_Battle *prevFirst;


	//Delete nodes where soldier's type is 1 in S_battle_satrapies
	first = S_battle_list;
	prevFirst = NULL;
	int currType;
	while(first!=NULL) {
		currType = hasTypeOne(first->soldier_id,first->satrapy_id);
		if(currType == 1 && prevFirst == NULL) {
			S_battle_list = S_battle_list->next;
		}else if(currType == 1) {
			prevFirst->next = first->next;
		}
		prevFirst = first;
		first = first->next;
	}


	//Delete 1/3 of the nodes that have horse_id != -1
	firstGA = GA_battle_list;
	prevGA = NULL;

	int counter = 0;
	while(firstGA!=NULL) {
		if(firstGA->horse_id != -1 && counter == 2) {
			prevGA->next = firstGA->next;
			counter = 0;
		}else if(firstGA->horse_id != -1) {
			counter++;
		}
		prevGA = firstGA;
		firstGA =  firstGA->next;
	}

	printGA_Battle(GA_battle_list);
	printS_Battle_List(S_battle_list);
	

	
	return 1;
}

/**
 * @brief The death of Alexander the Great
 *
 * @return 1 on success
 *         0 on failure
 */
int Alexanders_death (void) {
	struct GA_Battle *tmp;
	struct GA_Battle *tmp2;
	struct GA_Battle *battleList = GA_battle_list;
	struct GA_Battle *GA_Generals_last[5];
	int i;


	while(battleList!=NULL) {		
		switch(battleList->general) {
			case 0:
				add2Generals(&GA_Generals[0],&GA_Generals_last[0],battleList->soldier_id,battleList->horse_id,battleList->general);
				break;
			case 1:
				add2Generals(&GA_Generals[1],&GA_Generals_last[1],battleList->soldier_id,battleList->horse_id,battleList->general);
				break;
			case 2:
				add2Generals(&GA_Generals[2],&GA_Generals_last[2],battleList->soldier_id,battleList->horse_id,battleList->general);
				break;
			case 3:
				add2Generals(&GA_Generals[3],&GA_Generals_last[3],battleList->soldier_id,battleList->horse_id,battleList->general);
				break;
			case 4:
				add2Generals(&GA_Generals[4],&GA_Generals_last[4],battleList->soldier_id,battleList->horse_id,battleList->general);
				break;
		}
		tmp2 = battleList;
		battleList = battleList->next;
		tmp2->next = NULL;
		free(tmp2);
	}

	//print army of each GENERAL
	for(i =0; i<5; i++){
		tmp = GA_Generals[i];
		printf("\tGeneral %d = ",i+1);
		while(tmp!=NULL) {
			if(tmp == GA_Generals[i]) printf("<%d,%d,%d> ",tmp->soldier_id,tmp->horse_id,tmp->general);
			else printf(", <%d,%d,%d> ",tmp->soldier_id,tmp->horse_id,tmp->general);
			tmp = tmp->next;
		}
		printf("\n");
	}
	printf("DONE\n");

	
	return 1;
}

/**
 * @brief Print all soldiers of Alexander the Great
 *
 * @return 1 on success
 *         0 on failure
 */
int print_GA_soldiers (void) {
	printGA_army(0);
	printGA_army(1);
	printf("DONE\n");
	return 1;
}

/**
 * @brief Print all Satrapy soldiers
 *
 * @return 1 on success
 *         0 on failure
 */
int print_S_soldiers (void)
{
	struct Satrapy *tmp = satrapies_list->next;
	struct Army *curr;
	int counter = 1;
	while(tmp!=satrapies_list) {
		printf("\tSatrapy%d = ",counter);
		curr = tmp->s_army;
		while(curr!= tmp->sentinel){
			if(curr == tmp->s_army) printf("<%d,%d,%d>",curr->id,curr->type,curr->general);
			else printf(", <%d,%d,%d>",curr->id,curr->type,curr->general);
			curr = curr->next;
		}
		printf("\n");
		counter++;
		tmp = tmp->next;
	}
	printf("\nDONE\n");

	return 1;
}


/**
 * @brief Free resources
 *
 * @return 1 on success
 *         0 on failure
 */
int free_all(void)
{
	struct Army *GA_a = GA_army[0];
	struct Army *GA_help = NULL;
	while(GA_a!=NULL) {
		GA_help = GA_a;
		GA_a = GA_a->next;
		free(GA_help);
	}
	GA_a = GA_army[1];
	GA_help = NULL;
	while(GA_a!=NULL) {
		GA_help = GA_a;
		GA_a = GA_a->next;
		free(GA_help);
	}
	// printf("%d\n",tmp->id);
	


	struct S_Battle *S_help = NULL;
	while(S_battle_list!=NULL) {
		S_help = S_battle_list;
		S_battle_list = S_battle_list->next;
		free(S_help);
	}

	struct Satrapy *sat_list = satrapies_list->next;
	struct Satrapy *Sat_help = NULL;
	struct Army *Army_help = NULL;
	while(sat_list!=satrapies_list) {
		while(sat_list->s_army!=NULL){
			Army_help = sat_list->s_army;
			sat_list->s_army = sat_list->s_army->next;
			free(Army_help);
		}
		Sat_help = sat_list;
		sat_list = sat_list->next;
		free(Sat_help);
	}
	
	int k = 0;
	struct GA_Battle *tmpGeneral;
	for(k =0 ; k<5; k++){
		struct GA_Battle *tmpGeneral_help = NULL;
		tmpGeneral = GA_Generals[k];
		while(GA_a!=NULL) {
			tmpGeneral_help = tmpGeneral;
			tmpGeneral = tmpGeneral->next;
			free(tmpGeneral_help);
		}
	}
	return 1;
}




/**
 * @brief The main function
 *
 * @param argc Number of arguments
 * @param argv Argument vector
 *
 * @return 0 on success
 *         1 on failure
 */
int main(int argc, char** argv)
{
	FILE *fin = NULL;
	char buff[BUFFER_SIZE], event;

	/* Check command buff arguments */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_file> \n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Open input file */
	if (( fin = fopen(argv[1], "r") ) == NULL ) {
		fprintf(stderr, "\n Could not open file: %s\n", argv[1]);
		perror("Opening test file\n");
		return EXIT_FAILURE;
	}

	/* Initializations */
	initialize();
	

	/* Read input file buff-by-buff and handle the events */
	while (fgets(buff, BUFFER_SIZE, fin)) {

		DPRINT("\n>>> Event: %s", buff);

		switch(buff[0]) {

			/* Comment */
			case '#':
				break;

				/* Register GA soldier
				 * R <sid> <type> <general> */
			case 'R':
				{
					int sid, type, general;

					sscanf(buff, "%c %d %d %d", &event, &sid, &type, &general);
					DPRINT("%c %d %d %d\n", event, sid, type, general);

					if (register_GA_soldier(sid, type, general)) {
						DPRINT("R %d %d %d succeeded\n", sid, type, general);
					} else {
						fprintf(stderr, "R %d %d %d failed\n", sid, type, general);
					}

					break;
				}

				/* Register GA horse
				 * H <hid> <type> <general> */
			case 'H':
				{
					int hid, type, general;

					sscanf(buff, "%c %d %d %d", &event, &hid, &type, &general);
					DPRINT("%c %d %d %d\n", event, hid, type, general);

					if (register_GA_horse(hid, type, general)) {
						DPRINT("H %d %d %d succeeded\n", hid, type, general);
					} else {
						fprintf(stderr, "H %d %d %d failed\n", hid, type, general);
					}

					break;
				}

				/* Register Satrapy
				 * S <sid> */
			case 'S':
				{
					int sid;

					sscanf(buff, "%c %d", &event, &sid);
					DPRINT("%c %d\n", event, sid);

					if (register_satrapy(sid)) {
						DPRINT("S %d succeeded\n", sid);
					} else {
						fprintf(stderr, "S %d failed\n", sid);
					}

					break;
				}

				/* Register Satrapy soldier
				 * A <sid> <aid> <type> <general> */
			case 'A':
				{
					int sid, aid, type, general;

					sscanf(buff, "%c %d %d %d %d", &event, &sid, &aid, &type, &general);
					DPRINT("%c %d %d %d %d\n", event, sid, aid, type, general);

					if (register_S_soldier(sid, aid, type, general)) {
						DPRINT("A %c %d %d %d %d succeeded\n", event, sid, aid, type, general);
					} else {
						fprintf(stderr, "A %c %d %d %d %d failed\n", event, sid, aid, type, general);
					}

					break;
				}

				/* Prepare Alexander's soldiers for battle
				 * P */
			case 'P':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (prepare_for_battle_GA()) {
						DPRINT("P %c succeeded\n", event);
					} else {
						fprintf(stderr, "P %c failed\n", event);
					}

					break;
				}

				/* Prepare Satrapies for battle
				 * B <sid> */
			case 'B':
				{
					int sid;

					sscanf(buff, "%c %d", &event, &sid);
					DPRINT("%c %d\n", event, sid);

					if (prepare_for_battle_S(sid)) {
						DPRINT("B %c %d succeeded\n", event, sid);
					} else {
						fprintf(stderr, "B %c %d failed\n", event, sid);
					}

					break;
				}

				/* The death of a soldier
				 * K <sid> <flag> */
			case 'K':
				{
					int sid, flag;

					sscanf(buff, "%c %d %d", &event, &sid, &flag);
					DPRINT("%c %d %d\n", event, sid, flag);

					if (soldiers_death(sid, flag)) {
						DPRINT("K %c %d %d succeeded\n", event, sid, flag);
					} else {
						fprintf(stderr, "K %c %d %d failed\n", event, sid, flag);
					}

					break;
				}

				/* Victory of Alexander the Great's horsemen
				 * V */
			case 'V':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (GA_horsemen_victory()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* The death of Alexander the Great
				 * D */
			case 'D':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (Alexanders_death()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Print all soldiers of Alexander the Great
				 * X */
			case 'X':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_GA_soldiers()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Print all Satrapy soldiers
				 * Y */
			case 'Y':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_S_soldiers()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Empty line */
			case '\n':
				break;

				/* Ignore everything else */
			default:
				DPRINT("Ignoring buff: %s \n", buff);

				break;
		}
	}

	free_all();
	return (EXIT_SUCCESS);
}
