//
// ClientRequestHandler.h
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

#include <xmlrpc-c/girerr.hpp>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client_simple.hpp>

using namespace std;

typedef enum {

    kNetworkingProtocolUDP = 0,
    kNetworkingProtocolTCP,
    kNetworkingProtocolXMLRPC

} kNetworkingProtocol;

class ClientRequestHandler {

    private:

        int socket_descriptor;
        struct sockaddr_in address_descriptor;

        xmlrpc_c::clientSimple client;

        char buffer_in[1024];
        char buffer_out[1024];

        int setupSocket();
        int setupXMLRPC();

    public:

        char * address;
        const char * host;
        unsigned int port;
        kNetworkingProtocol protocol;

        ClientRequestHandler(const char host[], unsigned int port, kNetworkingProtocol protocol);
        ~ClientRequestHandler();

        int setup();
        int sendData(char buffer[], unsigned int length);
        int readData(char * buffer, unsigned int length);
        void invoke(const char method[], const char types[], xmlrpc_c::value * result, void * parameters);

};
