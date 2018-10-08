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

using namespace std;

class ServerRequestHandler {

    private:

        int socket_descriptor;
        int client_socket_descriptor;
        struct sockaddr_in address_descriptor;
        struct sockaddr_in client_address_description;
        socklen_t client_address_description_len;

    public:

        char * address;
        const char * host;
        unsigned int port;

        ServerRequestHandler(const char host[], unsigned int port);
        ~ServerRequestHandler();

        int setupSocket();
        int sendData(char buffer[], unsigned int length);
        int readData(char * buffer, unsigned int length);
        int readRequestData(char ** data);

};
