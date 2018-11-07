//
// server.cpp
//

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>

#import "Lower.h"
#import "Upper.h"

#import "../middleware/Method.h"
#import "../middleware/Invoker.h"
#import "../middleware/ServerRequestHandler.h"
#import "../middleware/Call.h"
#import "../middleware/CallStack.h"
#import "../middleware/Invoker.h"
#import "../middleware/Request.h"

using namespace std;

int main(int const, const char ** const) {

    char buffer[1024] = {0};
    const char host[] = "127.0.0.1\0";
    unsigned int port = 5000;
    unsigned int max_connections = 8;

    Lower lowerMethod = Lower();
    Upper upperMethod = Upper();

    Invoker invoker = Invoker();
    invoker.registerMethod("tolower", &lowerMethod);
    invoker.registerMethod("toupper", &upperMethod);

    ServerRequestHandler handler = ServerRequestHandler(host, port);

    if (handler.setupSocket(&invoker, max_connections) == 0) {

        handler.secure("secret-password");
        handler.run();

    } else {

        printf("Error: %s.\n", strerror(errno));

    }

    return 0;

}
