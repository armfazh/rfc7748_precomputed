/**
 * Copyright (c) 2017 Armando Faz <armfazh@ic.unicamp.br>. All Rights Reserved.
 * Institute of Computing.
 * University of Campinas, Brazil.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 2 or greater.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include <assert.h>
#include <fp25519_x64.h>
#include <gmp.h>
#include <string.h>

/**
 * Verifies that 0 <= c < 2^256 be congruent (mod p) to 0<= a < 2^512
 * @param Data Random binary data.
 * @param Size Non-trivial input size is set -max_len=64
 * @return Always return 0 in case of success.
 */
#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  if (Size != 2 * 2 * 32) return 0;

  EltFp25519_2w_x64 get_c, want_c;
  EltFp25519_2w_Buffer_x64 a;

  mpz_t gmp_a0, gmp_a1, gmp_low, gmp_high, two_to_256;
  mpz_init(gmp_a0);
  mpz_init(gmp_a1);
  mpz_init(gmp_low);
  mpz_init(gmp_high);

  // two_to_256 = 2^256
  mpz_init_set_ui(two_to_256, 1);
  mpz_mul_2exp(two_to_256, two_to_256, 256);

  setzero_EltFp25519_1w_x64(get_c + 0);
  setzero_EltFp25519_1w_x64(get_c + NUM_WORDS_ELTFP25519_X64);
  setzero_EltFp25519_1w_x64(want_c + 0);
  setzero_EltFp25519_1w_x64(want_c + NUM_WORDS_ELTFP25519_X64);

  memcpy(a, Data + 0, 128);

  red_EltFp25519_2w_x64(get_c, a);

  mpz_import(gmp_a0, 2 * SIZE_BYTES_FP25519, -1, sizeof(Data[0]), 0, 0,
             Data + 0);
  mpz_import(gmp_a1, 2 * SIZE_BYTES_FP25519, -1, sizeof(Data[0]), 0, 0,
             Data + 64);

  while (mpz_cmp(gmp_a0, two_to_256) >= 0) {
    mpz_mod_2exp(gmp_low, gmp_a0, 256);
    mpz_div_2exp(gmp_high, gmp_a0, 256);
    mpz_mul_ui(gmp_high, gmp_high, 38);
    mpz_add(gmp_a0, gmp_low, gmp_high);
  }
  mpz_export(want_c, NULL, -1, SIZE_BYTES_FP25519, 0, 0, gmp_a0);

  while (mpz_cmp(gmp_a1, two_to_256) >= 0) {
    mpz_mod_2exp(gmp_low, gmp_a1, 256);
    mpz_div_2exp(gmp_high, gmp_a1, 256);
    mpz_mul_ui(gmp_high, gmp_high, 38);
    mpz_add(gmp_a1, gmp_low, gmp_high);
  }
  mpz_export(want_c + NUM_WORDS_ELTFP25519_X64, NULL, -1, SIZE_BYTES_FP25519, 0,
             0, gmp_a1);

  assert(memcmp(get_c, want_c, sizeof(get_c)) == 0);

  mpz_clear(gmp_a0);
  mpz_clear(gmp_a1);
  mpz_clear(gmp_low);
  mpz_clear(gmp_high);
  mpz_clear(two_to_256);
  return 0;
}

#ifdef __cplusplus
}
#endif
