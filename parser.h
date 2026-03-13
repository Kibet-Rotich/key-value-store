#ifndef PARSER_H
#define PARSER_H

#include "kv_store.h"

// Takes the raw input from the network, executes the command on the table,
// and writes the text response into the output_buffer.
void process_command(HashTable *table, char *input_buffer, char *output_buffer);

#endif