## Pre-requirements

Fuzzing test is performed using `libFuzzer` library available [here](https://llvm.org/docs/LibFuzzer.html).


----

## Compiling Fuzz Test

 1. Assume the project was cloned into the folder: `rfc7748_precomputed/`
 2. Compile the source code:

```sh
 $ mkdir rfc7748_precomputed/build
 $ cd rfc7748_precomputed/build
 $ cmake ..
 $ make 
```

 3. You can get `libFuzzer` running:

```sh
 $ cd rfc7748_precomputed/fuzz
 $ make get_fuzzer
```
This will download a Clang compiler with the fuzzer library enabled.

 4. Compile the fuzz test programs:

```sh
 $ make 
```

## Running Fuzz Test
After compilation two testing programs will be generated. To test keygen function run:

```sh
 $ ./fuzz_hacl_keygen
 $ ./fuzz_hacl_shared
 $ ./fuzz_gmp_add
```
Each program will stop when an error is found; otherwise, it will run forever. You can stop the execution using CTRL+C (^C).

----

## Known issues
If clang is requesting the `libtinfo` library, you can install the following `ncurses` library:

```sh
 $ dnf install ncurses ncurses-compat-libs
```
----
