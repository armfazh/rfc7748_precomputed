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
 * Verifies that 0 <= c0=a0*b0 < 2^512 and 0 <= c1=a1*b1 < 2^512
 * @param Data Random binary data.
 * @param Size Non-trivial input size is set -max_len=128
 * @return Always return 0 in case of success.
 */
#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  if (Size != 2 * 2 * 32) return 0;

  EltFp25519_2w_x64 a, b;
  EltFp25519_2w_Buffer_x64 get_c, want_c;

  mpz_t gmp_a0, gmp_b0, gmp_c0, gmp_a1, gmp_b1, gmp_c1;
  mpz_init(gmp_a0);
  mpz_init(gmp_a1);
  mpz_init(gmp_b0);
  mpz_init(gmp_b1);
  mpz_init(gmp_c0);
  mpz_init(gmp_c1);

  memcpy(a, Data + 0, 64);
  memcpy(b, Data + 64, 64);

  memset(get_c, 0, sizeof(get_c));
  memset(want_c, 0, sizeof(want_c));

  mul2_256x256_integer_x64(get_c, a, b);

  mpz_import(gmp_a0, SIZE_BYTES_FP25519, -1, sizeof(Data[0]), 0, 0, Data + 0);
  mpz_import(gmp_a1, SIZE_BYTES_FP25519, -1, sizeof(Data[0]), 0, 0, Data + 32);
  mpz_import(gmp_b0, SIZE_BYTES_FP25519, -1, sizeof(Data[0]), 0, 0, Data + 64);
  mpz_import(gmp_b1, SIZE_BYTES_FP25519, -1, sizeof(Data[0]), 0, 0, Data + 96);

  mpz_mul(gmp_c0, gmp_a0, gmp_b0);
  mpz_mul(gmp_c1, gmp_a1, gmp_b1);
  mpz_export(want_c, NULL, -1, 2 * SIZE_BYTES_FP25519, 0, 0, gmp_c0);
  mpz_export(want_c + 2 * NUM_WORDS_ELTFP25519_X64, NULL, -1,
             2 * SIZE_BYTES_FP25519, 0, 0, gmp_c1);
  assert(memcmp(get_c, want_c, sizeof(get_c)) == 0);

  mpz_clear(gmp_a0);
  mpz_clear(gmp_a1);
  mpz_clear(gmp_b0);
  mpz_clear(gmp_b1);
  mpz_clear(gmp_c0);
  mpz_clear(gmp_c1);
  return 0;
}

#ifdef __cplusplus
}
#endif
