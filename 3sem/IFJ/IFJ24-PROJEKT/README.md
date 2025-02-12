# IFJ24 Projekt - VUT FIT
## POZOR - TOTO NIEJE OFICIALNE DOKUMENTACIA

### Automatické spustenie programu: 

./compile-run.sh -> automaticky skompiluje a executne program + pipene do stdin test.zig
./compile -> length skompiluje

### Manuálne spustenie programu: 
Máme full cmakefile t.j:

mkdir build && cd build
cmake ..
make -j 4
./ifjcompiler
