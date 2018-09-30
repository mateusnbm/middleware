//
// tcp_client.cpp
//

#include <cstdlib>
#include <string>
#include <iostream>

#import <sys/socket.h>
#import <netinet/in.h>
#import <arpa/inet.h>
#import <unistd.h>
#import <errno.h>
#import <string.h>
#import <sys/types.h>

#include <xmlrpc-c/girerr.hpp>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client_simple.hpp>

#define MESSAGE_LENGTH 512
#define REPEAT_COUNT 10000
#define PROTOCOL_NAME "tcp"

using namespace std;

int main(int argc, char **) {

    long num = rand();
    srand(time(&num));

    int i, j, count;
    char message_buffer[MESSAGE_LENGTH+1] = {0};
    char filename_buffer[256] = {0};

    const char host[] = "127.0.0.1\0";
    unsigned int port = 7100;

    int aux;
    int socket_descriptor;
    struct sockaddr_in address_descriptor;

    // ...

    memset(&address_descriptor, '0', sizeof(address_descriptor));

    // Create the socket (SOCK_DGRAM specifies UDP).

    socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    address_descriptor.sin_family = AF_INET;
	address_descriptor.sin_port = htons(port);

	if (socket_descriptor == -1) { return -1; }

    // Convert url address to its binary representation.

    aux = inet_pton(AF_INET, host, &address_descriptor.sin_addr);

    if (aux <= 0) { return -2; }

    // Information like IP address of the remote host and its port is bundled
    // up in a structure and a call to function connect() is made which tries
    // to connect this socket with the socket (IP address and port) of the
    // remote host.

    aux = connect(socket_descriptor, (struct sockaddr *) &address_descriptor, sizeof(address_descriptor));

	if (aux == -1) { return -3; }

    // ...

    sprintf(filename_buffer, "times/times_%s_%i.txt", PROTOCOL_NAME, MESSAGE_LENGTH);
    long stime, etime, delta;
    FILE *logs_file = fopen(filename_buffer, "w+");
    clockid_t clk_id = CLOCK_MONOTONIC;
    struct timespec time_spec;

    for (i = 0; i < REPEAT_COUNT; i++) {

        for (j = 0; j < MESSAGE_LENGTH; j++) {

            message_buffer[j] = (rand() % 26) + 'a' - (rand() % 2 == 0 ? 0 : ('a'-'A'));

        }

        memset(&time_spec, '0', sizeof(timespec));
        clock_gettime(clk_id, &time_spec);
        stime = time_spec.tv_nsec;

        // cout << "Message: " << message_buffer << endl;

        char cmd1[] = "tolower";
        write(socket_descriptor, cmd1, 7);
        write(socket_descriptor, message_buffer, MESSAGE_LENGTH);
        read(socket_descriptor, message_buffer, MESSAGE_LENGTH);
        // cout << "Lower: " << message_buffer << endl;

        char cmd2[] = "toupper";
        write(socket_descriptor, cmd2, 7);
        write(socket_descriptor, message_buffer, MESSAGE_LENGTH);
        read(socket_descriptor, message_buffer, MESSAGE_LENGTH);
        // cout << "Upper: " << message_buffer << endl;

        memset(&time_spec, '0', sizeof(timespec));
        clock_gettime(clk_id, &time_spec);
        etime = time_spec.tv_nsec;
        delta = (etime - stime) / 1000;

        count = sprintf(message_buffer, "%ld,", delta);
        count = fwrite(message_buffer, sizeof(char), count, logs_file);

    }

    fclose(logs_file);

    return 0;

}
