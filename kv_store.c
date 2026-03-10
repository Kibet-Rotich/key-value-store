#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kv_store.h"

// The Hash Function

unsigned int hash_function(const char *key, int capacity) {
    // TODO: Iterate through the characters of 'key' to calculate a large integer hash.
    // TODO: Return the hash modulo (%) the capacity to ensure it fits in the array bounds.
    size_t size = strlen(key);
    int hashval = 5381;
    for(int i = 0; i<size;i++){
        hashval = ((hashval << 5)+hashval)+ key[i];

    }

    return (unsigned int)hashval % capacity; 
}

// 2. Initialization
HashTable* create_table(int capacity) {
    // TODO: Allocate memory for the HashTable struct itself.
    // TODO: Allocate memory for the array of 'capacity' Node pointers (buckets).
    // Hint: Use calloc for the buckets so they are automatically initialized to NULL.
    // TODO: Set capacity and size, then return the table pointer.
    HashTable *hashtable = malloc(sizeof(HashTable));
    
    hashtable->buckets = calloc(capacity,sizeof(Node*));//is this sizeof node pointer or node?
    hashtable->size = 0;
    hashtable->capacity = capacity;
    return hashtable; 
}

// 3. Insert or Update
bool kv_set(HashTable *table, const char *key, const char *value) {
    // TODO: Get the bucket index using hash_function().
    // TODO: Traverse the linked list at that bucket. 
    //       If the key already exists, free() the old value and strdup() the new value. Return true.
    // TODO: If the key doesn't exist, allocate a new Node.
    //       Use strdup() to copy the key and value into the new Node.
    // TODO: Insert the new Node at the head of the linked list for that bucket.
    // TODO: Increment table->size and return true.
    int index = hash_function(key,table->capacity);
    Node *current = table->buckets[index];

    while(current){
        if(strcmp(current->key,key) == 0){
            free(current->value);
            current->value = malloc(strlen(value)+1);
            strcpy(current->value, value);
            return true;
        }
        current = current->next;
    }

    Node *new_node = malloc(sizeof(Node));
    
    new_node->key = malloc(strlen(key)+1);
    strcpy(new_node->key, key);
    
    new_node->value = malloc(strlen(value)+1);
    strcpy(new_node->value, value);
    
    new_node->next = table->buckets[index];
    table->buckets[index] = new_node;
    table->size++;
    return true;
}

// 4. Retrieve
char* kv_get(HashTable *table, const char *key) {
    // TODO: Get the bucket index using hash_function().
    // TODO: Traverse the linked list at that bucket looking for a matching key (use strcmp).
    // TODO: If found, return the node->value.
    // TODO: If the end of the list is reached, return NULL.
    int index = hash_function(key,table->capacity);
    Node *head = table->buckets[index];

    while(head){
        if(!(strcmp(head->key,key))){
            return head->value;

        }
        head = head->next;
    }
    
    return NULL;
}

// 5. Delete
bool kv_delete(HashTable *table, const char *key) {
    // TODO: Get the bucket index.
    // TODO: Traverse the list. You will need to keep track of the 'previous' node to bypass the deleted node.
    // TODO: If found, update the pointers: prev->next = current->next. (Handle the edge case where the node to delete is the head of the bucket).
    // TODO: free() the key, free() the value, and free() the Node itself.
    // TODO: Decrement table->size and return true.


    int index = hash_function(key, table->capacity);
    Node *todelete = table->buckets[index];
    Node *prev = table->buckets[index];
    
    while (todelete)
    {
        if(!(strcmp(todelete->key,key))){
            if(prev == todelete){
                table->buckets[index] = todelete->next;
            }
            prev->next = todelete->next;
            free(todelete->key);
            free(todelete->value);
            free(todelete);
            table->size--;
            return true;


        }else{
            prev = todelete;
            todelete = todelete->next;
        }
        
        
        
    }
    

    return false;
}

// 6. Teardown (Crucial for preventing memory leaks)
void free_table(HashTable *table) {
    // TODO: Loop through every bucket from 0 to capacity.
    // TODO: For each bucket, traverse the linked list and free() the key, value, and node.
    // TODO: After all nodes are freed, free() the buckets array.
    // TODO: Finally, free() the HashTable struct itself.

    for(int i = 0; i<table->capacity;i++){
        Node* head = table->buckets[i];
        while(head){
            free(head->key);
            free(head->value);
            Node* next = head->next;
            free(head);
            head = next;
        }
        

    }
    free(table->buckets);
    free(table);
}