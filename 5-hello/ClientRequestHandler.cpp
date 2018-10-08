//
// ClientRequestHandler.cpp
//

#include "ClientRequestHandler.h"

#pragma mark -
#pragma mark - Private

#pragma mark -
#pragma mark - Public

ClientRequestHandler::ClientRequestHandler(const char host[], unsigned int port) {
    
    char paux[7];

    this->host = host;
    this->port = port;
    this->address = (char *) malloc(sizeof(char) * (strlen(host) + 7));

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

ClientRequestHandler::~ClientRequestHandler() {

    free(this->address);

}

int ClientRequestHandler::setup() {

    int aux;

    memset(&this->address_descriptor, '0', sizeof(this->address_descriptor));

    // Create the socket (SOCK_STREAM specifies TCP).

    this->socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    this->address_descriptor.sin_family = AF_INET;
	this->address_descriptor.sin_port = htons(this->port);

	if (this->socket_descriptor == -1) { return -1; }

    // Convert url address to its binary representation.

    aux = inet_pton(AF_INET, this->host, &this->address_descriptor.sin_addr);

    if (aux <= 0) { return -2; }

    // Information like IP address of the remote host and its port is bundled
    // up in a structure and a call to function connect() is made which tries
    // to connect this socket with the socket (IP address and port) of the
    // remote host.

    aux = connect(
        this->socket_descriptor,
        (struct sockaddr *) &this->address_descriptor,
        sizeof(this->address_descriptor));

	if (aux == -1) { return -3; }

    return 0;

}

int ClientRequestHandler::sendData(char buffer[], unsigned int length) {

    return write(this->socket_descriptor, buffer, length);

}

int ClientRequestHandler::readData(char * buffer, unsigned int length) {

    return read(this->socket_descriptor, buffer, length);

}
