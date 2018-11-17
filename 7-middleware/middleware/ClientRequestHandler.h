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

using namespace std;

class ClientRequestHandler {

    private:

        //int socket_descriptor;
        struct sockaddr_in address_descriptor;

        char buffer_in[1024];
        char buffer_out[1024];

    public:

        int socket_descriptor;

        char * address;
        const char * host;
        unsigned int port;

        ClientRequestHandler(const char host[], unsigned int port);
        ~ClientRequestHandler();

        int setup();
        int sendData(char buffer[], unsigned int length);
        int readData(char * buffer, unsigned int length);

};
