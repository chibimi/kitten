#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>

#define PORT 8888
#define BACKLOG 10
#define BUFFER_SIZE 1024

void *connection_handler(void *);

const int responses_size = 7; //TODO : Find a way to calculate it
const char * const responses[] = {  "Miiiaaaaooouu",
                                    "Purr Purr <3",
                                    "ZZzzz ZzzzZZzz zZZzzz Zzzz",
                                    "Pet me, human",
                                    "I love you <3",
                                    "You don't see that I am really busy doing nothing right now ?",
                                    "I can haz cheeseburger ?"};


int main(int argc, char *argv[]) {

    int server_socket, client_socket;
    struct sockaddr_in server_addr;

    //initilize seed for random responses
    srand(time(NULL));

    //socket IP_V4, TCP
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1){
        perror("bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, BACKLOG) == -1){
        perror("listen");
        close(server_socket);
        exit(EXIT_FAILURE);
    };
    printf("listening to port %d...\n", PORT);

    while(1){
        client_socket = accept(server_socket, NULL, NULL);
        if (client_socket == -1) {
            perror("accept");
            continue;
        }

        //Create a thread to handle the client
        pthread_t thread;
        int *socket_desc = &client_socket;
        if( pthread_create(&thread, NULL , connection_handler , (void*) socket_desc) < 0)
        {
            perror("pthread_create");
            close(client_socket);
            free(socket_desc);
            continue;
        }
    }
}

void *connection_handler(void *socket_desc) {
    int client_socket = *(int*)socket_desc;
    int read_size;
    char buffer[BUFFER_SIZE];
    printf("Hey ! someone is trying to communicate, let's disappoint her (assuming it's a crazy cat lady).\n");

    while(1){

        memset(buffer, 0, BUFFER_SIZE);
        int read_size = recv(client_socket , buffer , sizeof buffer , 0);

        if(read_size > 0){
            printf("received : %s", buffer);
            const char *miaou = responses[rand() % responses_size];
            write(client_socket , miaou, strlen(miaou));
        }
        else if(read_size == 0){
            printf("Client disconnected");
            fflush(stdout);
            free(socket_desc);
            return 0;
        }
        else if(read_size == -1){
            perror("recv failed");
            free(socket_desc);
            return 0;
        }
    }
}
