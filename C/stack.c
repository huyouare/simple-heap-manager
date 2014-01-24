#include <stdio.h>

typedef unsigned long caddr_t;

main()
{
  caddr_t base = (caddr_t)&base;

  printf("main stack frame:\n");
  print_pointer(base, (caddr_t)&base, "base");
  printf("\n");

  proc1(base);
}


int proc1(caddr_t base)
{
  int d_int;
  char d_char;
  long d_long;
  char *d_string;
  char d_char_array[16];

  printf("proc1 stack frame:\n");
  print_pointer(base, (caddr_t)&base, "base");
  print_pointer(base, (caddr_t)&d_char, "d_char");
  print_pointer(base, (caddr_t)&d_int, "d_int");
  print_pointer(base, (caddr_t)&d_long, "d_long");
  print_pointer(base, (caddr_t)&d_string, "d_string");
  print_pointer(base, (caddr_t)&d_char_array, "d_char_array");
  printf("\n");

  proc2(base);
}

int proc2(caddr_t base)
{
  char d_char1;
  char d_char2;
  char d_char3;
  long d_long;
  int d_int;

  printf("proc2 stack frame:\n");
  print_pointer(base, (caddr_t)&d_char1, "d_char1");
  print_pointer(base, (caddr_t)&d_char2, "d_char2");
  print_pointer(base, (caddr_t)&d_char3, "d_char3");
  print_pointer(base, (caddr_t)&d_long, "d_long");
  print_pointer(base, (caddr_t)&d_int, "d_int");
  printf("\n");
}

int
print_pointer(caddr_t base, caddr_t itemptr, char* name)
{
  unsigned long ul_base = (unsigned long)base;
  unsigned long ul_itemptr = (unsigned long)itemptr;
  int ul_offset = ul_base - ul_itemptr;

  /*
   * %x prints as unsigned hex, %lx prints a long as unsigned hex
   */
  printf("0x%lx offset %d %s \n", ul_itemptr, ul_offset, name); 
}
