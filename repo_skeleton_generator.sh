#------------ creating the folders ------------

mkdir -p src/hardware
mkdir -p src/vm
mkdir -p src/parser
mkdir -p tests
mkdir -p web


#------------ creating the hardware files ------------

touch src/hardware/chips.c
touch src/hardware/chips.h


#------------ creating the higher-level hardware files ------------

touch src/hardware/adders.c
touch src/hardware/adders.h

touch src/hardware/alu.c
touch src/hardware/alu.h

touch src/hardware/memory.c
touch src/hardware/memory.h

touch src/hardware/cpu.c
touch src/hardware/cpu.h


#------------ VM and parser ------------

touch src/vm/vm.c
touch src/vm/vm.h

touch src/parser/parser.c
touch src/parser/parser.h


#------------ main ------------

touch src/main.c


#------------ project files ------------

touch README.md
touch Makefile
