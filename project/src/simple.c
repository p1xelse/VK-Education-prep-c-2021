int is_simple(int num) {
    if (num < 2)
        return 0;
    if (num == 2)
        return 1;

    for (int delimiter = 2; delimiter * delimiter < num; delimiter++) {
        if (num % delimiter == 0)
            return 0;
    }

    return 1;
}
