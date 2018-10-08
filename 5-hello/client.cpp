//
// client.cpp
//

#import <cstdlib>
#import <string>
#import <iostream>
#import "CallStack.h"
#import "ClientProxy.h"

#define MESSAGE_LENGTH 512
#define REPEAT_COUNT 100000

using namespace std;

int main(int argc, char **) {

    long num = rand();
    srand(time(&num));

    int i, j, count;
    char message_buffer[MESSAGE_LENGTH+1] = {0};
    char filename_buffer[256] = {0};

    const char host[] = "127.0.0.1\0";
    unsigned int port = 5000;

    ClientProxy proxy = ClientProxy(host, port);

    if (proxy.setup() < 0) {

        printf("Error: %s.\n", strerror(errno));

    }

    sprintf(filename_buffer, "times/times_%i.txt", MESSAGE_LENGTH);
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

        // ...

        CallStack stack = CallStack();
        stack.addChars(message_buffer);

        CallStack lowerResponse = proxy.invoke("tolower", stack);
        CallStack upperResponse = proxy.invoke("toupper", stack);

        //printf("lower %s\n", lowerResponse.serialize());
        //printf("upper %s\n", upperResponse.serialize());
        //printf("\n");

        // ...

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
