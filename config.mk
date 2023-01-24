DOCTEST_HEADERS=../../external/onqtam/doctest
DEPENDENCIES_HEADERS=                             \
	../../external/yhirose/cpp_httplib            \
	../../external/jarro2783/cxxopts/include      \
	../../external/gabime/spdlog/include          \
	../../external/taocpp/pegtl/include           \
	../../external/taocpp/json/include            \
	../../external/isto/json_validator/include    \
	../../external/isto/uri/include               \
	../../external/isto/iapws/include             \
	../../external/isto/array/include             \
	../../external/isto/uncertain_value/include   \
	../../external/isto/multikey_multimap/include \
	../../external/isto/units/include             \
	../../external/isto/hash_combine/include      \
	../../external/isto/template_pow/include      \
	../../external/isto/root_finding/include      \

PROJECT=remote_services
LINK.o=${LINK.cc}
#CXXFLAGS+=-O3 -g -std=c++2a -Wall -Wextra $(foreach dir, ${DEPENDENCIES_HEADERS}, -I../${dir})
CXXFLAGS+=-g -std=c++2a -Wall -Wextra $(foreach dir, ${DEPENDENCIES_HEADERS}, -I../${dir})
LDLIBS+= -lfmt -ldl -lpthread

