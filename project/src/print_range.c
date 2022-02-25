#include <stdio.h>
#include "print_range.h"

#define LEFT_BORDER 1

int print_range(int right_border) {
    if (right_border == LEFT_BORDER) {
        printf("%d", right_border);
        return LEFT_BORDER;
    } else {
        if (right_border  <= 0)
            printf(" %d", print_range(right_border + 1) - 1);
        else
            printf(" %d", print_range(right_border - 1) + 1);

        return right_border;
    }
}
