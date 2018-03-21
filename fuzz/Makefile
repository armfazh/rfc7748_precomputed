## See:
##    https://llvm.org/docs/LibFuzzer.html
##    https://releases.llvm.org/5.0.0/docs/LibFuzzer.html

FUZZER=./third_party/llvm-build/Release+Asserts/bin/clang -fsanitize=fuzzer,address

RFC7748_PATH=../build
RFC7748_LIB=rfc7748_precomputed
CFLAGS+= -g -O3 -Wall -Wextra -pedantic -fno-omit-frame-pointer -I../include/

all: hacl gmp

hacl: hacl_keygen hacl_shared
gmp: gmp_fp25519_a24 \
     gmp_fp25519_add \
     gmp_fp25519_sub \
     gmp_fp25519_red \
     gmp_fp25519_mul \
     gmp_fp25519_sqr \
     gmp_fp25519_mul2 \
     gmp_fp25519_sqr2 \
     gmp_fp25519_red2 \
     gmp_fp25519_inv \
     gmp_fp25519_fred


get_fuzzer:
	mkdir TMP_CLANG
	cd TMP_CLANG; git clone https://chromium.googlesource.com/chromium/src/tools/clang
	./TMP_CLANG/clang/scripts/update.py

hacl_%: hacl_%.c ./hacl/Hacl_Curve25519.c $(RFC7748_PATH)/lib/lib$(RFC7748_LIB).a
	$(FUZZER) -o $@ $(CFLAGS) $^ -I./hacl

gmp_%: gmp_%.c $(RFC7748_PATH)/lib/lib$(RFC7748_LIB).a
	$(FUZZER) -o $@ $(CFLAGS) $^ -lgmp

$(RFC7748_PATH)/lib/lib$(RFC7748_LIB).a:
	$(MAKE) -C $(RFC7748_PATH) $(RFC7748_LIB)

.PHONY: $(RFC7748_PATH)/lib/lib$(RFC7748_LIB).a

clean:
	rm -f hacl_keygen hacl_shared \
		gmp_fp25519_a24 \
		gmp_fp25519_add \
		gmp_fp25519_sub \
		gmp_fp25519_red \
		gmp_fp25519_mul \
		gmp_fp25519_sqr \
		gmp_fp25519_mul2 \
		gmp_fp25519_sqr2 \
		gmp_fp25519_inv \
		gmp_fp25519_fred

cleanall:
	rm -rf ./TMP_CLANG ./third_party

