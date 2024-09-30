---
title: 'CALISTO'
tags:
 - C++
authors:
 - given-names: Emmanuel
   surname: Le Trong
   orcid: 0000-0001-5163-1775
   affiliation: 1
affiliations:
 - name: ISTO, UMR 7327, Univ Orleans, CNRS, BRGM, OSUC, F-45071 Orléans, France
   index: 1
---

# Summary


accessibility
trustworthiness

result: value, uncertainty, bibliographic reference, digital signature?

model / interface separation
availability from any language or system that can do an HTTP request
schema: automatic input validation, automatic documentation (formal API description), automatic
references with results, automatic interface generation?

C++20 expressiveness and readability.
C++ generality, can handle arbitrary precision, propagation of uncertainty,
physical units, yet unforeseen features.

search among functions

simpler development flow

# Statement of need

`CALISTO` is a `C++` framework intended to (1) facilitate the implementation of
numerical models via a set of support libraries, and (2) improve their 
accessibility by making them available online.

The source code of `CALISTO` is split into several repositories that are provided
as `git` sub-modules to the main repository `calculisto/calisto`, for convenience.

## Implementation of models

Models are implemented as C++ generic functions (a.k.a. function templates)
which are expressive, readable and match nicely the equations as they are 
typeset in the publication accompanying the model. The use of generic functions 
allows perks such as arbitrary precision values, automatic propagation of
uncertainties, automatic conversion between physical units, or any yet
unforeseen feature. The `CALISTO` framework provides optional support for the 
propagation of uncertainties (in the `git` sub-module 
`calculisto/uncertain_value`) and physical units conversions (in the `git`
sub-module `calculisto/units`). 

Models are made available as standalone libraries (e.g. `calculisto/iapws`) 
that can be downloaded and integrated into personal projects. 

## Accessibility 

They are also grouped into thematic modules and made available online through 
the `CALISTO` server. . At the moment only one such module is provided: the 
`property` module, that computes
some thermodynamic properties of a given substances at given conditions. 

The `CALISTO` HTTP server accepts POST requests containing a JSON RPC request 
object as payload. The request describes which model of which module is to be 
used and what its inputs are. The server returns the results of the calculations 
in the form of a JSON RPC response object, in the HTTP response message.

TODO: example: send a JSON req, receive a JSON resp.

A module is accompanied by a JSON Schema document that specifies its API (what
methods it provides, what inputs it expects and outputs it produces), and some 
documentation. 

To be integrated into (a module of) the server, a new model comes with a JSON
document that describes the functions it provides and the bibliographic
reference(s) it implements. 


TODO: must conform to schema

TODO automatic input validation

TODO: JSON doc specific to property module but the approach will be generalized. 

TODO: automatic generation of glue code between the underlying lib and the
property module

## Automation

TODO: clairfy: I/O are JSON documents
TODO: Why is it a schema and not a document? -> validate models JSON and build
doc.

TODO: module + json shcema + models + json documents -> automatic

TODO thourough testing



The implementation of models in modern C++, with the aid of `CALISTO`
support libraries, results in highly expressive and readable source code, that
match nicely the equations are they are typeset in the publication accompanying 
the model. TODO: example

The framework consists of two parts. One is a set of support libraries that
simplify the implementation. The other is a facility to make the models
available online.


The support libraries then
automatize perks, such as, but not limited to, automatic propagation of 
uncertainties (if any), conversion between physical units (if any), arbitrary
precision values. 




Models are implemented in
modern C++ 

Using 
