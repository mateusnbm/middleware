//
// xmlrpc_sample_add_client.cpp
//

#include <cstdlib>
#include <string>
#include <iostream>

using namespace std;

#include <xmlrpc-c/girerr.hpp>
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/client_simple.hpp>

#define MESSAGE_LENGTH 10240 // 10 KB
//#define MESSAGE_LENGTH 102400 // 100 KB
//#define MESSAGE_LENGTH 512000 // 500 KB
#define REPEAT_COUNT 10000

int main(int argc, char **) {

    if (argc-1 > 0) {

        cerr << "This program has no arguments" << endl;

        exit(1);

    }

    try {

        int i, j, count;
        char message_buffer[MESSAGE_LENGTH] = {0};
        char filename_buffer[256] = {0};

        sprintf(filename_buffer, "times/times_tcp_%i.txt", MESSAGE_LENGTH);
        long stime, etime, delta;
        FILE *logs_file = fopen(filename_buffer, "w+");
        clockid_t clk_id = CLOCK_MONOTONIC;
        struct timespec time_spec;

        string const serverUrl("http://localhost:8080/RPC2");
        string const addMethodName("sample.add");
        string const helloMethodName("sample.hello");

        xmlrpc_c::clientSimple myClient;
        xmlrpc_c::value result;

        //myClient.call(serverUrl, addMethodName, "ii", &result, 5, 7);
        //int const sum = xmlrpc_c::value_int(result);

        for (i = 0; i < REPEAT_COUNT; i++) {

            // Get the current clock time in nanoseconds.

            memset(&time_spec, '0', sizeof(timespec));
            clock_gettime(clk_id, &time_spec);
            stime = time_spec.tv_nsec;

            // Say hello and receive the random message.

            xmlrpc_c::value helloResult;

            myClient.call(serverUrl, helloMethodName, "", &helloResult);

            string str = xmlrpc_c::value_string(helloResult);

            // Compute the time delta in microseconds and write it to a file.

            memset(&time_spec, '0', sizeof(timespec));
            clock_gettime(clk_id, &time_spec);
            etime = time_spec.tv_nsec;
            delta = (etime - stime) / 1000;

            // ...

            count = sprintf(message_buffer, "%ld,", delta);
            count = fwrite(message_buffer, sizeof(char), count, logs_file);

    	}

        fclose(logs_file);

    } catch (exception const& e) {

        cerr << "Client threw error: " << e.what() << endl;

    } catch (...) {

        cerr << "Client threw unexpected error." << endl;

    }

    return 0;

}
