if [[ $1 == tcp ]]; then
    clang -pthread -c sockets_tcp.cpp
    clang -pthread -o sockets_tcp sockets_tcp.o
    ./sockets_tcp
else
    clang -pthread -c sockets_udp.cpp
    clang -pthread -o sockets_udp sockets_udp.o
    ./sockets_udp
fi
