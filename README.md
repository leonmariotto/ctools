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
TODO: pydot provide a lot of functionality to highlight informations, produce subgraph,
and clean useless things. It may worth it to dev a script for that.

## Static Analysis
## MISRA checker

Need to install cppcheck.
The MISRA checker is a plugin of cppcheck. Configurations files is located under misra/ folder.
The misra Makefile rules build a report with single line per warning (can be used to
easily check if there is more error in CI and reject PR)

## Linter
## CMocka

Test is run with CMocka, it output a JUnit xml on stdout.
Cmocka is a submodule, so run :
```
git submodule update --init
```
When cloning repository.
