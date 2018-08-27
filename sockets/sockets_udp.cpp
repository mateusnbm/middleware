//
// sockets_udp.cpp
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

void *server(void *) {

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
	server_addr_descriptor.sin_port = htons(5000);

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

    // Terminate connections.

    close(socket_descriptor);

    return NULL;

}

void *client(void *) {

    int n = 0;



    FILE *image_file_descriptor;

    int caux;
    int raux;

    char buffer[1024] = {0};
    char filename[256] = {0};
    char ip_address[] = "127.0.0.1";
    char filepath[] = "downloads/udp_";

    int socket_descriptor;
    int client_socket_descriptor;
    struct sockaddr_in server_addr_descriptor;

    // Create socket (SOCK_DGRAM specifies UDP).

    socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);

	if (socket_descriptor == -1) {

        printf("[Client] Failed to create endpoint for communication.\n");
        printf("[Client] Error: %s.", strerror(errno));

        return NULL;

	}

	memset(&server_addr_descriptor, '0', sizeof(server_addr_descriptor));

	server_addr_descriptor.sin_family = AF_INET;
	server_addr_descriptor.sin_port = htons(5000);

    // Convert url address to its binary representation.

    raux = inet_pton(AF_INET, ip_address, &server_addr_descriptor.sin_addr);

    if (raux == 0) {

        printf("[Client] Buffer is not a valid string.\n");

        return NULL;

    } else if (raux < 0) {

        printf("[Client] Failed to convert internet address to binary format.\n");
        printf("[Client] Error: %s.", strerror(errno));

        return NULL;

    }

    // Information like IP address of the remote host and its port is bundled
    // up in a structure and a call to function connect() is made which tries
    // to connect this socket with the socket (IP address and port) of the
    // remote host.

    raux = connect(socket_descriptor, (struct sockaddr *)&server_addr_descriptor, sizeof(server_addr_descriptor));

	if (raux == -1) {

        printf("[Client] Failed to initiate connection.\n");
        printf("[Client] Error: %s.", strerror(errno));

        return NULL;

	}

    // Send an empty message to the server, gives the connection datagram.

    sendto(socket_descriptor, NULL, 0, 0, (struct sockaddr*) NULL, sizeof(server_addr_descriptor));

    // Receive the image's name.

    recvfrom(socket_descriptor, filename, sizeof(filename)-1, 0, (struct sockaddr *) NULL, NULL);

    // Receive the image.

    caux = 0;
    raux = 0;

    image_file_descriptor = fopen(strcat(filepath, filename), "w+");

    while (caux < 263224) {

        raux = recvfrom(socket_descriptor, buffer, sizeof(buffer), 0, (struct sockaddr *) NULL, NULL);

        if (raux > 0) {

            fwrite(buffer, sizeof(char), raux, image_file_descriptor);

        }

        caux += raux;

    }

    fclose(image_file_descriptor);

    // Terminate connections.

    close(socket_descriptor);

    return NULL;

}

int main (int argc, const char * argv[]) {

    printf("Started.\n");

    pthread_t sid, cid;

    pthread_create(&sid, NULL, server, NULL);
    pthread_create(&cid, NULL, client, NULL);

    pthread_join(sid, NULL);
    pthread_join(cid, NULL);

    printf("Finished.\n");

    return 0;

}
