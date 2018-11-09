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

    CallStack sampleStack = CallStack();

    char name[] = "Mateus Nunes de Barros";

    sampleStack.addConstChars(name);
    sampleStack.addConstChars("Mateus Nunes de Barros Magalhaes");
    sampleStack.addString(string("Mateus Nunes"));
    sampleStack.addInteger(1024);
    sampleStack.addFloat(3.14);
    sampleStack.addBoolean(false);

    char * param1 = sampleStack.getCharsAtIndex(0);
    printf("Parameter 1: %s\n", param1);
    free(param1);

    char * param2 = sampleStack.getCharsAtIndex(1);
    printf("Parameter 2: %s\n", param2);
    free(param2);

    string param3 = sampleStack.getStringAtIndex(2);
    cout << "Parameter 3: " << param3 << endl;

    int param4 = sampleStack.getIntAtIndex(3);
    printf("Parameter 4: %i\n", param4);

    float param5 = sampleStack.getFloatAtIndex(4);
    printf("Parameter 5: %f\n", param5);

    bool param6 = sampleStack.getBooleanAtIndex(5);
    printf("Parameter 6: %s\n", (param6 == true ? "true" : "false"));

    char * sampleStackSerialization = sampleStack.serialize();
    printf("sample stack: %s\n", sampleStackSerialization);
    free(sampleStackSerialization);

    return 0;

    // Initialize the connection.

    const char host[] = "127.0.0.1\0";
    unsigned int port = 5000;

    ClientProxy proxy1 = ClientProxy(host, port);
    ClientProxy proxy2 = ClientProxy(host, port);
    ClientProxy proxy3 = ClientProxy(host, port);

    proxy1.secure("secret-password");
    proxy2.secure("secret-password");
    proxy3.secure("secret-password");

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
