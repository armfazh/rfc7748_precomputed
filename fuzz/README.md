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
 $ make fuzz
```

 3. You can get `libFuzzer` running:

```sh
 $ make get_fuzzer
```
This will download a Clang compiler with the fuzzer library enabled. 
Depending on your Internet speed, this may take a few minutes for completing this task.

 
## Running Fuzz Test
After compilation two testing programs will be generated fuzzing against HACL library:

```sh
 $ ./bin/fuzz_hacl_keygen
 $ ./bin/fuzz_hacl_shared
```
Each program will stop when an error is found; otherwise, it will run forever. You can stop the execution using CTRL+C (^C).

Additionally, there are fuzzing tests against the GMP library.
```sh
 $ ./bin/gmp_fp25519_add
```
Each program will stop when an error is found; otherwise, it will run forever. You can stop the execution using CTRL+C (^C).

----

## Known issues
If clang is requesting the `libtinfo` library, you can install the following `ncurses` library:

```sh
 $ dnf install ncurses ncurses-compat-libs
```
----
