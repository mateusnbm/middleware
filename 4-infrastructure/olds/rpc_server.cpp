//
// rpc_server.cpp
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

using namespace std;

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

    const char host[] = "127.0.0.1\0";
    unsigned int port = 7300;

    xmlrpc_c::registry registry;
    registry.addMethod("sample.tolower", (new sampleLowerMethod));
    registry.addMethod("sample.toupper", (new sampleUpperMethod));

    xmlrpc_c::serverAbyss server = xmlrpc_c::serverAbyss(
        xmlrpc_c::serverAbyss::constrOpt()
        .registryP(&registry)
        .portNumber(port)
        .uriPath("/"));

    server.run();

    return 0;

}
