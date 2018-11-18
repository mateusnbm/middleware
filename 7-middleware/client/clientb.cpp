//
// clientb.cpp
//

#import <cstdlib>
#import <string>
#import <iostream>
#import <vector>
#import "../middleware/CallStack.h"
#import "../middleware/ClientProxy.h"

using namespace std;

int main(int argc, char ** argv) {

    const char host[] = "127.0.0.1\0";
    unsigned int port = atoi(argv[1]);

    CallStack sampleStack = CallStack();
    ClientProxy sampleProxy = ClientProxy(host, port);

    sampleStack.addFile("client/apple.jpg");

    sampleProxy.compress();
    //sampleProxy.secure("secret-password");

    if (sampleProxy.setup() < 0) {

        printf("[Client] Error:\n\n%s.\n\n", strerror(errno));

        return 0;

    }

    sampleProxy.invoke("sample", sampleStack);

    return 0;

}
