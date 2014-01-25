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

	metadata_t * cur = freelist;
	while(numbytes > cur->size){
		if(cur==NULL){
			print_freelist();
			return NULL;
		}
		cur = cur->next;
	}

	//Corner case for allocation rather than splitting
	if(numbytes >= cur->size - METADATA_T_ALIGNED){
		//size_t old_freelist_size = freelist->size;
		//MAKE POINTERS NULL
		printf("Freelist: %p ", cur);
		//new size is size of the block! we are not changing it.
		void * returnptr = (void *) cur;
		returnptr = METADATA_T_ALIGNED + returnptr;
		printf("Returnptr: %p ", returnptr);
		if(cur->prev==NULL){ //Case when cur is head of list
			freelist = cur->next;
		}
		cur->prev = NULL;
		cur->next = NULL;
		print_freelist();

		return returnptr;
	}


	printf("Freelist: %p ", cur);

	//change size, make pointers of allocated block NULL
	//MAKE POINTERS NULL
	size_t old_size = cur->size;
	cur->size = ALIGN(numbytes);

	void * returnptr = (void *) cur;
	returnptr = METADATA_T_ALIGNED + returnptr;
	
	void * newfreelist = (void *) cur;
	newfreelist = newfreelist + METADATA_T_ALIGNED + ALIGN(numbytes);
	
	metadata_t * newblock = (metadata_t *) newfreelist;
	newblock->size = old_size - METADATA_T_ALIGNED - ALIGN(numbytes);
	if(cur->prev == NULL){ //case when head of list
		freelist = newblock;
	}
	if(cur->prev != NULL){
		cur->prev->next = newblock;
	}
	if(cur->next != NULL){
		cur->next->prev = newblock;
	}
	cur->next = NULL;
	cur->prev = NULL;


	printf("Returnptr: %p ", returnptr);
	printf("newfreelist: %p ", newfreelist);
	printf("cur->size: %zd ", cur->size);
	printf("New block size: %lu \n", METADATA_T_ALIGNED + ALIGN(numbytes));


	print_freelist();

	//change freelist if first block used
	return returnptr;

	//void * newblock = (metadata_t*)

	//freelist = freelist + ALIGN(numbytes) + ;
}

void dfree(void* ptr) {
	print_freelist();
	/* Your free and coalescing code goes here */

	ptr = ptr - METADATA_T_ALIGNED; // GO TO header
	// YOU ARE ALREADY AT THE HEADER

	//NULL case
	if(freelist == NULL){
		freelist = (metadata_t*) ptr;
		//size should already be there
	}
	else{
		metadata_t * cur = freelist;
		void * curvoid = (void *) cur;

		//Freed block is before freelist
		if(ptr < curvoid){
			metadata_t * newfreeblock = (metadata_t *) ptr;
			newfreeblock->prev = NULL;
			newfreeblock->next = freelist;
			freelist = newfreeblock;
			return;
		}

		//Find the free block closest to the new block
		
		void * curnext = (void *) cur->next;
		while(cur->next!=NULL && curnext<ptr){
			cur = cur->next;
			curnext = (void *) cur->next;
		}
		printf("Block previous to ptr: %p ", cur);
		metadata_t * newfreeblock = (metadata_t *) ptr;
		newfreeblock->next = cur->next;
		cur->next->prev = newfreeblock;
		cur->next = newfreeblock;
		newfreeblock->prev = cur;
		// Coalesce
		void * prev = (void *) cur->prev;
		void * next = (void *) cur->next;
		
		if(prev + METADATA_T_ALIGNED + cur->prev->size == curvoid){
			cur->prev->size = cur->prev->size + cur->size + METADATA_T_ALIGNED;
			cur->prev->next = cur->next;
			cur->next->prev = cur->prev;
			// Do we need to change the cur metadata?
		}
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
