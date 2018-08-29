if [[ $1 == tcp ]]; then
    clang -pthread -c tcp_server.cpp
    clang -pthread -o tcp_server tcp_server.o
    clang -pthread -c tcp_client.cpp
    clang -pthread -o tcp_client tcp_client.o
    ./tcp_server &
    ./tcp_client &
else
    clang -pthread -c udp_server.cpp
    clang -pthread -o udp_server udp_server.o
    clang -pthread -c udp_client.cpp
    clang -pthread -o udp_client udp_client.o
    ./udp_server &
    ./udp_client &
fi
