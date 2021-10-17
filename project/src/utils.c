#include "utils.h"
#include <stdio.h>
#include <stddef.h>

size_t timer_from(int from) {
    size_t counter = 0;

    for (int i = from; i >= 0; --i) {
        ++counter;
        printf(i == 0 ? "%d\n" : "%d ", i);
    }

    return counter;
}

int custom_pow(int base, int power) {
    int res = 1;

    for (int i = 0; i < power; i++) {
        res *= base;
    }

    return res;
}
