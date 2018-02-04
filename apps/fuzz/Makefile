
## See: 
##    https://llvm.org/docs/LibFuzzer.html
##    https://releases.llvm.org/5.0.0/docs/LibFuzzer.html

FUZZER=/home/armfazh/third_party/llvm-build/Release+Asserts/bin/clang -fsanitize=fuzzer 

CODE_A= -I./hacl ./hacl/Hacl_Curve25519.c 
CODE_B=	-I../../include/ ../../build/lib/librfc7748_precomputed.a

all: fuzzer

fuzzer:
	$(FUZZER) -o $@ -g -O3 fuzz.c $(CODE_A) $(CODE_B) 

clean:
	rm -f fuzzer


