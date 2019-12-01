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

#include "fp.h"

/**
 * Verifies that 0 <= c=a+b < 2^K and that c be congruent to a+b mod p
 * @param Data Random binary data.
 * @param Size Non-trivial input size is set -max_len=2*N
 * @return Always return 0 in case of success.
 */
#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput(const uint8_t* Data, size_t Size) {
  if (Size != 2 * N) return 0;
  TYPE a, b, get_c, want_c;

  mpz_t gmp_a, gmp_b, gmp_c, two_prime, two_to_K;
  mpz_init(gmp_a);
  mpz_init(gmp_b);
  mpz_init(gmp_c);
  getTwoPrime(two_prime);  // two_prime = 2p
  getTwoPower(two_to_K);   // two_to_K = 2^K

  memcpy(a, Data + 0 * N, N);
  memcpy(b, Data + 1 * N, N);

  FpZero(get_c);
  FpZero(want_c);

  FpAdd(get_c, a, b);

  mpz_import(gmp_a, N, -1, sizeof(Data[0]), 0, 0, Data + 0 * N);
  mpz_import(gmp_b, N, -1, sizeof(Data[0]), 0, 0, Data + 1 * N);

  mpz_add(gmp_c, gmp_a, gmp_b);
  while (mpz_cmp(gmp_c, two_to_K) >= 0) {
    mpz_sub(gmp_c, gmp_c, two_prime);
  }
  mpz_export(want_c, NULL, -1, N, 0, 0, gmp_c);
  assert(memcmp(get_c, want_c, N) == 0);

  mpz_clear(gmp_a);
  mpz_clear(gmp_b);
  mpz_clear(gmp_c);
  mpz_clear(two_prime);
  mpz_clear(two_to_K);
  return 0;
}

#ifdef __cplusplus
}
#endif
