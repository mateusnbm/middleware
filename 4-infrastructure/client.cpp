//
// client.cpp
//

#include <cstdlib>
#include <string>
#include <iostream>

#include <xmlrpc-c/girerr.hpp>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client_simple.hpp>

#include "ClientRequestHandler.h"

#define NETWORKING_TCP 1
#define NETWORKING_UDP 0
#define NETWORKING_RPC 0
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
    unsigned int port = 5000;
    kNetworkingProtocol protocol = ( NETWORKING_TCP ? kNetworkingProtocolTCP : (NETWORKING_UDP ? kNetworkingProtocolUDP : kNetworkingProtocolXMLRPC ) );

    ClientRequestHandler handler = ClientRequestHandler(host, port, protocol);

    if (handler.setup() < 0) {

        printf("Error: %s.\n", strerror(errno));

    }

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

        if (NETWORKING_RPC) {

            xmlrpc_c::value result;
            handler.invoke("sample.tolower", "s", &result, &message_buffer);
            string str = xmlrpc_c::value_string(result);
            // cout << "Lower: " << str << endl;

            xmlrpc_c::value result2;
            handler.invoke("sample.toupper", "s", &result2, &message_buffer);
            str = xmlrpc_c::value_string(result2);
            // cout << "Upper: " << str << endl;

        } else {

            char cmd1[] = "tolower";
            handler.sendData(cmd1, 7);
            handler.sendData(message_buffer, MESSAGE_LENGTH);
            handler.readData(message_buffer, MESSAGE_LENGTH);
            // cout << "Lower: " << message_buffer << endl;

            char cmd2[] = "toupper";
            handler.sendData(cmd2, 7);
            handler.sendData(message_buffer, MESSAGE_LENGTH);
            handler.readData(message_buffer, MESSAGE_LENGTH);
            // cout << "Upper: " << message_buffer << endl;

        }

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
