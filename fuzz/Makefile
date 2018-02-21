## See:
##    https://llvm.org/docs/LibFuzzer.html
##    https://releases.llvm.org/5.0.0/docs/LibFuzzer.html

FUZZER=./third_party/llvm-build/Release+Asserts/bin/clang -fsanitize=fuzzer,address

RFC7748_PATH=../build
RFC7748_LIB=rfc7748_precomputed
CFLAGS+= -g -O3 -Wall -Wextra -fno-omit-frame-pointer -I../include/

BINARIES=fuzz_hacl_keygen \
         fuzz_hacl_shared \
         fuzz_gmp_fp25519_add \
         fuzz_gmp_fp25519_sub \
         fuzz_gmp_fp25519_red \
         fuzz_gmp_fp25519_a24

all: $(BINARIES)

get_fuzzer:
	mkdir TMP_CLANG
	cd TMP_CLANG; git clone https://chromium.googlesource.com/chromium/src/tools/clang
	./TMP_CLANG/clang/scripts/update.py

fuzz_hacl_%: fuzz_hacl_%.c ./hacl/Hacl_Curve25519.c $(RFC7748_PATH)/lib/lib$(RFC7748_LIB).a
	$(FUZZER) -o $@ $(CFLAGS) $^ -I./hacl

fuzz_gmp_%: fuzz_gmp_%.c $(RFC7748_PATH)/lib/lib$(RFC7748_LIB).a
	$(FUZZER) -o $@ $(CFLAGS) $^ -lgmp

$(RFC7748_PATH)/lib/lib$(RFC7748_LIB).a:
	$(MAKE) -C $(RFC7748_PATH) $(RFC7748_LIB)

.PHONY: $(RFC7748_PATH)/lib/lib$(RFC7748_LIB).a

clean:
	rm -f $(BINARIES)

cleanall:
	rm -rf ./TMP_CLANG ./third_party

