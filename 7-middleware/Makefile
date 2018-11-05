#
# Makefile
#

default: all

all:

	$(shell g++ -c middleware/Method.cpp)
	$(shell g++ -c middleware/Call.cpp)
	$(shell g++ -c middleware/CallStack.cpp)
	$(shell g++ -c middleware/Request.cpp)
	$(shell g++ -c middleware/ClientProxy.cpp)
	$(shell g++ -c middleware/Invoker.cpp)

	$(shell g++ -c middleware/ClientRequestHandler.cpp)
	$(shell g++ -c middleware/ServerRequestHandler.cpp)

	$(shell g++ -c server/Lower.cpp)
	$(shell g++ -c server/Upper.cpp)

	$(shell g++ Call.o CallStack.o Request.o ClientProxy.o ClientRequestHandler.o -o mclient client/client.cpp)
	$(shell g++ Call.o CallStack.o Request.o ServerRequestHandler.o Invoker.o Method.o Lower.o Upper.o -o mserver server/server.cpp)

clean:

	rm -f *.o mclient mserver
