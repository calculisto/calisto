include ../config.mk

.PHONY: all clean

all: server property.so

server: server.o

server.o: server.cpp server.hpp logger.hpp request_processor.hpp module.hpp method.hpp

schemas.hpp: ../data/*.json ../data/generate_header.sh
	cd ../data; ./generate_header.sh
	mv ../data/schemas.hpp .

property.so: property.cpp logger.hpp method.hpp
	$(CXX) $(CXXFLAGS) -shared -fPIC -o property.so property.cpp -lfmt

clean:
	rm -f *.o *.so server *.log
