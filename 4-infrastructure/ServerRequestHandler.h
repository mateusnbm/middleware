//
// ServerRequestHandler.h
//

#import <stdio.h>
#import <stdlib.h>

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

typedef enum {

    kNetworkingProtocolUDP = 0,
    kNetworkingProtocolTCP,
    kNetworkingProtocolXMLRPC

} kNetworkingProtocol;

class ServerRequestHandler {

    private:

        int socket_descriptor;
        int client_socket_descriptor;
        struct sockaddr_in address_descriptor;
        struct sockaddr_in client_address_description;
        socklen_t client_address_description_len;

        xmlrpc_c::serverAbyss * server;

    public:

        char * address;
        const char * host;
        unsigned int port;
        kNetworkingProtocol protocol;

        ServerRequestHandler(const char host[], unsigned int port, kNetworkingProtocol protocol);
        ~ServerRequestHandler();

        int setupSocket();
        int setupXMLRPC(xmlrpc_c::registry * registry);
        int sendData(char buffer[], unsigned int length);
        int readData(char * buffer, unsigned int length);

};
