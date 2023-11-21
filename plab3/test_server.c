/**
 * \author {AUTHOR}
 */

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include "config.h"
#include "lib/tcpsock.h"

typedef struct {
    pthread_t id;
    tcpsock_t *client;
} connectionHandlerData;

/**
 * Implements a sequential test server (only one connection at the same time)
 */

void *connectionHandler(void *vargp){
    int bytes, result;
    sensor_data_t data;

    connectionHandlerData *connData = (connectionHandlerData*)vargp;
    pthread_t id = connData->id;
    tcpsock_t *client = connData->client;
    printf("%lu\n", id);

        printf("Incoming client connection\n");
        do {
            // read sensor ID
            bytes = sizeof(data.id);
            result = tcp_receive(client, (void *) &data.id, &bytes);
            // read temperature
            bytes = sizeof(data.value);
            result = tcp_receive(client, (void *) &data.value, &bytes);
            // read timestamp
            bytes = sizeof(data.ts);
            result = tcp_receive(client, (void *) &data.ts, &bytes);
            if ((result == TCP_NO_ERROR) && bytes) {
                printf("thread id = %lu - sensor id = %" PRIu16 " - temperature = %g - timestamp = %ld\n", id, data.id, data.value,
                       (long int) data.ts);
            }
        } while (result == TCP_NO_ERROR);
        if (result == TCP_CONNECTION_CLOSED)
            printf("Peer has closed connection\n");
        else
            printf("Error occured on connection to peer\n");
        tcp_close(&client);
        return 0;
}

int main(int argc, char *argv[]) {
    
    if(argc < 3) {
    	printf("Please provide the right arguments: first the port, then the max nb of clients");
    	return -1;
    }
    
    int MAX_CONN = atoi(argv[2]);
    int PORT = atoi(argv[1]);

    pthread_t tid[MAX_CONN];
    tcpsock_t *server, *client[MAX_CONN];
    int conn_counter;
    connectionHandlerData *data = malloc(sizeof(connectionHandlerData));

    printf("Test server is started\n");
    if (tcp_passive_open(&server, PORT) != TCP_NO_ERROR) exit(EXIT_FAILURE);

    for (conn_counter = 0; conn_counter < MAX_CONN; conn_counter++) {
        if (tcp_wait_for_connection(server, &client[conn_counter]) != TCP_NO_ERROR) exit(EXIT_FAILURE);

        data->id = tid[conn_counter];
        data->client = client[conn_counter];

        pthread_create(&tid[conn_counter], NULL, connectionHandler, data);
    }

    for (conn_counter = 0; conn_counter < MAX_CONN; conn_counter++) {
        pthread_join(tid[conn_counter], NULL);
    }

    if (tcp_close(&server) != TCP_NO_ERROR) exit(EXIT_FAILURE);
    printf("Test server is shutting down\n");
    free(data);
    return 0;
}




