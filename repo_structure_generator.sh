#!/bin/bash

# ------------ Creating folders ------------

mkdir -p src/hardware
mkdir -p src/parser
mkdir -p src/vm
mkdir -p tests
mkdir -p web


# ------------ Hardware ------------

touch src/hardware/chips.cpp
touch src/hardware/chips.h

touch src/hardware/adders.cpp
touch src/hardware/adders.h

touch src/hardware/alu.cpp
touch src/hardware/alu.h

touch src/hardware/memory.cpp
touch src/hardware/memory.h

touch src/hardware/cpu.cpp
touch src/hardware/cpu.h


# ------------ Parser ------------

touch src/parser/parser.cpp
touch src/parser/parser.h


# ------------ VM ------------

touch src/vm/vm.cpp
touch src/vm/vm.h


# ------------ Main ------------

touch src/main.cpp


# ------------ Project files ------------

touch Makefile
touch README.md


echo "C++ emulator repository structure created."
