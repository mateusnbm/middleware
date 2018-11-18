//
// clientb.cpp
//

#import <cstdlib>
#import <string>
#import <iostream>
#import <vector>
#import "../middleware/CallStack.h"
#import "../middleware/ClientProxy.h"
#import "../middleware/Base64.h"

using namespace std;

int main(int argc, char ** argv) {

    const int repeat_count = 10000;
    const char times_filename[] = "times/encryption.txt";

    const char host[] = "127.0.0.1\0";
    unsigned int port = atoi(argv[1]);

    int count = 0;
    int file_index = 0;
    char buffer[128] = {0};
    long stime, etime, delta;
    struct timespec time_spec;
    clockid_t clk_id = CLOCK_MONOTONIC;

    FILE * times_file = fopen(times_filename, "w+");

    // Initialize the connection.

    ClientProxy proxy = ClientProxy(host, port);

    //proxy.compress();
    proxy.secure("secret-password");

    if (proxy.setup() < 0) {

        printf("[Client] Error:\n\n%s.\n\n", strerror(errno));

        return 0;

    }

    // Place calls.

    for (int i = 0; i < repeat_count; i++) {

        string name = to_string(file_index++);
        CallStack stack = CallStack();
        stack.addString(name);
        stack.addFile((i % 2 == 0) ? "client/apple.jpg" : "client/google.jpg");

        memset(&time_spec, '0', sizeof(timespec));
        clock_gettime(clk_id, &time_spec);
        stime = time_spec.tv_nsec;

        CallStack response = proxy.invoke("grayscale", stack);

        memset(&time_spec, '0', sizeof(timespec));
        clock_gettime(clk_id, &time_spec);
        etime = time_spec.tv_nsec;
        delta = (etime - stime) / 1000;

        count = sprintf(buffer, "%ld,", delta);
        count = fwrite(buffer, sizeof(char), count, times_file);

        int grayscale_image_len = 0;
        char * grayscale_image_data = response.getFileAtIndex(0, &grayscale_image_len);

        string filepath = "tmp/" + name + "-grayscale.jpg";

        char * c_filepath = (char *) malloc(sizeof(char)*(filepath.size()+1));
        std::copy(filepath.begin(), filepath.end(), c_filepath);
        c_filepath[filepath.size()] = '\0';
        int c_filepath_len = filepath.size();

        FILE * out = fopen(c_filepath, "wb+");
        fwrite(grayscale_image_data, sizeof(char), grayscale_image_len, out);
        fclose(out);

        free(grayscale_image_data);
        free(c_filepath);

    }

    // Terminate.

    fclose(times_file);

    return 0;

}
