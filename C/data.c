#include <stdio.h>

int g_int;  /* global static integer */
int g_isd_int = 27;  /* initialized static data */

/*
 * Type definition for a "C address" (e.g., a pointer) as an unsigned long.
 * C allows manipulation of addresses as integers: "pointer arithmetic".
 * Always use unsigned arithmetic: addresses may be "negative" numbers (high-order bit set).
 * Always use long, and never int: int is always 32 bits, but addresses may be 64 bits.
 * Sizes of C data types may vary, e.g., between 32-bit and 64-bit machines.
 * Use the "sizeof" operator.   Take care to write portable code!
 */
typedef unsigned long caddr_t;

main()
{
  char d_char;
  int d_int;
  long d_long;
  char *d_string;
  char d_char_array[16];
  caddr_t base = (caddr_t)&base;
  caddr_t itemptr;
  
  /*
   * %lud prints as long unsigned decimal
   */
  printf("sizes for basic C data types:\n");
  printf("sizeof char: %lu byte\n", sizeof(d_char));
  printf("sizeof int: %lu\n", sizeof(d_int));
  printf("sizeof long: %lu\n", sizeof(d_long));
  printf("sizeof pointer/address: %lu %lu %lu\n", sizeof(d_string), sizeof(base), sizeof(itemptr));
  printf("sizeof char[8] array: %lu\n", sizeof(d_char_array));
  printf("\n");

  printf("stack frame layout:\n");
  print_pointer(base, (caddr_t)&d_char, "d_char");
  print_pointer(base, (caddr_t)&d_int, "d_int");
  print_pointer(base, (caddr_t)&d_long, "d_long");
  print_pointer(base, (caddr_t)&d_string, "d_string");
  print_pointer(base, (caddr_t)&d_char_array, "d_char_array");
  printf("\n");

  printf("address of global integer ", "0x%lx g_int \n",  (caddr_t)&g_int);
  printf("address of initialized int", "0x%lx g_isd_int \n",  (caddr_t)&g_isd_int);
  printf("\n");


  d_int = -1;
  printf("The integer value -1 as signed decimal:   %d\n", d_int);
  printf("The integer value -1 as unsigned decimal: %ud\n", d_int);
  printf("The integer value -1 as unsigned hex:     0x%x\n", d_int);

}

int
print_pointer(caddr_t base, caddr_t itemptr, char* name)
{
  unsigned long ul_base = (unsigned long)base;
  unsigned long ul_itemptr = (unsigned long)itemptr;
  unsigned long ul_offset;

  ul_offset = ul_itemptr - ul_base;

  /*
   * %x prints as unsigned hex, %lx prints a long as unsigned hex
   */
  printf("0x%lx offset %lu %s \n", ul_itemptr, ul_offset, name); 
}

void
stringfun()
{
  char *d_string;
  char d_char_array[16];
  unsigned int *d_uint_block;
  unsigned long *d_ulong_block;
  void* d_rawptr;

  d_rawptr = (void*)&d_ulong_block;
  printf("address of d_ulong_block[] on the stack: 0x%lx\n", (caddr_t)d_rawptr);

 
  d_string = "abAB012\n";
  printf("address of a string constant: 0x%lx\n", (caddr_t)d_string);
  d_rawptr = (void*)d_string;
  printf("address of a string constant: 0x%lx\n", (caddr_t)d_rawptr);
  printf("\n");

  d_ulong_block = (unsigned long *) d_string;
  printf("string abAB012 as one ulong in hex: 0x%lx \n", d_ulong_block[0]);  

  d_uint_block = (unsigned int *) d_string;
  printf("string abAB012 as two uints in hex: 0x%x 0x%x \n", d_uint_block[0], d_uint_block[1]);

  printf("string abAB as an unsigned int in hex: 0x%x \n", *d_uint_block);
  d_uint_block++;
  printf("string 012 as an unsigned int in hex: 0x%x \n", *d_uint_block);
  printf("\n");


  
  

  
}
