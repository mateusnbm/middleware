clang -pthread -c $1_client.cpp
clang -pthread -o $1_client $1_client.o
./$1_client $2
