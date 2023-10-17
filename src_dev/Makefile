include ../config.mk


.PHONY: all clean

all: build server property.so

build: build.cpp

map_data.inc: build
	./build *.json

server: server.o

server.o: server.cpp server.hpp logger.hpp request_processor.hpp module.hpp method.hpp schemas.hpp 

property.so: property.cpp logger.hpp method.hpp map_data.inc
	$(CXX) $(CXXFLAGS) -shared -fPIC -o property.so property.cpp -lfmt

clean:
	rm -f *.o *.so server *.log *.inc
