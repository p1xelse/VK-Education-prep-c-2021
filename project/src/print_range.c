#include "print_range.h"

int print_range(int n)
{   
    if (n == 1)
    {
        printf("%d", n);
        return 1;
    }
    else
    {
        if (n  <= 0)
            printf(" %d", print_range(n + 1) - 1);
        else
            printf(" %d", print_range(n - 1) + 1);

        return n;
    }

}