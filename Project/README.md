# The Double-slit Experiment
						![Double-slit Experiment Visualization](https://github.com/robinho46/DD2356/blob/main/Project/images/last_frame.png)
This repository contains four versions, two serial codes one for SFML to visualize the double-slit experiment on your local computer and one to be executed on Dardel. OpenMP and MPI has also their code versions.

# Dependencies
```bash
SFML:
libsfml-graphics.so.2.6
libsfml-window.so.2.6
libsfml-system.so.2.6
C++:
libstdc++.so.6
```
The code has only been tested with these dependencies, you may be able to use other version.

## Run serial code with SFML
The sfml version can be executed on a local computer.
### Installation
```bash
sudo apt-get update
sudo apt install libsfml-dev
```
### Compiling and Running
```bash 
cd DD2356/Project/serial
g++ -o waveEq mainSFML.cpp -lsfml-graphics -lsfml-window -lsfml-system`
./waveEq
```

## Job Script Documentation For Dardel

For comprehensive guidelines on how to write job scripts and run them on Dardel, please refer to the official PDC documentation:

- [Job Scripts on Dardel](https://www.pdc.kth.se/support/documents/run_jobs/job_scripts_dardel.html)

For instructions on how to run jobs interactively on Dardel, visit:

- [Running Interactively on Dardel](https://www.pdc.kth.se/support/documents/run_jobs/run_interactively.html)

## Run tests on dardel
```bash 
cd DD2356/Project/unitTests/
make
make test
# When tests are done:
make clean
```

## Compile serial code on Dardel
```bash
cd DD2356/Project/serial
CC -O2 -o main.cpp main.out
```

## Compile OpenMP code on Dardel
```bash
cd DD2356/Project/openMp/
CC -O2 -fopenmp main.cpp -o main.out
```

## Compile MPI code on Dardel
Note: MPI goes under the C++ compiler and doesn't have to be specified.
```bash
cd DD2356/Project/openMp/
CC  main.cpp -o main.out
```
# Documentation
```bash
sudo apt-get install doxygen
cd DD2356/Project/
doxygen Doxyfile```
