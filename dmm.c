#include <stdio.h> //needed for size_t
#include <unistd.h> //needed for sbrk
#include <assert.h> //For asserts
#include "dmm.h"

/* You can improve the below metadata structure using the concepts from Bryant
 * and OHallaron book (chapter 9).
 */

typedef struct metadata {
       /* size_t is the return type of the sizeof operator. Since the size of
 	* an object depends on the architecture and its implementation, size_t 
	* is used to represent the maximum size of any object in the particular
 	* implementation. 
	* size contains the size of the data object or the amount of free
 	* bytes 
	*/
	size_t size;
	struct metadata* next;
	struct metadata* prev; //What's the use of prev pointer?
} metadata_t;

/* freelist maintains all the blocks which are not in use; freelist is kept
 * always sorted to improve the efficiency of coalescing 
 */

static metadata_t* freelist = NULL;

void* dmalloc(size_t numbytes) {
	
	if(freelist == NULL) { 			//Initialize through sbrk call first time
		if(!dmalloc_init())
			return NULL;
	}
	//test_print();

	assert(numbytes > 0);

	/* Your code goes here */

	// case of 0, return null pointer
	if(numbytes == 0){
		return NULL;
	}

	metadata_t * currfree = freelist;
	while(numbytes > currfree->size){
		if(currfree==NULL){
			print_freelist();
			return NULL;
		}
		currfree = currfree->next;
	}
	//Corner case for allocation rather than splitting
	if(numbytes > currfree->size - METADATA_T_ALIGNED){
		//size_t old_freelist_size = freelist->size;
		//MAKE POINTERS NULL
		printf("Freelist: %p", currfree);
		//new size is size of the block! we are not changing it.
		void * returnptr = (void *) currfree;
		returnptr = METADATA_T_ALIGNED + returnptr;
		printf("Returnptr: %p", returnptr);
		currfree = currfree->next;
		printf("New block size: %lu", METADATA_T_ALIGNED + currfree->size);
		print_freelist();
		return returnptr;
	}


	printf("Freelist: %p", currfree);

	//change size, make pointers of allocated block NULL
	//MAKE POINTERS NULL
	size_t old_freelist_size = currfree->size;
	currfree->size = ALIGN(numbytes);
	void * returnptr = (void *) currfree;
	returnptr = METADATA_T_ALIGNED + returnptr;
	printf("Returnptr: %p", returnptr);
	void * newfreelist = (void *) currfree;
	newfreelist = newfreelist + METADATA_T_ALIGNED + ALIGN(numbytes);
	printf("newfreelist: %p", newfreelist);
	currfree = (metadata_t *) newfreelist;
	currfree->size = old_freelist_size - METADATA_T_ALIGNED - ALIGN(numbytes);
	printf("freesize: %zd", currfree->size);
	printf("New block size: %lu", METADATA_T_ALIGNED + ALIGN(numbytes));
	print_freelist();
	return returnptr;

	//void * newblock = (metadata_t*)

	//freelist = freelist + ALIGN(numbytes) + ;
}

void dfree(void* ptr) {
	print_freelist();
	/* Your free and coalescing code goes here */

	ptr = ptr - METADATA_T_ALIGNED;

	//NULL case
	if(freelist == NULL){
		freelist = (metadata_t*) ptr;
		//size should already be there
	}
	else{
	//Find the free block closest to the new block
		metadata_t *currfree = freelist;
		while(currfree->next!=NULL && (void*)currfree->next<ptr){
			currfree = currfree->next;
		}
		metadata_t * newfreeblock = (metadata_t*) ptr;
		newfreeblock->next = currfree->next;
		currfree->next->prev = newfreeblock;
		currfree->next = newfreeblock;
		newfreeblock->prev = currfree;
	}
	print_freelist();

}

bool dmalloc_init() {

	/* Two choices: 
 	* 1. Append prologue and epilogue blocks to the start and the end of the freelist
 	* 2. Initialize freelist pointers to NULL
 	*
 	* Note: We provide the code for 2. Using 1 will help you to tackle the
 	* corner cases succinctly.
 	*/

	size_t max_bytes = ALIGN(MAX_HEAP_SIZE);
	freelist = (metadata_t*) sbrk(max_bytes); // returns heap_region, which is initialized to freelist
	/* Q: Why casting is used? i.e., why (void*)-1? */
	if (freelist == (void *)-1)
		return false;
	freelist->next = NULL;
	freelist->prev = NULL;
	freelist->size = max_bytes-METADATA_T_ALIGNED;
	return true;
}

/*Only for debugging purposes; can be turned off through -NDEBUG flag*/
void print_freelist() {
	metadata_t *freelist_head = freelist;
	while(freelist_head != NULL) {
		DEBUG("\tFreelist Size:%zd, Head:%p, Prev:%p, Next:%p\t",freelist_head->size,freelist_head,freelist_head->prev,freelist_head->next);
		freelist_head = freelist_head->next;
	}
	DEBUG("\n");
}
