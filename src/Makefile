.PHONY: all clean

CXXFLAGS+=                                                 \
  -std=c++20                                               \
  -Wall                                                    \
  -Wextra                                                  \
	-I../git_submodules/taocpp/pegtl/include                 \
	-I../git_submodules/taocpp/json/include                  \
	-I../git_submodules/gabime/spdlog/include                \
	-I../git_submodules/yhirose/cpp_httplib                  \
  -I../git_submodules/jarro2783/cxxopts/include            \
	-I../git_submodules/calculisto/array/include             \
	-I../git_submodules/calculisto/iapws/include             \
	-I../git_submodules/calculisto/json_validator/include    \
	-I../git_submodules/calculisto/multikey_multimap/include \
	-I../git_submodules/calculisto/root_finding/include      \
	-I../git_submodules/calculisto/uncertain_value/include   \
	-I../git_submodules/calculisto/units/include             \
	-I../git_submodules/calculisto/uri/include               \

LINK.o=${LINK.cc}

LDFLAGS+=-L../git_submodules/fmtlib/fmt/build

LDLIBS+=-lfmt

SRC_SERVER=             \
	server.cpp            \
	server.hpp            \
	request_processor.hpp \
	logger.hpp            \
	module.hpp            \
	method.hpp            \
	schemas.hpp           \

SRC_MODULE_PROPERTY=               \
	module_property.cpp              \
	logger.hpp                       \
	method.hpp                       \
	module_property_disambiguate.hpp \
	module_property_map_headers.inc  \
	module_property_map_data.inc     \

all: server module_property.so

server: server.o

server.o: ${SRC_SERVER}

module_property.so: ${SRC_MODULE_PROPERTY}
	${CXX} ${CXXFLAGS} -shared -fPIC -o module_property.so \
		-Lgit_submodules/fmtlib/fmt/build module_property.cpp -lfmt

module_property_map_headers.inc module_property_map_data.inc&: build ../assets/*.json
	./build ../assets/*.json

build: build.o

build.o: build.cpp

clean:
	rm -f *.o *.so server *.log *.inc
