//
// udp_server.cpp
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
    int read_aux;
    int wrote_aux;

    char buffer[1024] = {0};
    char filename[] = "lena.bmp\0";

    int socket_descriptor;
    int client_socket_descriptor;

    struct sockaddr_in server_addr_descriptor;
    struct sockaddr_in client_addr_description;
    socklen_t length = sizeof(client_addr_description);

    // Creates an unnamed socket inside the kernel and returns an integer
    // known as the socket descriptor. The function socket() takes a
    // domain/family as its first argument. For intenet family of IPv4 we
    // user AF_INET. The type parameter (2nd) specified that we are using UDP.

    socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);

    if (socket_descriptor == -1) {

        printf("[Server] Failed to create endpoint for communication.\n");
        printf("[Server] Error: %s.", strerror(errno));

        return NULL;

    }

	memset(&server_addr_descriptor, '0', sizeof(server_addr_descriptor));

	server_addr_descriptor.sin_family = AF_INET;
	server_addr_descriptor.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr_descriptor.sin_port = htons(5001);

    // The call to the function bind() assigns the details specified in
    // the structure 'server_addr_descriptor' to the socket created above.

	raux = bind(socket_descriptor, (struct sockaddr *) &server_addr_descriptor, sizeof(server_addr_descriptor));

    if (raux == -1) {

        printf("[Server] Failed to assign name to the socket.\n");
        printf("[Server] Error: %s.", strerror(errno));

        close(socket_descriptor);

        return NULL;

    }

     // The call to the function listen() specifies the maximum number of
     // client connections that the server should queue.

	raux = listen(socket_descriptor, 10);

    if (raux == 1) {

        printf("[Server] Failed to mark socket as accepting connections.\n");
        printf("[Server] Error: %s.", strerror(errno));

        close(socket_descriptor);

        return NULL;

    }

    for (int k = 0; k < 10000; k++) {

        // Wait for a message in order to obtain the client's datagram.

        recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, (struct sockaddr *) &client_addr_description, &length);

        // Send the filename.

        sendto(socket_descriptor, filename, strlen(filename), 0, (struct sockaddr *) &client_addr_description, sizeof(client_addr_description));

        // Send the image data.

        image_file_descriptor = fopen(filename, "rb");

        while ((read_aux = fread(buffer, sizeof(char), sizeof(buffer), image_file_descriptor)) > 0) {

            sendto(socket_descriptor, buffer, read_aux, 0, (struct sockaddr *) &client_addr_description, sizeof(client_addr_description));

        }

        fclose(image_file_descriptor);

    }

    // Terminate connections.

    close(socket_descriptor);

    return NULL;

}
