#include <stdio.h>
#include <string.h>
#include "parser.h"

static void run_case(HashTable *table, const char *command) {
	char input[128];
	char output[256] = {0};

	strncpy(input, command, sizeof(input) - 1);
	input[sizeof(input) - 1] = '\0';

	process_command(table, input, output);
	printf("CMD: %-20s -> RESP: %s\n", command, output);
}

int main(void) {
	HashTable *table = create_table(32);
	if (table == NULL) {
		fprintf(stderr, "Failed to create table\n");
		return 1;
	}

	run_case(table, "SET name Alice");
	run_case(table, "SET city Nairobi");
	run_case(table, "SET lang C");

	run_case(table, "GET name");
	run_case(table, "GET city");
	run_case(table, "GET missing_key");

	run_case(table, "DEL city");
	run_case(table, "DEL city");
	run_case(table, "GET city");

	run_case(table, "PING");

	free_table(table);
	return 0;
}
