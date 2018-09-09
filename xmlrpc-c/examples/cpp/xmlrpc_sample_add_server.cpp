//
// xmlrpc_sample_add_server.cpp
//

#define WIN32_LEAN_AND_MEAN  /* required by xmlrpc-c/server_abyss.hpp */

#include <cassert>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#ifdef _WIN32
#  include <windows.h>
#else
#  include <unistd.h>
#endif

using namespace std;

#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>

#ifdef _WIN32
  #define SLEEP(seconds) SleepEx(seconds * 1000);
#else
  #define SLEEP(seconds) sleep(seconds);
#endif

#define MESSAGE_LENGTH 10240 // 10 KB
//#define MESSAGE_LENGTH 102400 // 100 KB
//#define MESSAGE_LENGTH 512000 // 500 KB

class sampleAddMethod : public xmlrpc_c::method {

public:

    sampleAddMethod() {

        // signature and help strings are documentation -- the client
        // can query this information with a system.methodSignature and
        // system.methodHelp RPC.
        this->_signature = "i:ii";

        // method's result and two arguments are integers
        this->_help = "This method adds two integers together";

    }

    void execute(xmlrpc_c::paramList const& paramList, xmlrpc_c::value * const  retvalP) {

        int const addend(paramList.getInt(0));
        int const adder(paramList.getInt(1));

        paramList.verifyEnd(2);

        *retvalP = xmlrpc_c::value_int(addend + adder);

        // Sometimes, make it look hard (so client can see what it's like
        // to do an RPC that takes a while).
        if (adder == 1)
            SLEEP(2);

    }

};

class sampleHelloMethod : public xmlrpc_c::method {

public:

    sampleHelloMethod() {

        this->_signature = "s:";
        this->_help = "This method returns a random string.";

    }

    void execute(xmlrpc_c::paramList const& paramList, xmlrpc_c::value * const  retvalP) {

        int i, j;
        char message_buffer[MESSAGE_LENGTH] = {0};

        for (j = 0; j < (MESSAGE_LENGTH-1); j++) {

            message_buffer[j] = ( (((i + j) % 256) + (rand() % 256)) % 25) + 97;

        }

        *retvalP = xmlrpc_c::value_string(message_buffer);

    }

};

int main(int const, const char ** const) {

    try {

        xmlrpc_c::registry myRegistry;

        xmlrpc_c::methodPtr const sampleAddMethodP(new sampleAddMethod);
        xmlrpc_c::methodPtr const sampleHelloMethodP(new sampleHelloMethod);

        myRegistry.addMethod("sample.add", sampleAddMethodP);
        myRegistry.addMethod("sample.hello", sampleHelloMethodP);

        xmlrpc_c::serverAbyss myAbyssServer(xmlrpc_c::serverAbyss::constrOpt().registryP(&myRegistry).portNumber(8080));

        myAbyssServer.run();
        // xmlrpc_c::serverAbyss.run() never returns
        assert(false);

    } catch (exception const& e) {

        cerr << "Something failed.  " << e.what() << endl;

    }

    return 0;

}
