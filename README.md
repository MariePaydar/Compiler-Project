<h3>     </h3>
<h1 align="center"> Implementation of a compiler </h1>
<h3 align="center"> Compiler Design Fundamentals </h3>
<h5 align="center"> Final Project - <a href="https://en.sbu.ac.ir/">Shahid Beheshti University</a>(2023) </h5>
<h3>      </h3>


This project focuses on designing and implementing a simple compiler using llvm. To build the project, execute `build.sh`:

```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```
To compile your code, replace it with the code in `input.txt` and execute `run.sh`:
```bash
$ cd build
$ cd src
$ ./gsm "$(cat ../../input.txt)" > gsm.ll
$ llc --filetype=obj -o=gsm.o gsm.ll
$ clang -o gsmbin gsm.o ../../rtGSM.c
$ ./gsmbin
```
The compiler optimizer is designed in such a way that it considers a variable named `result` as the main output of the program and removes instructions that do not affect the value of `result`.
This compiler displays the value assigned in each assignment as `The result is:  `.

## Sample

Input:
```
int a = 9;
int result = 8;
result = 3;
result = 5 + a;
```

Output:
```
The result is: 14
```
