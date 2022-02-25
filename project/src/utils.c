#include <stdio.h>
#include <stddef.h>

#include "utils.h"

size_t timer_from(int from) {
    size_t counter = 0;

    for (int i = from; i >= 0; --i) {
        ++counter;
        printf("%d%c", i, i == 0 ? '\n' : ' ');
    }

    return counter;
}

int custom_pow(int base, int power) {
    if (power < 0)
        return 0;
    int res = 1;
    for (int i = 0; i < power; ++i) {
        res *= base;
    }

    return res;
}
