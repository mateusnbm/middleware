//
// ClientProxy.h
//

#import <stdio.h>
#import <stdlib.h>

#import "SHA256.h"
#import "Blowfish.h"
#import "CallStack.h"
#import "ClientRequestHandler.h"

using namespace std;

class ClientProxy {

    private:

        bool connected;
        const char * host;
        unsigned int port;
        string password;

        ClientRequestHandler * handler;

    public:

        ClientProxy(const char host[], unsigned int port);
        ~ClientProxy();

        int secure(const char key[]);
        int setup();
        CallStack invoke(const char method[], CallStack stack);

};
