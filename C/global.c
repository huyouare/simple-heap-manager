#include <stdio.h>

int g_int;  /* global static integer */
int g_isd_int = 27;  /* initialized static data */

main()
{
  int i;

  i = g_isd_int;
  g_int = i;
}
