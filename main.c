#include <stdio.h>
#include "kv_store.h"

int main() {

    // Create table
    HashTable *table = create_table(20);
    if (table == NULL) {
        printf("Failed to create hash table\n");
        return 1;
    }

    printf("Hash table created\n\n");

    // Insert key-value pairs
    kv_set(table, "name", "Rotich");
    printf("%d\n",hash_function("name",20));
    kv_set(table, "language", "C");
    printf("%d\n",hash_function("language",20));
    kv_set(table, "editor", "vim");
    printf("%d\n",hash_function("editor",20));
    kv_set(table, "os", "linux");
    printf("%d\n",hash_function("os",20));
    kv_set(table, "city", "Nairobi");
    printf("%d\n",hash_function("city",20));
    kv_set(table, "country", "Kenya");
    printf("%d\n",hash_function("country",20));
    kv_set(table, "project", "kv_store");
    printf("%d\n",hash_function("project",20));
    kv_set(table, "database", "hash_table");
    printf("%d\n",hash_function("database",20));

    kv_set(table, "status", "testing");
    printf("%d\n",hash_function("status",20));
    kv_set(table, "version", "1.0");
    printf("%d\n",hash_function("version",20));

    printf("Inserted 10 key-value pairs\n\n");

    // Keys to retrieve
    const char *keys[] = {
        "name", "language", "editor", "os", "city",
        "country", "project", "database", "status", "version"
    };

    printf("Retrieving values:\n");

    for (int i = 0; i < 10; i++) {
        char *value = kv_get(table, keys[i]);

        if (value != NULL) {
            printf("%s -> %s\n", keys[i], value);
        } else {
            printf("%s -> NOT FOUND\n", keys[i]);
        }
    }

    printf("\n");

    // Test missing key
    char *missing = kv_get(table, "unknown_key");
    if (missing == NULL) {
        printf("unknown_key -> NOT FOUND (correct behavior)\n");
    }

    printf("\n");

    // Test delete
    printf("Deleting key: city\n");

    if (kv_delete(table, "city")) {
        printf("city deleted successfully\n");
    } else {
        printf("city deletion failed\n");
    }

    char *check = kv_get(table, "city");
    if (check == NULL) {
        printf("Verified: city no longer exists\n");
    }

    printf("\n");

    // Free memory
    free_table(table);
    printf("Hash table freed\n");

    return 0;
}