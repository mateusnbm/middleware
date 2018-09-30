//
// udp_client.cpp
//
// 128 bytes = 0,125 KB
// 256 bytes = 0,25 KB
// 1024 bytes = 1 KB
// 51200 bytes = 50 KB
// 102400 bytes = 100 KB
//

#import <stdio.h>
#import <stdlib.h>
#import <pthread.h>

#import <sys/socket.h>
#import <netinet/in.h>
#import <arpa/inet.h>
#import <unistd.h>
#import <errno.h>
#import <string.h>
#import <sys/types.h>
#import <time.h>

//#define PACKET_LENGTH 5000
//#define MESSAGE_LENGTH 100000

#define PACKET_LENGTH 4096
//#define MESSAGE_LENGTH 49152
#define MESSAGE_LENGTH 98304

//#define PACKET_LENGTH 10240
//#define MESSAGE_LENGTH 10240 // 10 KB
//#define MESSAGE_LENGTH 102400 // 100 KB
//#define MESSAGE_LENGTH 512000 // 500 KB

#define COMMAND_LENGTH 128
#define MAX_CONNECTIONS 16
#define DEFAULT_PORT 5000
#define REPEAT_COUNT 10000

int main (int argc, const char * argv[]) {

    int raux;
    int port_number;

    int socket_descriptor;
    int client_socket_descriptor;
    struct sockaddr_in server_addr_descriptor;

    // Get the port number as a command line argument.

    port_number = argc < 2 ? DEFAULT_PORT : atoi(argv[1]);

    // Create socket (SOCK_DGRAM specifies UDP).

    socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);

	if (socket_descriptor == -1) {

        printf("[Client] Failed to create endpoint for communication.\n");
        printf("[Client] Error: %s.", strerror(errno));

        return 0;

	}

	memset(&server_addr_descriptor, '0', sizeof(server_addr_descriptor));

	server_addr_descriptor.sin_family = AF_INET;
	server_addr_descriptor.sin_port = htons(port_number);

    // Convert url address to its binary representation.

    raux = inet_pton(AF_INET, "127.0.0.1", &server_addr_descriptor.sin_addr);

    if (raux == 0) {

        printf("[Client] Buffer is not a valid string.\n");

        return 0;

    } else if (raux < 0) {

        printf("[Client] Failed to convert internet address to binary format.\n");
        printf("[Client] Error: %s.", strerror(errno));

        return 0;

    }

    // Information like IP address of the remote host and its port is bundled
    // up in a structure and a call to function connect() is made which tries
    // to connect this socket with the socket (IP address and port) of the
    // remote host.

    raux = connect(socket_descriptor, (struct sockaddr *)&server_addr_descriptor, sizeof(server_addr_descriptor));

	if (raux == -1) {

        printf("[Client] Failed to initiate connection.\n");
        printf("[Client] Error: %s.", strerror(errno));

        return 0;

	}

    // ...

    int i, j, count;
    char message_buffer[MESSAGE_LENGTH] = {0};
    char command_buffer[COMMAND_LENGTH] = {0};
    char filename_buffer[256] = {0};

    sprintf(filename_buffer, "times/times_udp_%i_%i.txt", PACKET_LENGTH, MESSAGE_LENGTH);

    long stime, etime, delta;
    FILE *logs_file = fopen(filename_buffer, "w+");

    clockid_t clk_id = CLOCK_MONOTONIC;
    struct timespec time_spec;

    for (i = 0; i < REPEAT_COUNT; i++) {

        //printf("%i\n", i);

        // Get the current clock time in nanoseconds.

        memset(&time_spec, '0', sizeof(timespec));
        clock_gettime(clk_id, &time_spec);
        stime = time_spec.tv_nsec;

        // Say hello and receive the random message.

        memset(command_buffer, '0', COMMAND_LENGTH);
        strcpy(command_buffer, "hello\0");

        count = sendto(
            socket_descriptor,
            command_buffer,
            COMMAND_LENGTH,
            0,
            (struct sockaddr *) NULL,
            sizeof(server_addr_descriptor)
        );

        for (j = 0; j < MESSAGE_LENGTH; j += PACKET_LENGTH) {

            recvfrom(
                socket_descriptor,
                message_buffer + (j * sizeof(char)),
                PACKET_LENGTH,
                0,
                (struct sockaddr *) NULL,
                NULL
             );

        }

        // Compute the time delta in microseconds and write it to a file.

        memset(&time_spec, '0', sizeof(timespec));
        clock_gettime(clk_id, &time_spec);
        etime = time_spec.tv_nsec;
        delta = (etime - stime) / 1000;

        //printf("%ld %ld\n", etime, stime);
        //printf("%ld\n", delta);

        count = sprintf(message_buffer, "%ld,", delta);
        count = fwrite(message_buffer, sizeof(char), count, logs_file);

    }

    // Tell the server to terminate it's activities.

    memset(command_buffer, '0', COMMAND_LENGTH);
    strcpy(command_buffer, "terminate\0");

    sendto(
        socket_descriptor,
        command_buffer,
        COMMAND_LENGTH,
        0,
        (struct sockaddr *) NULL,
        sizeof(server_addr_descriptor)
    );

    // Terminate connections.

    fclose(logs_file);
    close(socket_descriptor);

    return 0;

}
