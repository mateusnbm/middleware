//
// server.cpp
//

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>

#import "Method.h"
#import "Invoker.h"
#import "ServerRequestHandler.h"
#import "Call.h"
#import "CallStack.h"
#import "Invoker.h"
#import "Request.h"

using namespace std;

class Lower : public Method {

    public:

        CallStack execute(CallStack inputs) {

            char * v = inputs.getItemAtIndex(0);
            string v_str = string(v);
            transform(v_str.begin(), v_str.end(), v_str.begin(), ::tolower);

            CallStack stack = CallStack();
            stack.addString(v_str);

            free(v);

            return stack;

        }

};

class Upper : public Method {

    public:

        CallStack execute(CallStack inputs) {

            char * v = inputs.getItemAtIndex(0);
            string v_str = string(v);
            transform(v_str.begin(), v_str.end(), v_str.begin(), ::toupper);

            CallStack stack = CallStack();
            stack.addString(v_str);

            free(v);

            return stack;

        }

};

int main(int const, const char ** const) {

    char buffer[1024] = {0};
    const char host[] = "127.0.0.1\0";
    unsigned int port = 5000;

    ServerRequestHandler handler = ServerRequestHandler(host, port);

    if (handler.setupSocket() < 0) {

        printf("Error: %s.\n", strerror(errno));

    }

    Lower lowerMethod = Lower();
    Upper upperMethod = Upper();
    Invoker invoker = Invoker();
    invoker.registerMethod("tolower", &lowerMethod);
    invoker.registerMethod("toupper", &upperMethod);

    while (1) {

        char * data;

        handler.readRequestData(&data);

        Call call = Call(data);
        char * method = call.getMethod();
        CallStack stack = CallStack(call.getStack());
        CallStack result = invoker.invoke(method, stack);

        char * result_str = result.serialize();
        int result_len = string(result_str).length()+1;

        Request response = Request(result_str, result_len);
        int response_len = 0;
        char * response_str = response.serialize(&response_len);

        handler.sendData(response_str, response_len);

        free(data);
        free(method);
        free(result_str);
        free(response_str);

    }

    return 0;

}
