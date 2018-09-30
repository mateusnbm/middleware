//
// udp_server.cpp
//
// 128 bytes = 0,125 KB
// 256 bytes = 0,25 KB
// 1024 bytes = 1 KB
// 51200 bytes = 50 KB
// 102400 bytes = 100 KB
//

#import <stdio.h>
#import <stdlib.h>
#import <pthread.h>

#import <sys/socket.h>
#import <netinet/in.h>
#import <arpa/inet.h>
#import <unistd.h>
#import <errno.h>
#import <string.h>
#import <sys/types.h>
#import <time.h>

#define PACKET_LENGTH 4096
//#define MESSAGE_LENGTH 49152
#define MESSAGE_LENGTH 98304

//#define PACKET_LENGTH 10240
//#define MESSAGE_LENGTH 10240 // 10 KB
//#define MESSAGE_LENGTH 102400 // 100 KB
//#define MESSAGE_LENGTH 512000 // 500 KB

#define COMMAND_LENGTH 128
#define MAX_CONNECTIONS 16
#define DEFAULT_PORT 5000

int main (int argc, const char * argv[]) {

    int raux;
    int port_number;

    int socket_descriptor;
    int client_socket_descriptor;

    struct sockaddr_in server_addr_descriptor;
    struct sockaddr_in client_addr_description;
    socklen_t length = sizeof(client_addr_description);

    // Get the port number as a command line argument.

    port_number = argc < 2 ? DEFAULT_PORT : atoi(argv[1]);

    // Creates an unnamed socket inside the kernel and returns an integer
    // known as the socket descriptor. The function socket() takes a
    // domain/family as its first argument. For intenet family of IPv4 we
    // user AF_INET. The type parameter (2nd) specified that we are using UDP.

    socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);

    if (socket_descriptor == -1) {

        printf("[Server] Failed to create endpoint for communication.\n");
        printf("[Server] Error: %s.", strerror(errno));

        return 0;

    }

	memset(&server_addr_descriptor, '0', sizeof(server_addr_descriptor));

	server_addr_descriptor.sin_family = AF_INET;
	server_addr_descriptor.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr_descriptor.sin_port = htons(port_number);

    // The call to the function bind() assigns the details specified in
    // the structure 'server_addr_descriptor' to the socket created above.

	raux = bind(socket_descriptor, (struct sockaddr *) &server_addr_descriptor, sizeof(server_addr_descriptor));

    if (raux == -1) {

        printf("[Server] Failed to assign name to the socket.\n");
        printf("[Server] Error: %s.", strerror(errno));

        close(socket_descriptor);

        return 0;

    }

     // The call to the function listen() specifies the maximum number of
     // client connections that the server should queue.

	raux = listen(socket_descriptor, MAX_CONNECTIONS);

    if (raux == 1) {

        printf("[Server] Failed to mark socket as accepting connections.\n");
        printf("[Server] Error: %s.", strerror(errno));

        close(socket_descriptor);

        return 0;

    }

    // Handle received commands.

    int i, j, count, total_count;
    char message_buffer[MESSAGE_LENGTH] = {0};
    char command_buffer[COMMAND_LENGTH] = {0};

    while (1) {

        count = recvfrom(
            socket_descriptor,
            command_buffer,
            COMMAND_LENGTH,
            0,
            (struct sockaddr *) &client_addr_description,
            &length
        );

        if (count != COMMAND_LENGTH) {

            printf("[Server] Invalid command message length.\n");
            printf("[Server] Error: %s.\n", strerror(errno));
            printf("[Server] Skipping iteration.\n");

            continue;

        }

        if (strcmp(command_buffer, "terminate") == 0) {

            printf("[Server] Terminating.\n");

            break;

        } else if (strcmp(command_buffer, "hello") == 0) {

            //printf("[Server] Generating random message with %i bytes.\n", MESSAGE_LENGTH);

            total_count = 0;

            for (j = 0; j < MESSAGE_LENGTH; j++) {

                message_buffer[j] = ((i + j) % 256) + (rand() % 256);

            }

            for (j = 0; j < MESSAGE_LENGTH; j += PACKET_LENGTH) {

                count = sendto(
                    socket_descriptor,
                    message_buffer + (j * sizeof(char)),
                    PACKET_LENGTH,
                    0,
                    (struct sockaddr *) &client_addr_description,
                    sizeof(client_addr_description)
                );

                total_count += count;

                if (count == -1) {

                    //printf("[Server] Fuck: %s.\n", strerror(errno));

                }

                //printf("[Server] Sent %i bytes.\n", count);

            }

            //printf("[Server] Sent the whole message, totalizing %i bytes.\n", total_count);

        } else {

            printf("[Server] Unknown command.\n");
            printf("[Server] Skipping iteration.\n");

        }

    }

    // Terminate connections.

    close(socket_descriptor);

    return 0;

}
