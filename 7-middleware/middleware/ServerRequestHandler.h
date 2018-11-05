//
// ServerRequestHandler.h
//

#import <stdio.h>
#import <stdlib.h>
#import <string>

#import <sys/socket.h>
#import <netinet/in.h>
#import <arpa/inet.h>
#import <unistd.h>
#import <errno.h>
#import <string.h>
#import <sys/types.h>
#import <pthread.h>

#import "Call.h"
#import "CallStack.h"
#import "Request.h"
#import "Invoker.h"

using namespace std;

class ServerRequestHandler {

    private:

    public:

        int socket_descriptor;
        struct sockaddr_in address_descriptor;
        struct sockaddr_in client_address_description;
        socklen_t client_address_description_len;

        Invoker * invoker;

        int sendData(int socket_descriptor, char buffer[], unsigned int length);
        int readData(int socket_descriptor, char * buffer, unsigned int length);
        int readRequestData(int socket_descriptor, char ** data);

        char * address;
        const char * host;
        unsigned int port;

        ServerRequestHandler(const char host[], unsigned int port);
        ~ServerRequestHandler();

        int setupSocket(Invoker * invoker, unsigned int max_connections);
        int run();

};
