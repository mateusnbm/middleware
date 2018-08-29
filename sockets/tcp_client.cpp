//
// tcp_client.cpp
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

int main (int argc, const char * argv[]) {

    FILE *image_file_descriptor;

    int raux;
    int caux;

    char buffer[1024] = {0};
    char filename[256] = {0};
    char ip_address[] = "127.0.0.1";
    char filepath[256] = "downloads/tcp_";

    int socket_descriptor;
    int client_socket_descriptor;

    struct sockaddr_in server_addr_descriptor;

    // Create the socket.

    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_descriptor == -1) {

        printf("[Client] Failed to create endpoint for communication.\n");
        printf("[Client] Error: %s.\n", strerror(errno));

        return 0;

	}

	memset(&server_addr_descriptor, '0', sizeof(server_addr_descriptor));

	server_addr_descriptor.sin_family = AF_INET;
	server_addr_descriptor.sin_port = htons(5003);

    // Convert the url address to its binary representation.

    raux = inet_pton(AF_INET, ip_address, &server_addr_descriptor.sin_addr);

    if (raux == 0) {

        printf("[Client] Buffer is not a valid string.\n");

        return 0;

    } else if (raux < 0) {

        printf("[Client] Failed to convert internet address to binary format.\n");
        printf("[Client] Error: %s.\n", strerror(errno));

        return 0;

    }

    // Information like IP address of the remote host and its port is bundled
    // up in a structure and a call to function connect() is made which tries
    // to connect this socket with the socket (IP address and port) of the
    // remote host.

    raux = connect(socket_descriptor, (struct sockaddr *)&server_addr_descriptor, sizeof(server_addr_descriptor));

    if (raux == -1) {

        printf("[Client] Failed to initiate connection.\n");
        printf("[Client] Error: %s.\n", strerror(errno));

        return 0;

    }

    // ...

    printf("time %ld\n", time(NULL));

    for (int k = 0; k < 10000; k++) {

        char filepath[256] = "downloads/tcp_";
        char filename_buffer[256];
        sprintf(filename_buffer, "downloads/tcp_%i_%s", k, filename);

        // We're expecting the filename that shall be sent, let's read it.
        // The call to read() waits for a message to arrive if none is available.

        read(socket_descriptor, filename, sizeof(filename)-1);

        // Let's create the file and write the received bytes to it.

        image_file_descriptor = fopen(filename_buffer, "wb+");

        while ((caux = read(socket_descriptor, buffer, sizeof(buffer))) > 0) {

            fwrite(buffer, sizeof(char), caux, image_file_descriptor);

        }

        fclose(image_file_descriptor);

	}

    printf("time %ld\n", time(NULL));

    // Terminate connections.

    close(socket_descriptor);

    return 0;

}
