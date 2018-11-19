//
// ServerRequestHandler.cpp
//

#include "ServerRequestHandler.h"

#define DEBUG 1

#pragma mark -
#pragma mark - Private

typedef struct argument {

    pthread_t pid;
    int client_descriptor;
    ServerRequestHandler * handler;

} Argument;

void * handleConnection(void * arguments) {

    Argument * argument = (Argument *) arguments;

    while (1) {

        char * data;
        int data_len;

        argument->handler->readRequestData(argument->client_descriptor, &data, &data_len);

        if (argument->handler->compress_payloads == true) {

            #if DEBUG
            printf("[ServerRequestHandler] Connection ID %i, Received %i bytes (before decompression).\n\n", argument->client_descriptor, data_len);
            #endif

            int foo_len = 0;
            char * foo = decompress_data(data, data_len, &foo_len);

            free(data);

            data = foo;
            data_len = foo_len;

            #if DEBUG
            printf("[ServerRequestHandler] Connection ID %i, Received %i bytes (after decompression).\n\n", argument->client_descriptor, data_len);
            #endif

        }

        if (argument->handler->password.empty() == false) {

            #if DEBUG
            printf("[ServerRequestHandler] Connection ID %i, Received %i bytes, Encrypted data:\n\n%s\n\n", argument->client_descriptor, data_len, data);
            #endif

            string hash = sha256(argument->handler->password);
            BLOWFISH bf(hash);
            string en_payload = string(data);
            string de_payload = bf.Decrypt_CBC(en_payload);

            free(data);

            data = (char *) malloc(sizeof(char)*(de_payload.size()+1));
            std::copy(de_payload.begin(), de_payload.end(), data);
            data[de_payload.size()] = '\0';
            data_len = de_payload.size();

        }

        #if DEBUG
        printf("[ServerRequestHandler] Connection ID %i, Received %i bytes, Raw data:\n\n%s\n\n", argument->client_descriptor, data_len, data);
        #endif

        if (strcmp(data, "terminate") == 0) { break; }

        Call call = Call(data);
        char * method = call.getMethod();
        CallStack stack = CallStack(call.getStack());
        CallStack result = argument->handler->invoker->invoke(method, stack);

        char * result_str = result.serialize();
        int result_len = string(result_str).length()+1;

        #if DEBUG
        printf("[ServerRequestHandler] Connection ID %i, Sending %i bytes, Raw data:\n\n%s\n\n", argument->client_descriptor, result_len, result_str);
        #endif

        if (argument->handler->password.empty() == false) {

            string hash = sha256(argument->handler->password);
            BLOWFISH bf(hash);
            string de_payload = string(result_str);
            string en_payload = bf.Encrypt_CBC(de_payload);

            free(result_str);

            result_str = (char *) malloc(sizeof(char)*(en_payload.size()+1));
            std::copy(en_payload.begin(), en_payload.end(), result_str);
            result_str[en_payload.size()] = '\0';

            result_len = en_payload.size();// + 1;

            #if DEBUG
            printf("[ServerRequestHandler] Connection ID %i, Sending %i bytes, Encrypted data:\n\n%s\n\n", argument->client_descriptor, result_len, result_str);
            #endif

        }

        if (argument->handler->compress_payloads == true) {

            #if DEBUG
            printf("[ServerRequestHandler] Connection ID %i, Sending %i bytes (before compression).\n\n", argument->client_descriptor, result_len);
            #endif

            int foo_len = 0;
            char * foo = compress_data(result_str, result_len, &foo_len);

            free(result_str);

            result_str = foo;
            result_len = foo_len;

            #if DEBUG
            printf("[ServerRequestHandler] Connection ID %i, Sending %i bytes (after compression).\n\n", argument->client_descriptor, result_len);
            #endif

        }

        Request response = Request(result_str, result_len);
        int response_len = 0;
        char * response_str = response.serialize(&response_len);

        argument->handler->sendData(argument->client_descriptor, response_str, response_len);

        free(data);
        free(method);
        free(result_str);
        free(response_str);

    }

    #if DEBUG
    printf("[Server] Closing connection %i.\n\n", argument->client_descriptor);
    #endif

    free(argument);

    return NULL;

}

#pragma mark -
#pragma mark - Public

ServerRequestHandler::ServerRequestHandler(const char host[], unsigned int port) {

    char paux[7];

    this->host = host;
    this->port = port;
    this->address = (char *) malloc(sizeof(char) * (strlen(host) + 7));
    this->client_address_description_len = sizeof(this->client_address_description);
    this->compress_payloads = false;

    paux[0] = ':';
    paux[1] = ((port / 1000) % 10) + '0';
    paux[2] = ((port / 100) % 10) + '0';
    paux[3] = ((port / 10) % 10) + '0';
    paux[4] = (port % 10) + '0';
    paux[5] = '/';
    paux[6] = 0;

    strcpy(this->address, host);
    strcat(this->address, paux);

}

ServerRequestHandler::~ServerRequestHandler() {

    free(this->address);

}

int ServerRequestHandler::sendData(int socket_descriptor, char buffer[], unsigned int length) {

    return write(socket_descriptor, buffer, length);

}

int ServerRequestHandler::readData(int socket_descriptor, char * buffer, unsigned int length) {

    return read(socket_descriptor, buffer, length);

}

int ServerRequestHandler::readRequestData(int socket_descriptor, char ** data, int * len) {

    int n = 0;
    char b[1024] = {0};

    while (b[0] != '>') {

        this->readData(socket_descriptor, b, 1);

        if (b[0] == '<') continue;
        if (b[0] == '>') continue;

        n = (n*10)+(b[0]-'0');

    }

    char * buf = (char *) malloc(sizeof(char)*(n+1));
    buf[n] = '\0';

    this->readData(socket_descriptor, buf, n);

    *data = buf;
    *len = n;

    return 0;

}

int ServerRequestHandler::setupSocket(Invoker * invoker, unsigned int max_connections) {

    int aux;

    memset(&this->address_descriptor, '0', sizeof(this->address_descriptor));

    // Creates an unnamed socket inside the kernel and returns an integer
    // known as the socket descriptor. The function socket() takes a
    // domain/family as its first argument. For intenet family of IPv4 we
    // user AF_INET. The type parameter (2nd) specified that we are using UDP.

    this->socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

	this->address_descriptor.sin_family = AF_INET;
	this->address_descriptor.sin_addr.s_addr = htonl(INADDR_ANY);
	this->address_descriptor.sin_port = htons(this->port);

    if (this->socket_descriptor == -1) { return -1; }

    // The call to the function bind() assigns the details specified in
    // the structure 'server_addr_descriptor' to the socket created above.

	aux = bind(
        this->socket_descriptor,
        (struct sockaddr *) &this->address_descriptor,
        sizeof(this->address_descriptor));

    if (aux == -1) { return -2; }

     // The call to the function listen() specifies the maximum number of
     // client connections that the server should queue.

	aux = listen(this->socket_descriptor, max_connections);

    if (aux == 1) { return -3;}

    this->invoker = invoker;

    return 0;

}

int ServerRequestHandler::compress() {

    this->compress_payloads = true;

    return 0;

}

int ServerRequestHandler::secure(const char key[]) {

    this->password = string(key);

    return 0;

}

int ServerRequestHandler::run() {

    int client_descriptor;

    while (1) {

        client_descriptor = accept(this->socket_descriptor, (struct sockaddr *) NULL, NULL);

        if (client_descriptor < 0) { return -1; }

        #if DEBUG
        printf("[Server] Accepted new connection (%i).\n\n", client_descriptor);
        #endif

        Argument * argument = (Argument *) malloc(sizeof(Argument));

        argument->handler = this;
        argument->client_descriptor = client_descriptor;

        pthread_create(&argument->pid, NULL, handleConnection, (void *) argument);

    }

    return 0;

}
