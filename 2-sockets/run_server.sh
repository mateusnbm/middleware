clang -pthread -c $1_server.cpp
clang -pthread -o $1_server $1_server.o
./$1_server $2
