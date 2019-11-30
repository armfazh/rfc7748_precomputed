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
#include <fp448_x64.h>
#include <gmp.h>
#include <string.h>

/**
 * Verifies that 0 <= c=a+b < 2^448 and that c be congruent to a+b mod p
 * @param Data Random binary data.
 * @param Size Non-trivial input size is set -max_len=64
 * @return Always return 0 in case of success.
 */
#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  if (Size != 2 * SIZE_BYTES_FP448)
    return -1;

  EltFp448_1w_x64 a, b, get_c, want_c;

  mpz_t gmp_a, gmp_b, gmp_c, prime, two_to_448;
  mpz_init(gmp_a);
  mpz_init(gmp_b);
  mpz_init(gmp_c);

  // prime = 2^448-2^224-1
  mpz_init_set_ui(prime, 1);
  mpz_mul_2exp(prime, prime, 224);
  mpz_sub_ui(prime, prime, 1);
  mpz_mul_2exp(prime, prime, 224);
  mpz_sub_ui(prime, prime, 1);

  // two_to_448 = 2^448
  mpz_init_set_ui(two_to_448, 1);
  mpz_mul_2exp(two_to_448, two_to_448, 448);

  memcpy(a, Data + 0 * SIZE_BYTES_FP448, SIZE_BYTES_FP448);
  memcpy(b, Data + 1 * SIZE_BYTES_FP448, SIZE_BYTES_FP448);

  setzero_EltFp448_1w_x64(get_c);
  setzero_EltFp448_1w_x64(want_c);

  add_EltFp448_1w_x64(get_c, a, b);

  mpz_import(gmp_a, SIZE_BYTES_FP448, -1, sizeof(Data[0]), 0, 0,
             Data + 0 * SIZE_BYTES_FP448);
  mpz_import(gmp_b, SIZE_BYTES_FP448, -1, sizeof(Data[0]), 0, 0,
             Data + 1 * SIZE_BYTES_FP448);

  mpz_add(gmp_c, gmp_a, gmp_b);
  while (mpz_cmp(gmp_c, two_to_448) >= 0) {
    mpz_sub(gmp_c, gmp_c, prime);
  }
  mpz_export(want_c, NULL, -1, SIZE_BYTES_FP448, 0, 0, gmp_c);
  assert(memcmp(get_c, want_c, SIZE_BYTES_FP448) == 0);

  mpz_clear(gmp_a);
  mpz_clear(gmp_b);
  mpz_clear(gmp_c);
  mpz_clear(prime);
  mpz_clear(two_to_448);
  return 0;
}

#ifdef __cplusplus
}
#endif
