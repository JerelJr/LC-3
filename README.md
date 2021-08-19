# LC-3 Virtual Machine

A simple virtual machine made with the LC-3 instruction set.

To make your own version of this project follow this [online tutorial](https://justinmeiners.github.io/lc3-vm/) and this [ISA pdf](https://justinmeiners.github.io/lc3-vm/supplies/lc3-isa.pdf). It works on Linux and Windows machines (should also work on Mac OS but has not been tested). This version of the LC-3 project comes with additional comments, a few added parts for functionality on Windows, and a bit more organization. The test folder contains two files to run the VM on.

## Compiling 
A simple makefile has been provided. For a normal compilation, simply type: 
```
make lc3
```
For debugging:
```
make debug
```
And to clean:
```
make clean
```
## Running
This vm runs code written in the LC3 language. Two test files have been provided in the `test` folder. Here is an example run command:
```
./lc3 test/rogue.obj
```