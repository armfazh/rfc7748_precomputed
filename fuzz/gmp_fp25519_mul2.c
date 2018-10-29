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
