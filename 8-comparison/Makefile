# Since the programs in this directories are examples for the user, this
# make file should be as ordinary as possible.  It should not rely heavily
# on included make files or configuration parameters.  It should not use
# libtool.  Also, we don't try to build or rebuild the libraries on which
# these programs depend.


ifeq ($(SRCDIR),)
SRCDIR = $(CURDIR)/../..
BLDDIR = $(SRCDIR)
endif

#include $(BLDDIR)/config.mk

default: all

CXXFLAGS = $(CFLAGS_PERSONAL) $(CADD)
LDFLAGS += $(LADD)

# If this were a real application, working from an installed copy of
# Xmlrpc-c, XMLRPC_C_CONFIG would just be 'xmlrpc-c-config'.  It would be
# found in the user's PATH.
#XMLRPC_C_CONFIG = $(BLDDIR)/xmlrpc-c-config.test
XMLRPC_C_CONFIG = xmlrpc-c-config

SERVERPROGS_ABYSS = rpc_server
CLIENTPROGS = rpc_client

PROGS =
PROGS += $(SERVERPROGS_ABYSS)
PROGS += $(CLIENTPROGS)

INCLUDES = `Magick++-config --cxxflags --cppflags --ldflags` -lz -I. $(shell $(XMLRPC_C_CONFIG) c++2 client abyss-server --cflags)

LIBS_SERVER_ABYSS = $(shell $(XMLRPC_C_CONFIG) c++2 abyss-server --libs)
LIBS_SERVER_CGI = $(shell $(XMLRPC_C_CONFIG) c++2 cgi-server --libs)
LIBS_CLIENT = $(shell $(XMLRPC_C_CONFIG) c++2 client --libs)
LIBS_BASE = $(shell $(XMLRPC_C_CONFIG) c++2 --libs)

$(shell g++ -c SHA256.cpp)
$(shell g++ -c Blowfish.cpp)
$(shell g++ -c Compression.cpp)

all: $(PROGS)

CXXLD = g++ `Magick++-config --cxxflags --cppflags --ldflags` SHA256.o Blowfish.o Compression.o -lz

$(SERVERPROGS_ABYSS):%:%.o
	$(CXXLD) -o $@ $^ $(LIBS_SERVER_ABYSS) $(LDFLAGS)

$(CLIENTPROGS):%:%.o
	$(CXXLD) -o $@ $^ $(LIBS_CLIENT) $(LDFLAGS)

LIBS_PSTREAM_CLIENT = $(shell $(XMLRPC_C_CONFIG) c++2 client --libs)

pstream_client:%:%.o
	$(CXXLD) -o $@ $^ $(LIBS_PSTREAM_CLIENT) $(LDFLAGS)

LIBS_PSTREAM_SERVER = $(shell $(XMLRPC_C_CONFIG) c++2 pstream-server --libs)

pstream_inetd_server pstream_serial_server:%:%.o
	$(CXXLD) -o $@ $^ $(LIBS_PSTREAM_SERVER) $(LDFLAGS)

OBJECTS = $(patsubst %,%.o,$(patsubst %.cgi,%_cgi,$(PROGS)))

$(OBJECTS):%.o:%.cpp
	$(CXX) -c $(INCLUDES) $(CXXFLAGS) $<

# See example/Makefile for an explanation of config.h and xmlrpc_amconfig.h

#$(OBJECTS): config.h xmlrpc_amconfig.h

#config.h:
#	$(LN_S) $(BLDDIR)/xmlrpc_config.h $@
#xmlrpc_amconfig.h:
#	$(LN_S) $(BLDDIR)/$@ .

.PHONY: clean
clean:
	rm -f $(PROGS) *.o config.h xmlrpc_amconfig.h
	rm -f tcp_client tcp_server udp_client udp_server
	$(MAKE) -C cpp clean
