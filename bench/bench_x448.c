#include <rfc7748_precomputed.h>
#include <stdio.h>
#include "clocks.h"
#include "random.h"

static void random_X448_key(argKey key) {
  random_bytes(key, X448_KEYSIZE_BYTES);
}

void bench_x448(void) {
  X448_KEY secret_key;
  X448_KEY public_key;
  X448_KEY shared_secret;

  printf("===== X448  =====\n");
  oper_second(random_X448_key(secret_key), "KeyGen",
              X448_KeyGen(public_key, secret_key));
  oper_second(random_X448_key(secret_key);
              random_X448_key(public_key), "Shared",
              X448_Shared(shared_secret, public_key, secret_key));
}
