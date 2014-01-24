#include <math.h> /* for sqrt() */
#include <stdlib.h> /* for perror()*/
 
/* Obligatory Hello World example using a format specifier */
void helloWorld() {
       char string[] = "Hello World";
       printf("Using format specifier: %s\n", string);
}

/* Scan n integers and add them; Note the usage of & which denotes the memory
 * address */

int scanAndSumInt(int n) {
    int c = 0, a, sum = 0;
    printf("Enter %d integers to add\n", n);
    for(c = 1; c <= n; c++) {
        scanf("%d", &a);
        printf("Value entered = %d\t", a);
        printf("Address of a = %p\n", &a);
        sum += a;
    }
    return sum;
}

void checkEven_v1(int n) {
    if ( n%2 == 0 )
        printf("Even\n");
    else
        printf("Odd\n");
}

void checkEven_v2(int n) {
    if ( n & 1 == 0 )
        printf("Even\n");
    else
        printf("Odd\n");
}

void checkEven_v3(int n) {
    n%2 == 0 ? printf("Even\n") : printf("Odd\n");
}

/* Factorial iterative version */
long factorial_v1(int n) {
    int c; 
    long fact = 1;
    for (c = 1; c <= n; c++)
        fact = fact * c;
    return fact;
}

/* Factorial using recursion */
long factorial_v2(int n) {
   if (n == 0)
        return 1;
   else
        return(n * factorial_v2(n-1)); 
}

/* Helper function for factorial using tail recursion */
long factorial_tailrecursive(int n, long accumulator) {
    if (n == 0) 
        return accumulator;
    return factorial_tailrecursive(n-1, n*accumulator);
}

/* Factorial using tail recursion */
long factorial_v3(int n) {
    return factorial_tailrecursive(n,1);
}

/* Local swap using call by value*/
void swap_local(int x, int y) {
    int temp;
    printf("Before swap: %d, %d\n", x, y);
    temp = x;
    x = y;
    y = temp;
    printf("After swap: %d, %d\n", x, y);
}

/* Swap using call by reference */
void swap_v1(int *x, int *y) {
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

/* Swap without using an additional variable*/
void swap_v2(int *x, int *y) {
    *x = *x + *y;
    *y = *x - *y;
    *x = *x - *y;
}

/* Swap using bitwise OR */
void swap_v3(int *x, int *y) {
    *x = *x ^ *y;
    *y = *x ^ *y;
    *x = *x ^ *y;
}

typedef int bool;
enum { false, true };

bool palindrome(int n) {
    int n_r = 0;
    while(n != 0) {
        n_r = n_r * 10;
        n_r += n%10;
        n = n/10;
    }
    if(n_r == n)
        return true;
    else
        return false;
}

/* Add linker -lm while compiling; Note the cast to int of sqrt()  */
bool checkPrime(int n) {
    int c;
    for(c = 2; c <= (int)sqrt(n); c++) {
        if(n % c == 0)
            return false;
    }
    return true;
}

void readFile(char * fileName) {
    char ch;
    FILE *fp;
    fp = fopen(fileName, "r"); /* Read mode */
    if(fp == NULL) {
        perror("Error while opening a file\n");
        exit(EXIT_FAILURE);
    }
    printf("The contents of the file are: \n");
    while( (ch = fgetc(fp)) != EOF) {
        putc(ch, stdout); /* equivalent to printf("%c",ch); */
    }
    fclose(fp);
}

void pointer_fun_v1() {
	//Declaring a pointer of type int
	int *p1;
	//Declaring a variable of type int
	int v;

	//Assigning a integer value to the location pointed by p1
	//The statement is legal but you might have runtime error (segfault)
	//since p1 is not yet allocated memory
	//*p1 = 10;

	//printf can fail here since we did not assign a memory for p1 yet
	//printf("%d\n",*p1);
	
	//Assiging some arbitrary address is legal but you might be corrupting
	//the memory or you might not have a access to that particular
	//location
	//p1 = 0xbfa42ca8;

	//*p1 = 10;
	//printf can fail here again due to incorrect alignment or
	//due to not able to access the location
	//printf("%d\n",*p1);
	
	//Here we assign the memory to p1
	p1 = (int *) malloc(sizeof(int)*1);
	*p1 = 10;
	int *p2 = p1;
	printf("Address of v: %p\n",&v);
	printf("Value at p1: %d\n",*p1);
	printf("Address of p1: %p\n",p1);
	printf("Address of p2: %p\n",p2);
	printf("Value at p2: %d\n",*p2);
	printf("Freeing p1\n");
	free(p1);

	//The print can fail since p1 is already freed and we are trying to
	//dereference p2 which points to the same location as p1
	printf("Value at p2: %d\n",*p2);
}

/* Using structures and pointers */
void pointer_fun_v2() {

    typedef struct {
        int id;
        float PI;
        char ch;
        char* name;
    } student_t ;
    
    student_t *alice, *bob, *charlie;
    alice = (student_t *) malloc (sizeof(student_t));
    
    /* Check if memory is available */
    if(alice == NULL) {
        perror("Error in malloc\n");
        exit(EXIT_FAILURE);
    }

    int size_int = sizeof(int);
    int size_float = sizeof(float);
    int size_char = sizeof(char);
    int size_charArr = sizeof(char *);

    printf("Size of int: %d\n", size_int);
    printf("Size of float: %d\n", size_float);
    printf("Size of char: %d\n", size_char);
    printf("Size of char *: %d\n", size_charArr); 
    printf("Estimated size of the student_t structure: %d\n", size_int + size_float + size_char + size_charArr);
    printf("Actual size of student_t structure: %d\n", sizeof(student_t));

    /* C provides some syntatic sugar for convenience */
    printf("Address of member id: %p\n", &alice->id);
    printf("Address of member PI: %p\n", &alice->PI);
    printf("Address of member ch: %p\n", &alice->ch);
    printf("Address of member name: %p\n", &alice->name);
   
    (*alice).id = 10;
    (*alice).PI = 3.14;
    (*alice).ch = 'A';
    (*alice).name = "Alice";
    
    printf("Id: %d\n", (*alice).id);   
    printf("PI: %.2f\n", (*alice).PI);   
    printf("ch: %c\n", (*alice).ch);   
    printf("name: %s\n", (*alice).name);   

    printf("Copying alice to bob and updating name\n");
    bob = alice; /* alice and bob now points to the same location */

    printf("Address of alice: %p\n", alice);
    printf("Address of bob: %p\n", bob);
    (*bob).name = "Bob";

    printf("Id: %d\n", alice->id);   
    printf("PI: %.2f\n", alice->PI);   
    printf("ch: %c\n", alice->ch);   
    printf("name: %s\n", alice->name);   

    printf("Id: %d\n", (*bob).id);   
    printf("PI: %.2f\n", (*bob).PI);   
    printf("ch: %c\n", (*bob).ch);   
    printf("name: %s\n", (*bob).name);   

    /* calloc initializes the values to 0 */
    charlie = (student_t *) calloc (sizeof(student_t), 0);
    
    /* Check if memory is available */
    if(charlie== NULL) {
        perror("Error in malloc\n");
        exit(EXIT_FAILURE);
    }

    charlie = alice;
    printf("Address of alice: %p\n", alice);
    printf("Address of bob: %p\n", bob);
    printf("Address of charlie: %p\n", charlie);
    /* C provides some syntatic sugar for convenience */
    charlie->id = alice->id;
    charlie->PI = alice->PI;
    charlie->ch = alice->ch;
    charlie->name = "Charlie";
    
    printf("Freeing alice now\n");
    free(alice); 
    /* alice and bob are both dangling pointers now. It is a good practice to see the pointer to NULL after free; C does not do this
     * for you, but may or may not throw access violation when to attempt to
     * the access the free(d) location. */
    alice = NULL;
    bob = NULL;

    printf("Id: %d\n", charlie->id);   
    printf("PI: %.2f\n", charlie->PI);   
    printf("ch: %c\n", charlie->ch);   
    printf("name: %s\n", charlie->name);   
}

/* Alternative version for 'free()' */
void safefree(void **pp) {
    if (pp != NULL) {               /* safety check */
        free(*pp); /* deallocate chunk, note that free(NULL) is valid */
       *pp = NULL; /* reset original pointer */
    }
}


/* A singly-linked list node, holding data and pointer to next node */
struct node_t
{
       struct node_t* next;
       int data;
};

typedef struct node_t node;

/* Allocate a new node with the given data and next pointer */
node* new_node(int data, node* next) {
       node* node = malloc(sizeof *node);
       node->data = data;
       node->next = next;
       return node;
}

/* Insert the given data at the front of the list specified by a 
 *  * ** pointer to the head node
 *   * */
void insert_front(node** head, int data) {
       node* node = new_node(data, *head);
       *head = node;
}

