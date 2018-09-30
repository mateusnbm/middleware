//
// tcp_server.cpp
//

#define WIN32_LEAN_AND_MEAN  /* required by xmlrpc-c/server_abyss.hpp */

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>

#import <sys/socket.h>
#import <netinet/in.h>
#import <arpa/inet.h>
#import <unistd.h>
#import <errno.h>
#import <string.h>
#import <sys/types.h>

#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>

using namespace std;

#define MESSAGE_LENGTH 512
#define MAX_CONNECTIONS 16

int main(int const, const char ** const) {

    int aux;
    char buffer[1024] = {0};

    const char host[] = "127.0.0.1\0";
    unsigned int port = 7100;

    int socket_descriptor;
    int client_socket_descriptor;
    struct sockaddr_in address_descriptor;
    struct sockaddr_in client_address_description;
    socklen_t client_address_description_len;

    // ...

    memset(&address_descriptor, '0', sizeof(address_descriptor));

    // Creates an unnamed socket inside the kernel and returns an integer
    // known as the socket descriptor. The function socket() takes a
    // domain/family as its first argument. For intenet family of IPv4 we
    // user AF_INET. The type parameter (2nd) specified that we are using UDP.

    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

	address_descriptor.sin_family = AF_INET;
	address_descriptor.sin_addr.s_addr = htonl(INADDR_ANY);
	address_descriptor.sin_port = htons(port);

    if (socket_descriptor == -1) { return -1; }

    // The call to the function bind() assigns the details specified in
    // the structure 'server_addr_descriptor' to the socket created above.

	aux = bind(socket_descriptor, (struct sockaddr *) &address_descriptor, sizeof(address_descriptor));

    if (aux == -1) { return -2; }

     // The call to the function listen() specifies the maximum number of
     // client connections that the server should queue.

	aux = listen(socket_descriptor, MAX_CONNECTIONS);

    if (aux == 1) { return -3;}

    // A call to accept() puts the server to sleep while no requests
    // are made, when the three way TCP handshake is complete, the
    // function returns the socket descriptor representing the client.

    client_socket_descriptor = accept(socket_descriptor, (struct sockaddr *) NULL, NULL);

    if (client_socket_descriptor < 0) { return -4; }

    // ...

    while (1) {

        int cmd_len = read(client_socket_descriptor, buffer, 7);

        if (cmd_len == 0) continue;

        buffer[7] = 0;

        // cout << "length: " << cmd_len << endl;
        // cout << "command: " << buffer << endl;

        if (strcmp(buffer, "tolower") == 0) {

            int msg_len = read(client_socket_descriptor, buffer, MESSAGE_LENGTH);
            // cout << "length: " << msg_len << endl;
            // cout << "message: " << buffer << endl;
            for (int i = 0; i < MESSAGE_LENGTH; i++) { buffer[i] = tolower(buffer[i]); }

        } else if (strcmp(buffer, "toupper") == 0) {

            int msg_len = read(client_socket_descriptor, buffer, MESSAGE_LENGTH);
            // cout << "length: " << msg_len << endl;
            // cout << "message: " << buffer << endl;
            for (int i = 0; i < MESSAGE_LENGTH; i++) { buffer[i] = toupper(buffer[i]); }

        }

        write(client_socket_descriptor, buffer, MESSAGE_LENGTH);

    }

    return 0;

}
