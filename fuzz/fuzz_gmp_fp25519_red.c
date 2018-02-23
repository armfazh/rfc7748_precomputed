#include <assert.h>
#include <string.h>

#include <gmp.h>
#include <fp25519_x64.h>

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  if (Size != 2 * 32) return 0;
  EltFp25519_1w_x64 a, b, get_c, want_c;
  EltFp25519_1w_Buffer_x64 buffer_c;

  mpz_t gmp_a, gmp_b, gmp_c, gmp_low, gmp_high, two_to_256;
  mpz_init(gmp_a);
  mpz_init(gmp_b);
  mpz_init(gmp_c);
  mpz_init(gmp_low);
  mpz_init(gmp_high);

  // two_to_256 = 2^256
  mpz_init_set_ui(two_to_256, 1);
  mpz_mul_2exp(two_to_256, two_to_256, 256);

  setzero_EltFp25519_1w_x64(get_c);
  setzero_EltFp25519_1w_x64(want_c);

  memcpy(a, Data + 0, 32);
  memcpy(b, Data + 32, 32);

  mul_256x256_integer_x64(buffer_c, a, b);
  red_EltFp25519_1w_x64(get_c, buffer_c);

  mpz_import(gmp_a, NUM_WORDS_ELTFP25519_X64, -1, sizeof(a[0]), 0, 0, a);
  mpz_import(gmp_b, NUM_WORDS_ELTFP25519_X64, -1, sizeof(b[0]), 0, 0, b);
  mpz_mul(gmp_c, gmp_a, gmp_b);

  while (mpz_cmp(gmp_c, two_to_256) >= 0) {
    mpz_mod_2exp(gmp_low, gmp_c, 256);
    mpz_div_2exp(gmp_high, gmp_c, 256);
    mpz_mul_ui(gmp_high, gmp_high, 38);
    mpz_add(gmp_c, gmp_low, gmp_high);
  }
  mpz_export(want_c, NULL, -1, SIZE_BYTES_FP25519, 0, 0, gmp_c);

  assert(memcmp(get_c, want_c, SIZE_BYTES_FP25519)==0);

  mpz_clear(gmp_a);
  mpz_clear(gmp_b);
  mpz_clear(gmp_c);
  mpz_clear(gmp_low);
  mpz_clear(gmp_high);
  mpz_clear(two_to_256);
  return 0;
}
