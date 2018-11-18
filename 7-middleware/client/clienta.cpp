//
// client.cpp
//

#import <cstdlib>
#import <string>
#import <iostream>
#import <vector>
#import "../middleware/CallStack.h"
#import "../middleware/ClientProxy.h"
#include "zlib.h"

using namespace std;



unsigned long file_size(const char filename[]) {

    FILE *pFile = fopen(filename, "rb");
    fseek (pFile, 0, SEEK_END);
    unsigned long size = ftell(pFile);
    fclose (pFile);
    return size;

}

int decompress_one_file(const char infilename[], const char outfilename[]) {

    gzFile infile = gzopen(infilename, "rb");
    FILE *outfile = fopen(outfilename, "wb");
    if (!infile || !outfile) return -1;

    char buffer[128];
    int num_read = 0;

    while ((num_read = gzread(infile, buffer, sizeof(buffer))) > 0) {

        fwrite(buffer, 1, num_read, outfile);

    }

    gzclose(infile);
    fclose(outfile);

    return 0;

}

int compress_one_file(const char infilename[], const char outfilename[]) {

    FILE *infile = fopen(infilename, "rb");
    gzFile outfile = gzopen(outfilename, "wb");
    if (!infile || !outfile) return -1;

    char inbuffer[128];
    int num_read = 0;

    unsigned long total_read = 0, total_wrote = 0;

    while ((num_read = fread(inbuffer, 1, sizeof(inbuffer), infile)) > 0) {

        total_read += num_read;

        gzwrite(outfile, inbuffer, num_read);

    }

    fclose(infile);
    gzclose(outfile);

    printf("Read %ld bytes, Wrote %ld bytes, Compression factor %4.2f%%\n", total_read, file_size(outfilename), (1.0-file_size(outfilename)*1.0/total_read)*100.0);

    return 0;

}




int main(int argc, char ** argv) {

    //compress_one_file("client/malu.jpg", "client/apple_compressed");
    //decompress_one_file("client/apple_compressed", "client/apple_decompressed.jpeg");

    compress_one_file("client/mateus.txt", "client/mateus_compressed");
    decompress_one_file("client/mateus_compressed", "client/mateus_decompressed.txt");

    // ...

    return 0;



    printf("\n");

    // Prepare a sample call stack to demonstrate the usage of all
    // serialization capabilities.

    CallStack sampleStack = CallStack();

    char name[] = "Mateus Nunes de Barros";
    static const int ints_arr[] = {1, 2 , 3, 4, 5};
    vector <int> ints (ints_arr, ints_arr + sizeof(ints_arr) / sizeof(ints_arr[0]));
    static const float floats_arr[] = {1.5, 2.3 , 3.5, 4.8, 5.4};
    vector <float> floats (floats_arr, floats_arr + sizeof(floats_arr) / sizeof(floats_arr[0]));

    sampleStack.addConstChars(name);
    sampleStack.addConstChars("Mateus Nunes de Barros Magalhaes");
    sampleStack.addString(string("Mateus Nunes"));
    sampleStack.addInteger(1024);
    sampleStack.addFloat(3.14);
    sampleStack.addBoolean(false);
    sampleStack.addVector(ints);
    sampleStack.addVector(floats);
    sampleStack.addFile("client/apple.jpg", true);

    char * sampleStackSerialization = sampleStack.serialize();
    printf("[Client] Sample Call Stack Serialization:\n\n%s\n\n", sampleStackSerialization);
    free(sampleStackSerialization);

    // Prepate five simple call stacks to demonstrate that the server is able
    // to handle multiples connections at the same time. They will have only a
    // integer that specifies the number of seconds the server should wait
    // before returning somehting.

    CallStack simpleStackA = CallStack();
    CallStack simpleStackB = CallStack();
    CallStack simpleStackC = CallStack();
    CallStack simpleStackD = CallStack();
    CallStack simpleStackE = CallStack();

    simpleStackA.addInteger(8);
    simpleStackB.addInteger(5);
    simpleStackC.addInteger(6);
    simpleStackD.addInteger(2);
    simpleStackE.addInteger(4);

    char * simpleStackSerializationA = simpleStackA.serialize();
    char * simpleStackSerializationB = simpleStackB.serialize();
    char * simpleStackSerializationC = simpleStackC.serialize();
    char * simpleStackSerializationD = simpleStackD.serialize();
    char * simpleStackSerializationE = simpleStackE.serialize();

    printf("[Client] Sample Call Stack A Serialization:\n\n%s\n\n", simpleStackSerializationA);
    printf("[Client] Sample Call Stack B Serialization:\n\n%s\n\n", simpleStackSerializationB);
    printf("[Client] Sample Call Stack C Serialization:\n\n%s\n\n", simpleStackSerializationC);
    printf("[Client] Sample Call Stack D Serialization:\n\n%s\n\n", simpleStackSerializationD);
    printf("[Client] Sample Call Stack E Serialization:\n\n%s\n\n", simpleStackSerializationE);

    free(simpleStackSerializationA);
    free(simpleStackSerializationB);
    free(simpleStackSerializationC);
    free(simpleStackSerializationD);
    free(simpleStackSerializationE);

    // Initialize the connection.

    const char host[] = "127.0.0.1\0";
    unsigned int port = atoi(argv[1]);

    ClientProxy sampleProxy = ClientProxy(host, port);

    sampleProxy.secure("secret-password");

    ClientProxy simpleProxyA = ClientProxy(host, port);
    ClientProxy simpleProxyB = ClientProxy(host, port);
    ClientProxy simpleProxyC = ClientProxy(host, port);
    ClientProxy simpleProxyD = ClientProxy(host, port);
    ClientProxy simpleProxyE = ClientProxy(host, port);

    simpleProxyA.secure("secret-password");
    simpleProxyB.secure("secret-password");
    simpleProxyC.secure("secret-password");
    simpleProxyD.secure("secret-password");
    simpleProxyE.secure("secret-password");

    // Use the sample call stack to demonstrate the serialization capabilities.

    if (sampleProxy.setup() < 0) {

        printf("[Client] Error:\n\n%s.\n\n", strerror(errno));

        return 0;

    }

    sampleProxy.invoke("sample", sampleStack);

    // Demonstrate that the server can handle multiple connections.

    if (simpleProxyA.setup() < 0 ||
        simpleProxyB.setup() < 0 ||
        simpleProxyC.setup() < 0 ||
        simpleProxyD.setup() < 0 ||
        simpleProxyE.setup() < 0) {

        printf("[Client] Error:\n\n%s.\n\n", strerror(errno));

        return 0;

    }

    simpleProxyA.invoke("sleeper", simpleStackA);
    simpleProxyB.invoke("sleeper", simpleStackB);
    simpleProxyC.invoke("sleeper", simpleStackC);
    simpleProxyD.invoke("sleeper", simpleStackD);
    simpleProxyE.invoke("sleeper", simpleStackE);

    return 0;

}
