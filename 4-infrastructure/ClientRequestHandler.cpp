//
// ClientRequestHandler.cpp
//

#include "ClientRequestHandler.h"

#pragma mark -
#pragma mark - Private

int ClientRequestHandler::setupSocket() {

    int aux;
    int type = (this->protocol == kNetworkingProtocolTCP ? SOCK_STREAM : SOCK_DGRAM);

    memset(&this->address_descriptor, '0', sizeof(this->address_descriptor));

    // Create the socket (SOCK_DGRAM specifies UDP).

    this->socket_descriptor = socket(AF_INET, type, 0);
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

#pragma mark -
#pragma mark - Public

ClientRequestHandler::ClientRequestHandler(const char host[], unsigned int port, kNetworkingProtocol protocol) {

    char paux[7];

    this->host = host;
    this->port = port;
    this->address = (char *) malloc(sizeof(char) * (strlen(host) + 7));
    this->protocol = protocol;

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

    switch (protocol) {

        case kNetworkingProtocolTCP: return this->setupSocket(); break;
        case kNetworkingProtocolUDP: return this->setupSocket(); break;
        case kNetworkingProtocolXMLRPC: return 0; break;

    }

}

int ClientRequestHandler::sendData(char buffer[], unsigned int length) {

    if (this->protocol == kNetworkingProtocolTCP) {

        return write(this->socket_descriptor, buffer, length);

    } else {

        return sendto(this->socket_descriptor, buffer, length, 0, (struct sockaddr *) NULL, sizeof(this->address_descriptor));

    }

}

int ClientRequestHandler::readData(char * buffer, unsigned int length) {

    if (this->protocol == kNetworkingProtocolTCP) {

        return read(this->socket_descriptor, buffer, length);

    } else {

        return recvfrom(this->socket_descriptor, buffer, length, 0, (struct sockaddr *) NULL, NULL);

    }

}

void ClientRequestHandler::invoke(const char method[], const char types[], xmlrpc_c::value * result, void * parameters) {

    this->client.call(this->address, method, types, result, parameters);

}
