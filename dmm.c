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
bool init = false;

void* dmalloc(size_t numbytes) {
	DEBUG("there\n");
	if(freelist == NULL) { 			//Initialize through sbrk call first time
		if(!init && !dmalloc_init()){
			return NULL;
		}		
	}
	//test_print();

	assert(numbytes > 0);
	DEBUG("assert?\n");
	/* Your code goes here */

	// case of 0, return null pointer
	if(numbytes == 0){
		return NULL;
	}
	DEBUG("here\n"); 
	metadata_t * cur = freelist;
	if(cur==NULL || cur==0x0){
		print_freelist();
		return NULL;
	}
	DEBUG("%p\n", cur);
	while(numbytes > cur->size){

		cur = cur->next;
		if(cur==NULL || cur==0x0){
			print_freelist();
			return NULL;
		}
	}

	//Corner case for allocation rather than splitting
	DEBUG("numbytes: %lu, cur->size - METADATA_T_ALIGNED: %lu", numbytes, cur->size - METADATA_T_ALIGNED);
	// REALLY DUMB BUG:
	// These are UNSIGNED longs, so negative numbers are large!
	if(METADATA_T_ALIGNED >= cur->size || numbytes >= cur->size - METADATA_T_ALIGNED){
		//size_t old_freelist_size = freelist->size;
		//MAKE POINTERS NULL
		DEBUG("Freelist: %p \n", cur);
		//new size is size of the block! we are not changing it.
		void * returnptr = (void *) cur;
		returnptr = METADATA_T_ALIGNED + returnptr;
		DEBUG("Returnptr: %p \n", returnptr);
		if(cur->prev==NULL){ //Case when cur is head of list
			freelist = cur->next;
		}
		cur->prev = NULL;
		cur->next = NULL;
		print_freelist(); 

		return returnptr;
	}


	DEBUG("Freelist (2): %p ", cur);

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
	else{
		cur->prev->next = newblock;
		newblock->prev = cur->prev;
	}
	if(cur->next == NULL){
		newblock->next = NULL;
	}
	else{
		cur->next->prev = newblock;
		newblock->next = cur->next;
	}
	cur->next = NULL;
	cur->prev = NULL;


	DEBUG("Returnptr: %p ", returnptr);
	DEBUG("newfreelist: %p ", newfreelist);
	DEBUG("cur->size: %zd ", cur->size);
	DEBUG("New block size: %lu \n", METADATA_T_ALIGNED + ALIGN(numbytes));


	print_freelist();

	//change freelist if first block used
	return returnptr;

	//void * newblock = (metadata_t*)

	//freelist = freelist + ALIGN(numbytes) + ;
}

void dfree(void* ptr) {
	/* Your free and coalescing code goes here */

	ptr = ptr - METADATA_T_ALIGNED; // GO TO header
	// YOU ARE ALREADY AT THE HEADER

	//NULL case
	if(freelist == NULL){
		freelist = (metadata_t*) ptr;
		//size should already be there
		print_freelist();
		return;
	}
	else{
		metadata_t * cur = freelist;
		void * curvoid = (void *) cur;
		if(curvoid==ptr){
			DEBUG("Block already free!!!");
			return;
		}

		//Freed block is before freelist
		if(ptr < curvoid){
			metadata_t * newfreeblock = (metadata_t *) ptr;
			freelist->prev = newfreeblock;
			newfreeblock->prev = NULL;
			newfreeblock->next = freelist;
			freelist = newfreeblock;

			void * next = (void *) newfreeblock->next;
			void * newfreeblockvoid = (void *) newfreeblock;
			// Coalesce to next block
			DEBUG("Next: %p, %p \n", newfreeblockvoid + METADATA_T_ALIGNED + newfreeblock->size, next);
			if(newfreeblockvoid + METADATA_T_ALIGNED + newfreeblock->size == next){
				newfreeblock->size = newfreeblock->size + newfreeblock->next->size + METADATA_T_ALIGNED;
				metadata_t * newfreenext = newfreeblock->next;
				newfreeblock->next = newfreeblock->next->next;
				if(newfreeblock->next != NULL){
					newfreeblock->next->prev = newfreeblock;
				}
				newfreenext->next = NULL;
				newfreenext->prev = NULL;
				// Do we need to change the cur metadata?
			}
			print_freelist();
			return;
		}

		//Find the free block closest to the new block
		
		void * curnext = (void *) cur->next;

		while(cur->next!=NULL && curnext<=ptr){
			if(curnext==ptr){
				DEBUG("Block already free!!!");
				return;
			}
			cur = cur->next;
			curnext = (void *) cur->next;
		}
		DEBUG("Block previous to ptr: %p \n", cur);
		metadata_t * newfreeblock = (metadata_t *) ptr;
		DEBUG("ptr/newfreeblock: %p\n", newfreeblock);
		newfreeblock->next = cur->next;
		if(cur->next != NULL){
			cur->next->prev = newfreeblock;
		}
		cur->next = newfreeblock;
		newfreeblock->prev = cur; 

		void * prev = (void *) cur;
		void * next = (void *) newfreeblock->next;
		void * newfreeblockvoid = (void *) newfreeblock;
		// Coalesce to next block
		DEBUG("Next: %p, %p \n", newfreeblockvoid + METADATA_T_ALIGNED + newfreeblock->size, next);
		if(newfreeblockvoid + METADATA_T_ALIGNED + newfreeblock->size == next){
			newfreeblock->size = newfreeblock->size + newfreeblock->next->size + METADATA_T_ALIGNED;
			metadata_t * newfreenext = newfreeblock->next;
			newfreeblock->next = newfreeblock->next->next;
			if(newfreeblock->next != NULL){
				newfreeblock->next->prev = newfreeblock;
			}
			newfreenext->next = NULL;
			newfreenext->prev = NULL;
			// Do we need to change the cur metadata?
		}

		// Coalesce, cur is the previous block
		DEBUG("Prev: %p, %p \n", prev + METADATA_T_ALIGNED + cur->size, newfreeblockvoid);
		if(prev + METADATA_T_ALIGNED + cur->size == newfreeblockvoid){
			cur->size = cur->size + newfreeblock->size + METADATA_T_ALIGNED;
			cur->next = newfreeblock->next;
			if(newfreeblock->next != NULL){
				newfreeblock->next->prev = cur;
			}
			newfreeblock->next = NULL;
			newfreeblock->prev = NULL;
			// Do we need to change the cur metadata?
		}
	}
	print_freelist();


}

bool dmalloc_init() {

	init = true;

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
