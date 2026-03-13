#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kv_store.h"

// The Hash Function
unsigned int hash_function(const char *key, int capacity) {
    size_t size = strlen(key);
    int hashval = 5381;
    for(int i = 0; i<size;i++){
        hashval = ((hashval << 5)+hashval)+ key[i];

    }

    return (unsigned int)hashval % capacity; 
}

// Initialization
HashTable* create_table(int capacity) {
    
    HashTable *hashtable = malloc(sizeof(HashTable));
    
    hashtable->buckets = calloc(capacity,sizeof(Node*));
    hashtable->size = 0;
    hashtable->capacity = capacity;
    return hashtable; 
}

// Insert or Update
bool kv_set(HashTable *table, const char *key, const char *value) {

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

// Retrieve
char* kv_get(HashTable *table, const char *key) {
    
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

// Delete
bool kv_delete(HashTable *table, const char *key) {

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

// free allocated memory
void free_table(HashTable *table) {
    

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