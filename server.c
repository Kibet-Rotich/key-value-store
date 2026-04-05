#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h> 
#include <signal.h>     

#include "kv_store.h"
#include "parser.h"

#define PORT 8080
#define BUFFER_SIZE 1024
#define BANNER_MESSAGE "KV Store Server started on port 8080\n"

// --- NEW CODE: THREAD ARGUMENTS ---
// We need a struct because pthread_create only lets you pass ONE void pointer,
// but our worker thread needs BOTH the client_fd and the HashTable pointer.
typedef struct {
    int client_fd;
    HashTable *table;
} thread_args_t;

// --- NEW CODE: THREAD WORKER FUNCTION ---
// This function acts as the "main" function for each new thread.
void *handle_client(void *arg) {
    // TODO 1: Cast the 'arg' back to a thread_args_t pointer.
    thread_args_t *args = (thread_args_t *)arg;
    
    // TODO 2: Extract client_fd and table into local variables.
    int client_fd = args->client_fd;
    HashTable *table = args->table;
    char buffer[BUFFER_SIZE] = {0};

    
    // TODO 3: FREE the memory of the thread_args_t pointer! 
    // (Since we malloc'd it in the main loop, we must free it here to prevent leaks).
    free(arg);

    // TODO 4: Move your existing connection logic here.
    // Cut and paste the write(BANNER_MESSAGE), the read() loop, 
    // the process_command() call, and the close(client_fd) from your old while loop.
     // Send a greeting so clients waiting for an initial banner can proceed.
    write(client_fd, BANNER_MESSAGE, strlen(BANNER_MESSAGE));

        // Read command from client
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_read;
    while((bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1))>0){
        char response[BUFFER_SIZE] = {0};


        process_command(table, buffer, response);

            // Send the result back to client
        write(client_fd, response, strlen(response));
        memset(buffer,0,BUFFER_SIZE);
        }

    close(client_fd);
    

    // Thread is done with this client.
    return NULL; 
}


int main() {

    signal(SIGPIPE,SIG_IGN);//tell OS, when you get a sigpipe command, dont kill my process, just return -1 and it will be handled in my c program
    int server_fd, client_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    HashTable *table = create_table(100);
    if (table == NULL) {
        fprintf(stderr, "Failed to initialize Hash Table\n");
        exit(EXIT_FAILURE);
    }

      // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Attach socket to port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 100) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }


    printf("KV Store Server started on port %d\n", PORT);

    while (1) {
        printf("Waiting for connection...\n");
        
        if ((client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept");
            continue;
        }

        // --- NEW CODE: SPAWN A THREAD ---

        // TODO 5: Dynamically allocate a new thread_args_t struct using malloc.
        // If malloc fails, perror and close the client_fd.
        thread_args_t *tptr = malloc(sizeof(thread_args_t));
        if(tptr == NULL ){
            printf("Thread pointer memory not allocated");
            perror("Thread mem allocation");
            close(client_fd);
            continue;
        }

        // TODO 6: Populate the struct with the current client_fd and the table pointer.
        tptr->client_fd = client_fd;
        tptr->table = table;

        // TODO 7: Create the thread.
        pthread_t thread_id;
        int returnval = pthread_create(&thread_id, NULL, handle_client, (void *)tptr);
        if (returnval != 0){
            perror("Failed to create thread");
            free(tptr);
            close(client_fd);
            continue;
        }
        pthread_detach(thread_id);
    }

    free_table(table);
    close(server_fd);
    return 0;
}