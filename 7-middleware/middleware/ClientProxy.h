//
// ClientProxy.h
//

#import <stdio.h>
#import <stdlib.h>
#import "CallStack.h"
#import "ClientRequestHandler.h"

using namespace std;

class ClientProxy {

    private:

        const char * host;
        unsigned int port;

        ClientRequestHandler * handler;

    public:

        ClientProxy(const char host[], unsigned int port);
        ~ClientProxy();

        int setup();
        CallStack invoke(const char method[], CallStack stack);

};
