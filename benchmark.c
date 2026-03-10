#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "kv_store.h"

#define MAX_KEY 64
#define MAX_VAL 64

void generate_key(char *buffer, int i) {
    sprintf(buffer, "key_%d", i);
}

void benchmark(int n) {

    HashTable *table = create_table(n * 2);

    char key[MAX_KEY];
    char value[MAX_VAL];

    clock_t start, end;

    // ----------------------
    // Measure INSERT time
    // ----------------------
    start = clock();

    for (int i = 0; i < n; i++) {
        generate_key(key, i);
        sprintf(value, "value_%d", i);
        kv_set(table, key, value);
    }

    end = clock();

    double insert_time = (double)(end - start) / CLOCKS_PER_SEC;

    // ----------------------
    // Measure GET time
    // ----------------------
    start = clock();

    for (int i = 0; i < n; i++) {
        generate_key(key, i);
        kv_get(table, key);
    }

    end = clock();

    double get_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("N = %d\n", n);
    printf("Insert Time: %f seconds\n", insert_time);
    printf("Get Time:    %f seconds\n", get_time);
    printf("-------------------------\n");

    free_table(table);
}

int main() {

    int tests[] = {1000, 10000, 50000, 100000, 500000, 1000000};
    int size = sizeof(tests) / sizeof(tests[0]);

    for (int i = 0; i < size; i++) {
        benchmark(tests[i]);
    }

    return 0;
}