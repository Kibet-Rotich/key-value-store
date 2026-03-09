#ifndef KV_STORE_H
#define KV_STORE_H

#include <stdbool.h>

// A single Key-Value pair stored as a node in a linked list
typedef struct Node {
    char *key;
    char *value;
    struct Node *next; // Pointer to the next node (for collisions)
} Node;

// The main Hash Table structure
typedef struct {
    Node **buckets;    // Array of pointers to Nodes
    int capacity;      // Total number of buckets
    int size;          // Total number of key-value pairs currently stored
} HashTable;

// --- Core API ---

// Initialize a new hash table with a given number of buckets
HashTable* create_table(int capacity);

// Insert a key-value pair. If the key exists, update the value.
// Returns true on success, false on failure (e.g., out of memory).
bool kv_set(HashTable *table, const char *key, const char *value);

// Retrieve a value by its key. 
// Returns a pointer to the value string, or NULL if not found.
char* kv_get(HashTable *table, const char *key);

// Delete a key-value pair from the table.
// Returns true if deleted, false if the key wasn't found.
bool kv_delete(HashTable *table, const char *key);

// Free the entire hash table, including all nodes, keys, and values.
void free_table(HashTable *table);

// --- Internal Helper for testing ---

// Compute an array index for a given string key
unsigned int hash_function(const char *key, int capacity);

#endif // KV_STORE_H