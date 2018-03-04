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
 * Verifies that 0 <= c=a mod p < 2^255-19 for a number 0 <= a < 2^256
 * @param Data Random binary data.
 * @param Size Non-trivial input size is set -max_len=32
 * @return Always return 0 in case of success.
 */
#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  if (Size != 32) return 0;
  EltFp25519_1w_x64 a, get_c, want_c;

  mpz_t gmp_a, gmp_c, prime;
  mpz_init(gmp_a);
  mpz_init(gmp_c);

  // prime = p-2 = 2^255-19
  mpz_init_set_ui(prime, 1);
  mpz_mul_2exp(prime, prime, 255);
  mpz_sub_ui(prime, prime, 19);

  setzero_EltFp25519_1w_x64(get_c);
  setzero_EltFp25519_1w_x64(want_c);

  memcpy(a, Data + 0, 32);

  copy_EltFp25519_1w_x64(get_c, a);
  fred_EltFp25519_1w_x64(get_c);

  mpz_import(gmp_a, SIZE_BYTES_FP25519, -1, sizeof(Data[0]), 0, 0, Data + 0);
  mpz_mod(gmp_c, gmp_a, prime);
  mpz_export(want_c, NULL, -1, SIZE_BYTES_FP25519, 0, 0, gmp_c);

  assert(memcmp(get_c, want_c, SIZE_BYTES_FP25519) == 0);

  mpz_clear(gmp_a);
  mpz_clear(gmp_c);
  mpz_clear(prime);
  return 0;
}

#ifdef __cplusplus
}
#endif
