#
# Makefile
#

default: all

all:

	$(shell g++ -c Method.cpp)
	$(shell g++ -c Call.cpp)
	$(shell g++ -c CallStack.cpp)
	$(shell g++ -c Request.cpp)
	$(shell g++ -c ClientProxy.cpp)
	$(shell g++ -c Invoker.cpp)

	$(shell g++ -c ClientRequestHandler.cpp)
	$(shell g++ -c ServerRequestHandler.cpp)

	$(shell g++ Call.o CallStack.o Request.o ClientProxy.o ClientRequestHandler.o -o client client.cpp)
	$(shell g++ Call.o CallStack.o Request.o ServerRequestHandler.o Invoker.o Method.o -o server server.cpp)

clean:

	rm -f *.o client server
