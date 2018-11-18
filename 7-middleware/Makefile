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
	$(shell g++ -c middleware/Blowfish.cpp)
	$(shell g++ -c middleware/SHA256.cpp)
	$(shell g++ -c middleware/Base64.cpp)
	$(shell g++ -c middleware/Compression.cpp)

	$(shell g++ -c middleware/ClientRequestHandler.cpp)
	$(shell g++ -c middleware/ServerRequestHandler.cpp)

	$(shell g++ -c server/Lower.cpp)
	$(shell g++ -c server/Upper.cpp)
	$(shell g++ -c server/Sample.cpp)
	$(shell g++ -c server/Sleeper.cpp)
	$(shell g++ `Magick++-config --cxxflags --cppflags --ldflags` -c server/Grayscale.cpp)

	$(shell g++ Call.o CallStack.o Request.o ClientProxy.o ClientRequestHandler.o Blowfish.o SHA256.o Base64.o Compression.o -lz -o clienta client/clienta.cpp)
	$(shell g++ Call.o CallStack.o Request.o ServerRequestHandler.o Invoker.o Blowfish.o SHA256.o Method.o Lower.o Upper.o Sample.o Sleeper.o Base64.o Compression.o -lz -o servera server/servera.cpp)

	$(shell g++ Call.o CallStack.o Request.o ClientProxy.o ClientRequestHandler.o Blowfish.o SHA256.o Base64.o Compression.o -lz -o clientb client/clientb.cpp)
	$(shell g++ `Magick++-config --cxxflags --cppflags --ldflags` Call.o CallStack.o Request.o ServerRequestHandler.o Invoker.o Blowfish.o SHA256.o Method.o Lower.o Upper.o Base64.o Sample.o Sleeper.o Grayscale.o Compression.o -lz -o serverb server/serverb.cpp)

clean:

	rm -f *.o *.compression clienta clientb servera serverb
