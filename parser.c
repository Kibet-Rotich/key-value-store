#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include "parser.h"


size_t tokenize(char *input, char **tokens, size_t max_tokens) {
    size_t count = 0;
    char *p = input;

    while (*p != '\0' && count < max_tokens) {
        // 1. Skip leading whitespace for the current token
        while (*p == ' ' || *p == '\t' || *p == '\r' || *p == '\n') {
            p++;
        }
        
        // If we hit the end of the string while skipping spaces, break out
        if (*p == '\0') break;

        // 2. Check if the token starts with a quote
        if (*p == '"') {
            p++; // Move pointer past the opening quote
            tokens[count++] = p; // Record the start of the token
            
            // Walk forward until we hit the closing quote or end of string
            while (*p != '\0' && *p != '"') {
                p++;
            }
            
            // If we found the closing quote, replace it with a null terminator
            if (*p == '"') {
                *p = '\0'; 
                p++; // Move past the null terminator for the next loop
            }
        } 
        // 3. Handle a normal unquoted token
        else {
            tokens[count++] = p; // Record the start of the token
            
            // Walk forward until we hit a space or newline
            while (*p != '\0' && *p != ' ' && *p != '\t' && *p != '\r' && *p != '\n') {
                p++;
            }
            
            // If we hit a space, replace it with a null terminator
            if (*p != '\0') {
                *p = '\0';
                p++; // Move past the null terminator
            }
        }
    }
    
    return count;
}

void process_command(HashTable *table, char *input_buffer, char *output_buffer) {

    char* tokens[3];
    size_t count = tokenize(input_buffer, tokens, 3);

    if (count == 0) {
        strcpy(output_buffer, "No input received\n");
        return;
    } else if (count == 2) {
        if(strcmp(tokens[0], "GET") == 0) {
            char *result = kv_get(table, tokens[1]);
            if (result) {
                sprintf(output_buffer, "%s\n", result);
            } else {
                strcpy(output_buffer, "(nil)\n");
            }
        } else if(strcmp(tokens[0], "DEL") == 0) {
            if(kv_delete(table, tokens[1])) {
                strcpy(output_buffer, "(integer) 1\n");
            } else {
                strcpy(output_buffer, "(integer) 0\n");
            }
        } else {
            strcpy(output_buffer, "-ERR unknown command\n");
        }
        return;
    } else if (count == 3 && strcmp(tokens[0], "SET") == 0) {
        if(kv_set(table, tokens[1], tokens[2])) {
            strcpy(output_buffer, "OK\n");
        } else {
            strcpy(output_buffer, "-ERR server out of memory\n");
        }
        return;
    }

    strcpy(output_buffer, "-ERR invalid syntax\n");
}