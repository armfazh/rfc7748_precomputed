## See:
##    https://llvm.org/docs/LibFuzzer.html
##    Requires clang version > 6.0

FUZZER=clang -fsanitize=fuzzer,address

RFC7748_PATH=../build
RFC7748_LIB=rfc7748_precomputed
CFLAGS+=-O3 -Wall -Wextra -pedantic -fno-omit-frame-pointer -I../include/

all: fp hacl

fp: $(patsubst %.c,%,$(wildcard op*.c))
op%: op%.c
	@for fp in 25519 448 ; do \
		echo "fuzz$${fp}_$@"; \
	    $(FUZZER) $^ -DFIELD=$${fp} \
	 	   -o "fuzz$${fp}_$@" $(CFLAGS) \
	 	   -lgmp $(RFC7748_PATH)/lib/lib$(RFC7748_LIB).a; \
    done

hacl: fuzz_hacl_keygen fuzz_hacl_shared
fuzz_hacl_%: hacl_%.c ./hacl/Hacl_Curve25519.o $(RFC7748_PATH)/lib/lib$(RFC7748_LIB).a
	$(FUZZER) -o $@ $(CFLAGS) $^ -I./hacl

$(RFC7748_PATH)/lib/lib$(RFC7748_LIB).a:
	$(MAKE) -C $(RFC7748_PATH) $(RFC7748_LIB)

.PHONY: $(RFC7748_PATH)/lib/lib$(RFC7748_LIB).a

clean:
	rm -f fuzz_hacl_keygen fuzz_hacl_shared fuzz25519_* fuzz448_*
