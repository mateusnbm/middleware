//
// client.cpp
//

#import <cstdlib>
#import <string>
#import <iostream>
#import "../middleware/CallStack.h"
#import "../middleware/ClientProxy.h"

using namespace std;

int main(int argc, char **) {

    // Initialize the connection.

    const char host[] = "127.0.0.1\0";
    unsigned int port = 5000;

    ClientProxy proxy1 = ClientProxy(host, port);
    ClientProxy proxy2 = ClientProxy(host, port);
    ClientProxy proxy3 = ClientProxy(host, port);

    if (proxy1.setup() < 0 || proxy2.setup() < 0 || proxy3.setup() < 0) {

        printf("Error: %s.\n", strerror(errno));

    }

    // Simulate simple invocation.

    sleep(5);

    CallStack stack = CallStack();
    stack.addConstChars("Mateus Nunes de Barros");

    CallStack lowerResponse = proxy1.invoke("tolower", stack);
    CallStack upperResponse = proxy1.invoke("toupper", stack);

    printf("lower %s\n", lowerResponse.serialize());
    printf("upper %s\n", upperResponse.serialize());
    printf("\n");

    return 0;

}
