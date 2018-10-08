//
// ServerRequestHandler.cpp
//

#include "ServerRequestHandler.h"

#define PACKET_LENGTH 4096
#define MESSAGE_LENGTH 1024
#define COMMAND_LENGTH 128
#define MAX_CONNECTIONS 16
#define DEFAULT_PORT 5000

#pragma mark -
#pragma mark - Private

#pragma mark -
#pragma mark - Public

ServerRequestHandler::ServerRequestHandler(const char host[], unsigned int port) {

    char paux[7];

    this->host = host;
    this->port = port;
    this->address = (char *) malloc(sizeof(char) * (strlen(host) + 7));
    this->client_address_description_len = sizeof(this->client_address_description);

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

int ServerRequestHandler::setupSocket() {

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

	aux = listen(this->socket_descriptor, MAX_CONNECTIONS);

    if (aux == 1) { return -3;}

    // A call to accept() puts the server to sleep while no requests
    // are made, when the three way TCP handshake is complete, the
    // function returns the socket descriptor representing the client.

    this->client_socket_descriptor = accept(
        this->socket_descriptor,
        (struct sockaddr *) NULL,
        NULL);

    if (this->client_socket_descriptor < 0) { return -4; }

    return 0;

}

int ServerRequestHandler::sendData(char buffer[], unsigned int length) {

    return write(this->client_socket_descriptor, buffer, length);

}

int ServerRequestHandler::readData(char * buffer, unsigned int length) {

    return read(this->client_socket_descriptor, buffer, length);

}

int ServerRequestHandler::readRequestData(char ** data) {

    int n = 0;
    char b[1024] = {0};

    while (b[0] != '>') {

        this->readData(b, 1);

        if (b[0] == '<') continue;
        if (b[0] == '>') continue;

        n = (n*10)+(b[0]-'0');

    }

    this->readData(b, n);

    string item = string(b);
    unsigned int len = item.length()+1;
    const char * tmp = item.c_str();
    char * buffer = (char *) malloc(len * sizeof(char));
    strcpy(buffer, tmp);
    buffer[len-1] = '\0';

    *data = buffer;

    return 0;

}
