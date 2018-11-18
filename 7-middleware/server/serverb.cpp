//
// serverb.cpp
//

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>

#import "Sample.h"
#import "Sleeper.h"
#import "Grayscale.h"

#import "../middleware/Method.h"
#import "../middleware/Invoker.h"
#import "../middleware/ServerRequestHandler.h"
#import "../middleware/Call.h"
#import "../middleware/CallStack.h"
#import "../middleware/Invoker.h"
#import "../middleware/Request.h"

using namespace std;

int main(int const, const char ** argv) {

    char buffer[1024] = {0};
    const char host[] = "127.0.0.1\0";
    unsigned int port = atoi(argv[1]);
    unsigned int max_connections = 8;

    Sample sampleMethod = Sample();
    Sleeper sleeperMethod = Sleeper();
    Grayscale grayscaleMethod = Grayscale();

    Invoker invoker = Invoker();
    invoker.registerMethod("sample", &sampleMethod);
    invoker.registerMethod("sleeper", &sleeperMethod);
    invoker.registerMethod("grayscale", &grayscaleMethod);

    ServerRequestHandler handler = ServerRequestHandler(host, port);

    if (handler.setupSocket(&invoker, max_connections) == 0) {

        //handler.compress();
        handler.secure("secret-password");
        handler.run();

    } else {

        printf("Error: %s.\n", strerror(errno));

    }

    return 0;

}
