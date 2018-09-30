//
// server.cpp
//

#define WIN32_LEAN_AND_MEAN  /* required by xmlrpc-c/server_abyss.hpp */

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <string>

#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>

#import "ServerRequestHandler.h"

using namespace std;

#define NETWORKING_TCP 1
#define NETWORKING_UDP 0
#define NETWORKING_RPC 0
#define MESSAGE_LENGTH 512

class sampleLowerMethod : public xmlrpc_c::method {

    public:

        sampleLowerMethod() {

            this->_signature = "s:s";
            this->_help = "This method returns a lowercase string.";

        }

        void execute(xmlrpc_c::paramList const& paramList, xmlrpc_c::value * const  retvalP) {

            string data = paramList.getString(0);
            transform(data.begin(), data.end(), data.begin(), ::tolower);
            *retvalP = xmlrpc_c::value_string(data);

        }

};

class sampleUpperMethod : public xmlrpc_c::method {

    public:

        sampleUpperMethod() {

            this->_signature = "s:s";
            this->_help = "This method returns a uppercase string.";

        }

        void execute(xmlrpc_c::paramList const& paramList, xmlrpc_c::value * const  retvalP) {

            string data = paramList.getString(0);
            transform(data.begin(), data.end(), data.begin(), ::toupper);
            *retvalP = xmlrpc_c::value_string(data);

        }

};

int main(int const, const char ** const) {

    char buffer[1024] = {0};

    if (NETWORKING_RPC) {

        const char host[] = "127.0.0.1\0";
        unsigned int port = 5000;
        kNetworkingProtocol protocol = kNetworkingProtocolXMLRPC;

        ServerRequestHandler handler = ServerRequestHandler(host, port, protocol);

        xmlrpc_c::registry registry;
        registry.addMethod("sample.tolower", (new sampleLowerMethod));
        registry.addMethod("sample.toupper", (new sampleUpperMethod));

        if (handler.setupXMLRPC(&registry) < 0) {

            printf("Error: %s.\n", strerror(errno));

        }

    } else {

        const char host[] = "127.0.0.1\0";
        unsigned int port = 5000;
        kNetworkingProtocol protocol = NETWORKING_TCP ? kNetworkingProtocolTCP : kNetworkingProtocolUDP;

        ServerRequestHandler handler = ServerRequestHandler(host, port, protocol);

        if (handler.setupSocket() < 0) {

            printf("Error: %s.\n", strerror(errno));

        }

        while (1) {

            int cmd_len = handler.readData(buffer, 7);

            if (cmd_len == 0) continue;

            buffer[7] = 0;

            // cout << "length: " << cmd_len << endl;
            // cout << "command: " << buffer << endl;

            if (strcmp(buffer, "tolower") == 0) {

                int msg_len = handler.readData(buffer, MESSAGE_LENGTH);
                // cout << "length: " << msg_len << endl;
                // cout << "message: " << buffer << endl;
                for (int i = 0; i < MESSAGE_LENGTH; i++) { buffer[i] = tolower(buffer[i]); }

            } else if (strcmp(buffer, "toupper") == 0) {

                int msg_len = handler.readData(buffer, MESSAGE_LENGTH);
                // cout << "length: " << msg_len << endl;
                // cout << "message: " << buffer << endl;
                for (int i = 0; i < MESSAGE_LENGTH; i++) { buffer[i] = toupper(buffer[i]); }

            }

            handler.sendData(buffer, MESSAGE_LENGTH);

        }

    }

    return 0;

}
