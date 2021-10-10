int is_simple_num(int num) {
    if (num < 2)
        return 0;
    if (num == 2)
        return 1;

    for (int i = 2; i < num; i++) {
        if (num % i == 0)
            return 0;
    }

    return 1;
}
