#include <stdio.h>

int main()  {
    const int a = 12;
    int * p = &a;
    *p = 20;
    printf("a = %d, *p = %d\n", a, *p);
    printf("&a = %p, p = %p\n", &a, p);
    return 0;
}