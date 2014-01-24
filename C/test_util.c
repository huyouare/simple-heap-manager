#include <stdio.h>
#include "util.h"

int main() {
    printf("Hello two-tenners!\n");

    printf("Some C syntax to remember: \n"
            " *\t-is a pointer to\n"
            " []\t-is an array of\n"
            " ()\t-is a function returning\n"
            " &\t-is an address of\n");
    
    printf("Invoking helloWorld()\n");
    helloWorld();

    printf("Invoking scanAndSumInt(2): %d\n",scanAndSumInt(2));

    int x = 3, y = 4;
    printf("Invoking swap_local(3,4)\n");
    swap_local(x,y);
    printf("After swap at global level: %d, %d\n", x, y);
    printf("Invoking swap_v1(3,4)\n");
    swap_v1(&x,&y);
    printf("After swap at global level: %d, %d\n", x, y);


    printf("Invoking pointer_fun_v2()\n");
    pointer_fun_v2();
    return 0;
}
