# CTools

## Whatsup

This repository is intended to collect tools for building, analyze
and instrument source code in C/C++.
Also it may include other things : documentation generation,
test framework, ...

## Building

Assembly, preprocessing, object code.

## Coverage

Makefile use $(COVERAGE_FLAG) for compiling.
Linking stage need to include -lcov

## Profiling

Makefile use $(FPROFILE_FLAG)

## Callgraphs

Makefile use $(FANALYZER_FLAG) which create .dot along with object.
The .dot is then transformed with dot utility int a dedicated rule.

## Static Analysis
## Linter
## CMocka



