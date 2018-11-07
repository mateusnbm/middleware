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

    if (this->password.empty() == false) {

        string hash = sha256(this->password);
        BLOWFISH bf(hash);
        string de_payload = string("terminate");
        string en_payload = bf.Encrypt_CBC(de_payload);

        string payload = "<" + to_string(en_payload.size()) + ">" + en_payload;
        int request_len = payload.size() + 1;

        char * request = (char *) malloc(sizeof(char)*(payload.size()+1));
        std::copy(payload.begin(), payload.end(), request);
        request[payload.size()] = '\0';

        this->handler->sendData(request, request_len);

        free(request);

    } else {

        char terminate[] = "<9>terminate\0";

        this->handler->sendData(terminate, 12);

    }

    delete this->handler;

}

int ClientProxy::secure(const char key[]) {

    this->password = string(key);

    return 0;

}

int ClientProxy::setup() {

    return this->handler->setup();

}

CallStack ClientProxy::invoke(const char method[], CallStack stack) {

    Call r_call = Call(method, stack);

    int str_call_len = 0;
    char * str_call = r_call.serialize(&str_call_len);

    printf("[ClientProxy] Send data (decrypted): %s.\n", str_call);

    if (this->password.empty() == false) {

        string hash = sha256(this->password);
        BLOWFISH bf(hash);
        string de_payload = string(str_call);
        string en_payload = bf.Encrypt_CBC(de_payload);

        free(str_call);

        str_call = (char *) malloc(sizeof(char)*(en_payload.size()+1));
        std::copy(en_payload.begin(), en_payload.end(), str_call);
        str_call[en_payload.size()] = '\0';

        str_call_len = en_payload.size() + 1;

        printf("[ClientProxy] Send data (encrypted): %s.\n", str_call);

    }

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

    char * data = (char *) malloc(sizeof(char)*(n+1));
    data[n] = '\0';

    this->handler->readData(data, n);

    if (this->password.empty() == false) {

        printf("[ClientProxy] Received data (encrypted): %s.\n", data);

        string hash = sha256(this->password);
        BLOWFISH bf(hash);
        string en_payload = string(data);
        string de_payload = bf.Decrypt_CBC(en_payload);

        free(data);

        data = (char *) malloc(sizeof(char)*(de_payload.size()+1));
        std::copy(de_payload.begin(), de_payload.end(), data);
        data[de_payload.size()] = '\0';

    }

    printf("[ClientProxy] Received data (decrypted): %s.\n", data);

    string item = string(data);
    unsigned int len = item.length()+1;
    const char * tmp = item.c_str();
    char * buffer = (char *) malloc(len * sizeof(char));
    strcpy(buffer, tmp);
    buffer[len-1] = '\0';

    CallStack response = CallStack(buffer);

    free(data);
    free(buffer);

    return response;

}
