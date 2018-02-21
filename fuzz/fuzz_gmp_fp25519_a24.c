#include <assert.h>
#include <string.h>

#include <gmp.h>
#include <fp25519_x64.h>

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  if (Size != 32) return 0;
  const uint64_t a24 = 121666;
  EltFp25519_1w_x64 a, get_c, want_c;

  mpz_t gmp_a, gmp_c, gmp_low, gmp_high, two_to_256;
  mpz_init(gmp_a);
  mpz_init(gmp_c);
  mpz_init(gmp_low);
  mpz_init(gmp_high);

  // two_to_256 = 2^256
  mpz_init_set_ui(two_to_256, 1);
  mpz_mul_2exp(two_to_256, two_to_256, 256);

  setzero_EltFp25519_1w_x64(get_c);
  setzero_EltFp25519_1w_x64(want_c);

  memcpy(a, Data + 0, 32);

  mul_a24_EltFp25519_1w_x64(get_c, a);

  mpz_import(gmp_a, NUM_WORDS_ELTFP25519_X64, -1, sizeof(a[0]), 0, 0, a);
  mpz_mul_ui(gmp_c, gmp_a, a24);

//  int i = 0;
  while (mpz_cmp(gmp_c, two_to_256) >= 0) {
    mpz_mod_2exp(gmp_low, gmp_c, 256);
    mpz_div_2exp(gmp_high, gmp_c, 256);
    mpz_mul_ui(gmp_high, gmp_high, 38);
    mpz_add(gmp_c, gmp_low, gmp_high);
//    printf("Red #%d: ",i++);mpz_out_str(stdout, 16,gmp_c);printf("\n");
  }
  mpz_export(want_c, NULL, -1, SIZE_BYTES_FP25519, 0, 0, gmp_c);

  assert(memcmp(get_c, want_c, SIZE_BYTES_FP25519) == 0);

  mpz_clear(gmp_a);
  mpz_clear(gmp_c);
  mpz_clear(gmp_low);
  mpz_clear(gmp_high);
  mpz_clear(two_to_256);
  return 0;
}
