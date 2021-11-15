#include <stdio.h>
#include <limits.h>
int     main(void)
{
    int a = INT_MAX - 20000;

    printf("%d, %p",a, &a);
}