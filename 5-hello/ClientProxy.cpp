//
// ClientProxy.cpp
//

#import "ClientProxy.h"
#import "Call.h"
#import "Request.h"

#pragma mark -
#pragma mark - Private

#pragma mark -
#pragma mark - Public

ClientProxy::ClientProxy(const char host[], unsigned int port) {

    this->host = host;
    this->port = port;
    this->handler = new ClientRequestHandler(host, port);

}

ClientProxy::~ClientProxy() {

    delete this->handler;

}

int ClientProxy::setup() {

    return this->handler->setup();

}

CallStack ClientProxy::invoke(const char method[], CallStack stack) {

    Call r_call = Call(method, stack);

    int str_call_len = 0;
    char * str_call = r_call.serialize(&str_call_len);

    Request request = Request(str_call, str_call_len);

    int request_len = 0;
    char * request_str = request.serialize(&request_len);

    this->handler->sendData(request_str, request_len);

    free(str_call);
    free(request_str);

    // ...

    int n = 0;
    char b[1024] = {0};

    while (b[0] != '>') {

        this->handler->readData(b, 1);

        if (b[0] == '<') continue;
        if (b[0] == '>') continue;

        n = (n*10)+(b[0]-'0');

    }

    this->handler->readData(b, n);

    string item = string(b);
    unsigned int len = item.length()+1;
    const char * tmp = item.c_str();
    char * buffer = (char *) malloc(len * sizeof(char));
    strcpy(buffer, tmp);
    buffer[len-1] = '\0';

    CallStack response = CallStack(buffer);

    free(buffer);

    return response;

}
