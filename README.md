# CALISTO 

## Introduction
CALISTO is the framework set up at the [ISTO](https://www.isto-orleans.fr) 
laboratory to implement scientific numerical models, and make them globally 
available to everyone.

Once implemented, a model can be interrogated by sending a 
[JSON RPC](https://www.jsonrpc.org) request
as the payload of an HTTP POST request to a public server. 
The result (or an error) is sent back in the HTTP response body.

For example, one can send the following request from the command line, which
asks a specific model (codename `ISTO_IAPWS_G12_INVERSE`, part of the `property`
module) for the pressure of supercooled water at some given temperature (235 K) 
and density (968 kg/m3).
```sh
$ curl -s https://rs-isto.cnrs-orleans.fr:7327 -d '{
  "jsonrpc": "2.0",
  "id": '$RANDOM',
  "method": "property/execute_function",
  "params": {
    "signature": {
      "substance": "supercooled_water",
      "property": "pressure",
      "model": "ISTO_IAPWS_G12_INVERSE",
      "conditions": [
        "temperature",
        "density"
      ]
    },
    "arguments": {
      "temperature": {
        "value": 235,
        "unit": "K"
      },
      "density": {
        "value": 968,
        "unit": "kg.m-3"
      }
    }
  }
}'
```
The HTTP response contains the following body:
```json
{
  "id": 7288,
  "jsonrpc": "2.0",
  "references": [
    "10.1063/1.4895593"
  ],
  "result": {
    "uncertainty": 0.0,
    "unit": "Pa",
    "value": 101327.76794618522
  }
}
```
According to this model, the pressure is approximately 101327 Pa.

Models are grouped into modules. At the moment only one such module is
implemented, the `property` module. It provides the thermodynamic properties of
water, in various states, at some given conditions.

## Documentation

Documentation is available online here: 
https://calcul-isto.cnrs-orleans.fr/calisto/

Most of the documentation is generated automatically from the JSON Schema
describing the server's capabilities, which can be obtained with this command
line:
```sh
curl -s https://rs-isto.cnrs-orleans.fr:7327 \
  -d '{"jsonrpc":"2.0","id":'$RANDOM',"method":"'get_schema'","params":{}}'
```

A prototype of an interactive web interface to the `property` module is 
available here: https://calcul-isto.cnrs-orleans.fr/calisto/property/

## Building

This
```sh
git clone --recurse-submodules git://github.com/calculisto/calisto.git
cd calisto
make -j
```
will build the server and the `property` module. It requires a C++20 capable
compiler and GNU Make.

### Source repositories

The whole framework is split in distinct, self-contained repositories. This
particular repository only contains the code for the HTTP server and the 
`property` module.

The other repositories participating in this project are
 - [`calculisto/array`](https://github.com/calculisto/array) 
 implements a `std::array` with associated arithmetic operators and mathematical
 functions.
 - [`calculisto/iapws`](https://github.com/calculisto/iapws) 
 implements some thermodynamic models from the [IAPWS](http://www.iapws.org/). 
 It currently forms the basis of the `property` module.
 - [`calculisto/json_validator`](https://github.com/calculisto/json_validator)
 implements a validator for JSON documents with respect to a 
 [JSON Schema](https://json-schema.org/).
 - [`calculisto/multikey_multimap`](https://github.com/calculisto/multikey_multimap)
 implements a multi-key, multi-value map.
 - [`calculisto/root_finding`](https://github.com/calculisto/root_finding)
 implements some root-finding algorithms.
 - [`calculisto/uncertain_value`](https://github.com/calculisto/uncertain_value)
 implements a value-with-attached-uncertainty type, along with the necessary
 arithmetic operators and mathematical function.
 - [`calculisto/units`](https://github.com/calculisto/units)
 implements a physical units system.
 - [`calculisto/uri`](https://github.com/calculisto/uri)
 implements a URI type.

For convenience they are included in this repository as git sub-modules .

### Dependencies
In addition to the above listed, in-house, repositories, the project also 
depends on the following external repositories:
 - [`gabime/spdlog`](https://github.com/gabime/spdlog) for logging.
 - [`jarro2783/cxxopts`](https://github.com/jarro2783/cxxopts) to parse the 
 server's executable options.
 - [`taocpp/json`](https://github.com/taocpp/json) to parse JSON.
 - [`taocpp/pegtl`](https://github.com/taocpp/pegtl) a generic parsing tool, 
 this is a dependency of `taocpp/pegtl` (and `calculisto/uri`).
 - [`yhirose/cpp_httplib`](https://github.com/yhirose/cpp_httplib) for the HTTP
 server.

For convenience, those are also included in this repository as git sub-modules.

Other dependencies that are not provided as sub-modules are:
 - [doctest/doctest](https://github.com/doctest/doctest), and
 - [fmtlib/fmt](https://github.com/fmtlib/fmt).
These are usually provided by your GNU/Linux distribution.

## Design considerations

### Implementation
The framework is implemented in C++20. 

We value correctness and expressiveness above performance. As a result, this 
code probably has a lot of opportunity for optimization.

### Security
This server does not have HTTPS capabilities. If security is of concern, it
should be hosted behind an adequately configured reverse-proxy.

### Tests
Each repository has its own unit test suite. The tests in this repository serve 
as integration tests. 

## Affiliation

This material is developed by the Numerical Modelling platform at the 
Institut des Sciences de la Terre d'Orléans (https://www.isto-orleans.fr), 
a joint laboratory of the University of Orléans (https://www.univ-orleans.fr), 
the french National Center For Scientific Research (https://www.cnrs.fr) and 
the french Geological Survey (https://www.brgm.fr).

![logo ISTO](https://calcul-isto.cnrs-orleans.fr/logos/isto-156.png) &emsp;
![logo CNRS](https://calcul-isto.cnrs-orleans.fr/logos/cnrs-128.png) &emsp;
![logo UO](https://calcul-isto.cnrs-orleans.fr/logos/uo-180.png) &emsp;
![logo BRGM](https://calcul-isto.cnrs-orleans.fr/logos/brgm-256.png)
