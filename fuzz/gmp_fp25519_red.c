/**
 * Copyright (c) 2017, Armando Faz <armfazh@ic.unicamp.br>. All rights reserved.
 * Institute of Computing.
 * University of Campinas, Brazil.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *  * Neither the name of University of Campinas nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
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
  if (Size != 2 * 32) return 0;
  EltFp25519_1w_x64 get_c, want_c;
  EltFp25519_1w_Buffer_x64 a;

  mpz_t gmp_a, gmp_low, gmp_high, two_to_256;
  mpz_init(gmp_a);
  mpz_init(gmp_low);
  mpz_init(gmp_high);

  // two_to_256 = 2^256
  mpz_init_set_ui(two_to_256, 1);
  mpz_mul_2exp(two_to_256, two_to_256, 256);

  setzero_EltFp25519_1w_x64(get_c);
  setzero_EltFp25519_1w_x64(want_c);

  memcpy(a, Data + 0, 64);

  red_EltFp25519_1w_x64(get_c, a);

  mpz_import(gmp_a, 2 * SIZE_BYTES_FP25519, -1, sizeof(Data[0]), 0, 0,
             Data + 0);

  while (mpz_cmp(gmp_a, two_to_256) >= 0) {
    mpz_mod_2exp(gmp_low, gmp_a, 256);
    mpz_div_2exp(gmp_high, gmp_a, 256);
    mpz_mul_ui(gmp_high, gmp_high, 38);
    mpz_add(gmp_a, gmp_low, gmp_high);
  }
  mpz_export(want_c, NULL, -1, SIZE_BYTES_FP25519, 0, 0, gmp_a);

  assert(memcmp(get_c, want_c, SIZE_BYTES_FP25519) == 0);

  mpz_clear(gmp_a);
  mpz_clear(gmp_low);
  mpz_clear(gmp_high);
  mpz_clear(two_to_256);
  return 0;
}

#ifdef __cplusplus
}
#endif
