/*************************************************************
 * @file   main.c                                            *
 * @author Nikolaos Batsaras <batsaras@csd.uoc.gr>  	     *
 *                                                    	     *
 * @brief  Source file for the needs of cs-240a project 2018 *
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Persida.h"

#define BUFFER_SIZE 1024  /* Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints 
 * or comment to disable it */
#define DEBUG

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else  /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */

/**
 * My function here:
 */

void freeHorses(struct HorsePQ *arr){
	int i;
	for(i = 0; i<arr->size; i++){
		free( (arr + i));
	}

	
}

void freeList(struct AR_Battle *first){
	if(first == NULL) return;
	freeList(first->next);
	free(first);
}
void freeHash(struct AR_Battle **hash_table){
	int i;
	for(i =0 ;i< hash_size; i++){
		freeList(hash_table[i]);
	}
}
void freeBattle(struct GA_Battle *root){
	if(root == NULL) return;
	freeBattle(root->lc);
	freeBattle(root->rc);
	free(root);
}

void freeArmy(struct Army *root){
	if(root == NULL) return;
	freeArmy(root->lc);
	freeArmy(root->rc);
	free(root);
}

// ALL PRINTS
void printGASoldiers(struct Army *root){
	if(root == NULL) return;
	printGASoldiers(root->lc);
	
	printf("%d, ", root->id);
	printGASoldiers(root->rc);
}
void printHashTable(struct AR_Battle **table){
	struct AR_Battle *tmp = NULL;
	int i;
	for(i =0;i<hash_size;i++){		
		tmp = table[i];
		printf("Index %d: ",i);
		while(tmp!=NULL){
			printf("%d, ",tmp->id);
			tmp= tmp->next;
		}
		printf("\n");
	}
	printf("DONE\n");
}
void printGABattleTree(struct GA_Battle *root){
	if(root == NULL) return;
	printGABattleTree(root->lc);
	printf("<%d,%d>, ", root->soldier_id,root->horse_id);
	printGABattleTree(root->rc);
}
// END OF PRINTS



void inOrderDelete(struct GA_Battle *root,struct GA_Battle *prevRoot,struct GA_Tree **tree, int X){

	if(root == NULL) return;
	static int counter = 0;
	static int twct = 0;
	char *path = NULL;
	struct GA_Battle *prev = NULL;
	struct GA_Battle *tmp = NULL;
	tmp = (*tree)->Ga_tree;
	
	inOrderDelete(root->lc,root,tree,X);
	if(counter == X) {
		path = CalculatePath((*tree)->size);
		int i;
		for(i =0; i < (int) log2((*tree)->size); i++){
			if(path[i] == '0') {
				prev = tmp;
				tmp = tmp->lc;
			}else {
				prev = tmp;
				tmp = tmp->rc;
			}
		}


		root->horse_id = tmp->horse_id;
		root->soldier_id = tmp->soldier_id;
		
		if(prev->rc == tmp) {
			prev->rc = NULL;
		}
		else if(prev->lc == tmp) {
			prev->lc = NULL;
		}
		(*tree)->size = (*tree)->size - 1;
		counter = 0;
	}
	counter++;
	twct++;
	inOrderDelete(root->rc,root,tree,X);
}


int findp(){
	int p =  max_soldiers_id_g;
	if(p % 2 ==0)p++;
	return p;
}
int h(int k){
	return ( (a*k +b) %p ) %hash_size;
}

struct AR_Battle** insertToHashTable(struct AR_Battle **table, int id){

	int pos;
	pos = h(id);
	struct AR_Battle *tmp = NULL;
	printf("\nerr1\n");
	struct AR_Battle *res = (struct AR_Battle *)malloc(sizeof(struct AR_Battle));
	printf("\nerr2\n");
	res->id = id;
	res->next = NULL;
	if(table[pos] == NULL ) {
		table[pos] = res;
	}else{
		tmp = table[pos];
		table[pos] = res;
		res->next = tmp;
	}
	
	return table;

}
void inOrder(struct Army* root){
	if(root == NULL) return;
	inOrder(root->lc);
	printf("%d, ", root->id);
	inOrder(root->rc);
}

void inOrderBT(struct GA_Battle* root){
	if(root == NULL) return;
	inOrderBT(root->lc);
	printf("<%d,%d>, ", root->soldier_id,root->horse_id);
	inOrderBT(root->rc);
}
 
int HeapDeleteMin(struct HorsePQ **heap){
	struct Horse *arr = (*heap)->horsePQ;
	int m,p;
	if((*heap)->size== 0) return -1; //error
	int id = (arr)->id;
	int age = (arr + ((*heap)->size -1))->age;
	(*heap)->size--;

	if((*heap)->size == 1)return id;

	m = 0;
	while( ( ((2*m)+1) < ((*heap)->size) && age > (arr + ((2*m )+1))->age ) || 
			(((2*m)+2) < ((*heap)->size) && age > (arr + ((2*m )+2))->age)
	) {
		if((2*m)+2 < ((*heap)->size)) {
			if((arr + ((2*m )+1))->age < (arr + ((2*m )+2))->age)  {
				p = (2*m)+1;
			}else p = (2*m)+2;
		}else p = (*heap)->size -1;
		(arr + m)->age = (arr +p)->age;
		(arr + m)->id = (arr +p)->id;
		m = p;
	}
	(arr +m)->age = age;
	(arr +m)->id = (arr + ((*heap)->size))->id;

	return id;


}

void inOrderBattleTree(struct Army *root){
	if(root == NULL) return;
	inOrderBattleTree(root->lc);
	struct GA_Battle *tmp = GA_Battle_Tree->Ga_tree;
	struct GA_Battle *prevTmp= NULL;
	int sid = root->id;
	int hid = HeapDeleteMin(&GA_Horses);

	struct GA_Battle *res = (struct GA_Battle *)malloc(sizeof(struct GA_Battle));
	res->horse_id = hid;
	res->soldier_id = sid;
	res->lc = NULL;
	res->rc = NULL;
	char *path = CalculatePath(GA_Battle_Tree->size + 1);
	int i;


	for( i = 0; i < (int) log2(GA_Battle_Tree->size)+1; i++){
		if(path[i] == '0'){
			prevTmp = tmp;
			tmp = tmp->lc;
		}else if (path[i] == '1' ){
			prevTmp = tmp;
			tmp = tmp->rc;
		}
	}

	if(tmp == GA_Battle_Tree->Ga_tree) {
		GA_Battle_Tree->Ga_tree = res;
	}
	else{
		if(prevTmp->lc == tmp) {
			tmp = res;
			prevTmp->lc = tmp;
		}
		else if(prevTmp->rc == tmp) {
			tmp = res;
			prevTmp->rc = tmp;
		}
	}

	
	
	GA_Battle_Tree->size++;

	inOrderBattleTree(root->rc);
}



struct Army* BinarySearchTreeInsert(struct Army *root, int sid){

	struct Army *tmp,*res,*prevTmp;
	tmp = NULL;
	res = NULL;
	prevTmp = NULL;
	tmp = root;
	while(tmp != NULL){
		if(tmp->id == sid){
			return root;
		}
		prevTmp = tmp;
		if(sid < tmp->id) tmp = tmp->lc;
		else tmp = tmp->rc;
	}

	res = (struct Army*)malloc(sizeof(struct Army));
	if(res == 0){
		printf("ERROR");
		return root;
	}
	res->id = sid;
	res->lc = NULL;
	res->rc = NULL;
	if(prevTmp == NULL)return res;
	else if(sid <prevTmp->id) prevTmp->lc = res;
	else prevTmp->rc = res;
	return root;
}

struct HorsePQ *HeapInsert(struct HorsePQ *heap, int hid, int hAge){

	if(heap->size >= max_horses){
		printf("ERROR\n");
		return heap;
	}

	int m = heap->size;
	struct Horse *arr = heap->horsePQ;

	while(m > 0 && hAge < (arr + (int)floor( (m-1)/2))->age) {
		(arr + m)->age = (arr + (int)floor( (m-1)/2))->age;
		(arr + m)->id = (arr + (int)floor( (m-1)/2))->id;
		m = (int)floor( (m-1)/2);
	}

	(arr + m)->age = hAge;
	(arr + m)->id = hid;
	 
	heap->size +=1;

	return heap;
}

struct Army **split(struct Army *root,struct Army **arr,int limit){

	// I've tried to write an algorith that implements the splitting in O(h)
	// but i didn't manage to write something that it worked always write.
	// The following code implements split but it's higher than O(h).
	if(root == NULL) return NULL;
	split(root->lc,arr,limit);
	if(root->id < 500) {
		arr[0] = BinarySearchTreeInsert(arr[0],root->id);
	}else if(root->id < 1000) {
		arr[1] = BinarySearchTreeInsert(arr[1],root->id);
	}else if(root->id < 1500) {
		arr[2] = BinarySearchTreeInsert(arr[2],root->id);
	}else if(root->id < 2000) {
		arr[3] = BinarySearchTreeInsert(arr[3],root->id);
	}
	else if(root->id < 2500) {
		arr[4] = BinarySearchTreeInsert(arr[4],root->id);
	}
	split(root->rc,arr,limit);
	return arr;
}


void hash_delete(struct AR_Battle **table, struct GA_Battle *root){
	if(root == NULL)return;
	int c = 0;
	int pos;
	hash_delete(table,root->lc);
	pos = h(root->soldier_id);
	while(table[pos] !=NULL && c < 3){
		table[pos] = table[pos]-> next;
		c++;
	}
	hash_delete(table,root->rc);
}



/**
 * @brief Optional function to initialize data structures that 
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int initialize (void)
{
	srand((unsigned) time(NULL));
	hash_size = (int)rand() % max_soldiers_g;
	p = findp();
	a = rand() % (p +1);
	b = rand() % (p);
	hash_table = (struct AR_Battle **)malloc(sizeof(struct AR_Battle) * hash_size);
	int i;
	for(i =0 ;i< hash_size; i++){
		hash_table[i] = NULL;
	}



	GA_Army = NULL;
	GA_Horses = (struct HorsePQ *)malloc(sizeof(struct HorsePQ *));
	GA_Horses->horsePQ = (struct Horse *)malloc(max_horses * sizeof(struct Horse));
	GA_Horses->size = 0;

	GA_Battle_Tree = (struct GA_Tree *)malloc(sizeof(struct GA_Tree));
	GA_Battle_Tree->Ga_tree = (struct GA_Battle *)malloc(sizeof(struct GA_Battle));
	GA_Battle_Tree->size = 0;
	GA_Battle_Tree->Ga_tree = NULL;

	Generals = (struct Army **)malloc(sizeof(struct Army) * 5);

	for(i = 0 ;i< 5; i++){
		Generals[i] = NULL;
	}
	return 1;
}

/**
 * @brief Register Alexander the Great soldier 
 *
 * @param sid     The soldier's id
 *
 * @return 1 on success
 *         0 on failure
 */
int register_GA_soldier (int sid)
{
	GA_Army = BinarySearchTreeInsert(GA_Army,sid);
	printf("GA soldiers = ");
	printGASoldiers(GA_Army);
	printf("\nDONE\n");
	return 1;
}

/**
 * @brief Register Alexander the Great horse 
 *
 * @param sid     The horse's id
 * @param age     The horse's age
 *
 * @return 1 on success
 *         0 on failure
 */
int register_GA_horse (int hid, int age)
{
	int level = 0;
	int cnt = 1;
	GA_Horses = HeapInsert(GA_Horses,hid,age);
	int i,k,j;

	//Prints all horses in the current level
	for(i = level; i <(int)(log2(GA_Horses->size + 1));i++){
		printf("LEVEL %d : " ,i);
		if(GA_Horses->size == 1){
			printf("<%d, %d>, ",(GA_Horses->horsePQ)->id,(GA_Horses->horsePQ)->age);
		}else if(pow(2,i+1) - 1 == GA_Horses->size){
			for(j = cnt; j < GA_Horses->size;j++){
				printf("<%d, %d>, ",(GA_Horses->horsePQ + j)->id,(GA_Horses->horsePQ + j)->age);
				cnt++;
			}
		}
		printf("\n");
		level++;
	}
	if(GA_Horses->size == max_horses){
		printf("LEVEL %d : ", level);
		for(k = cnt;k<GA_Horses->size;k++){
			printf("<%d, %d>, ",(GA_Horses->horsePQ + k)->id,(GA_Horses->horsePQ + k)->age);
		}
		printf("\n");
	}


	return 1;
}

/**
 * @brief Register enemy soldier
 *
 * @param aid     The soldier's id
 *
 * @return 1 on success
 *         0 on failure
 */
int register_AR_soldier (int aid)
{
	printf("\nerr3\n");
	hash_table = insertToHashTable(hash_table,aid);
	printf("\nerr4\n");
	printHashTable(hash_table);
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
	inOrderBattleTree(GA_Army);
	printf("\nGA battle = ");
	inOrderBT(GA_Battle_Tree->Ga_tree);
	printf("\nDONE\n");
	return 1;
}

/**
 * @brief The ambush against Alexander the Great
 * @param X  Kill 1-every-X soldiers
 * @return 1 on success
 *         0 on failure
 */
int ambush_GA (int X)
{
	inOrderDelete(GA_Battle_Tree->Ga_tree,NULL,&GA_Battle_Tree,X);
	printf("\nGA battle = ");
	inOrderBT(GA_Battle_Tree->Ga_tree);
	printf("\nDONE\n");
	return 1;
}

/**
 * @brief Victory of Alexander the Great's army
 *
 * @return 1 on success
 *         0 on failure
 */
int GA_victory (void)
{
	hash_delete(hash_table,GA_Battle_Tree->Ga_tree);
	printHashTable(hash_table);
	return 1;
}

/**
 * @brief The death of Alexander the Great
 *
 * @return 1 on success
 *         0 on failure
 */
int Alexanders_death (void)
{
	Generals = split(GA_Army,Generals,400);
	int i;
	for(i = 0; i<5; i++){
		printf("General %d = ",i);
		inOrder(Generals[i]);
		printf("\n");
	}
	return 1;
}

/**
 * @brief Print all soldiers of Alexander the Great
 *
 * @return 1 on success
 *         0 on failure
 */
int print_GA_soldiers (void)
{
	printf("GA soldiers = ");
	printGASoldiers(GA_Army);
	printf("\nDONE\n");
	return 1;
}

/**
 * @brief Print all of Alexander the Great horses
 *
 * @return 1 on success
 *         0 on failure
 */
int print_GA_horses (void)
{
	int level = 0;
	int cnt = 1;
	int i,j,k;	
	for(i = level; i <(int)(log2(GA_Horses->size + 1));i++){
		printf("LEVEL %d : " ,i);
		if(GA_Horses->size == 1){
			printf("<%d, %d>, ",(GA_Horses->horsePQ)->id,(GA_Horses->horsePQ)->age);
		}else if(pow(2,i+1) - 1 == GA_Horses->size){
			for(j = cnt; j < GA_Horses->size;j++){
				printf("<%d, %d>, ",(GA_Horses->horsePQ + j)->id,(GA_Horses->horsePQ + j)->age);
				cnt++;
			}
		}
		printf("\n");
		level++;
	}
	if(GA_Horses->size == max_horses){
		printf("LEVEL %d : ", level);
		for(k = cnt;k<GA_Horses->size;k++){
			printf("<%d, %d>, ",(GA_Horses->horsePQ + k)->id,(GA_Horses->horsePQ + k)->age);
		}
		printf("\n");
	}
	return 1;
}

/**
 * @brief Print Alexander the Great army
 *
 * @return 1 on success
 *         0 on failure
 */
int print_GA_army (void)
{
	printf("\nGA Battle:\n");
	printGABattleTree(GA_Battle_Tree->Ga_tree);
	printf("\nDONE\n");
	return 1;
}

/**
 * @brief Print enemy's army
 *
 * @return 1 on success
 *         0 on failure
 */
int print_AR_army (void)
{
	printHashTable(hash_table);
	return 1;
}


/** Useful for insertions and deletions in the GA's battle complete tree 
* complete tree 
*  
*  Function: CalculatePath
*  ------------------------
*  treeSize: The size of a complete binary tree
*  returns: The path to the rightmost leaf of the last level of the tree
*
*  The path is returned as a char array, where '0' corresponds to a left turn
*  and '1' to a right turn, starting from the root node.
*/
/****************************************************************************************************************/
/* Kudos to Elias Papavasileiou for the implementation!                                                         */
/* https://gitlab.com/snippets/1775340?fbclid=IwAR3_Sz8r-4qkNHSmOIxJ0CwLz1SCKYHZi7m5XTDpuZxdRIlBPM57XFyaBcY     */
/****************************************************************************************************************/
char* CalculatePath(int treeSize) {
	if (treeSize <= 1) return '\0';
	const int BITVECTOR_LENGTH = (int) log2(treeSize) + 1;
	char *bitvector = calloc(BITVECTOR_LENGTH, sizeof(char));
	bitvector[BITVECTOR_LENGTH - 1] = '\0';
	int i;
	for (i = BITVECTOR_LENGTH - 2; i >= 0; i--) {
		bitvector[i] = '0' + treeSize % 2;
		treeSize /= 2;
	}
	return bitvector;
}



/**
 * @brief Free resources
 *
 * @return 1 on success
 *         0 on failure
 */
int free_all(void)
{
	int i;
	freeArmy(GA_Army);
	freeBattle(GA_Battle_Tree->Ga_tree);
	freeHash(hash_table);
	freeHorses(GA_Horses);
	for(i = 0; i<5;i++){
		free(Generals[i]);
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


	// Read max horses
	fgets(buff, BUFFER_SIZE, fin);
	sscanf(buff, "%d", &max_horses);
	DPRINT("Max horses: %d\n", max_horses)

	// Read max enemy soldiers' number
	fgets(buff, BUFFER_SIZE, fin);
	sscanf(buff, "%d", &max_soldiers_g);
	DPRINT("Max enemy soldiers: %d\n", max_soldiers_g)

	// Read max enemy soldiers' ID
	fgets(buff, BUFFER_SIZE, fin);
	sscanf(buff, "%d", &max_soldiers_id_g);
	DPRINT("Max enemy soldiers' ID: %d\n", max_soldiers_id_g)

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
				 * R <sid> */
			case 'R':
				{
					int sid;

					sscanf(buff, "%c %d", &event, &sid);
					DPRINT("%c %d\n", event, sid);

					if (register_GA_soldier(sid)) {
						DPRINT("R %d succeeded\n", sid);
					} else {
						fprintf(stderr, "R %d failed\n", sid);
					}

					break;
				}

				/* Register GA horse
				 * H <hid> <age> */
			case 'H':
				{
					int hid, age;

					sscanf(buff, "%c %d %d", &event, &hid, &age);
					DPRINT("%c %d %d\n", event, hid, age);

					if (register_GA_horse(hid, age)) {
						DPRINT("H %d %d succeeded\n", hid, age);
					} else {
						fprintf(stderr, "H %d %d failed\n", hid, age);
					}

					break;
				}
				/* Register enemy soldier
				 * A <aid> */
			case 'A':
				{
					int aid;

					sscanf(buff, "%c %d", &event, &aid);
					DPRINT("%c %d\n", event, aid);

					if (register_AR_soldier(aid)) {
						DPRINT("A %d succeeded\n", aid);
					} else {
						fprintf(stderr, "A %d failed\n", aid);
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
				/* Ambush against Alexander the Great
				 * T <X> */
			case 'T':
				{
					int X;

					sscanf(buff, "%c %d", &event, &X);
					DPRINT("%c %d\n", event, X);

					if (ambush_GA(X)) {
						DPRINT("T %d succeeded\n", X);
					} else {
						fprintf(stderr, "T %d failed\n", X);
					}

					break;
				}

				/* Victory of Alexander the Great's army
				 * K */
			case 'K':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (GA_victory()) {
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
				/* Print all Alexander the Great horses
				 * Y */
			case 'Y':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_GA_horses()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}
			/* Print of Alexander the Great army
				 * X */
			case 'Z':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_GA_army()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}
				/* Print all enemy soldiers
				 * Y */
			case 'W':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_AR_army()) {
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
