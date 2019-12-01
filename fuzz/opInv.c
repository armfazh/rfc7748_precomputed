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
 * Verifies that 0 <= a < 2^K and that c be congruent to a^-1 mod p
 * @param Data Random binary data.
 * @param Size Non-trivial input size is set -max_len=N
 * @return Always return 0 in case of success.
 */
#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  if (Size != N) return 0;

  TYPE a, get_c, want_c;

  mpz_t gmp_a, gmp_c, prime, prime_minus_two;
  mpz_init(gmp_a);
  mpz_init(gmp_c);
  getPrime(prime);
  getPminus2(prime_minus_two);

  memcpy(a, Data + 0, N);

  FpZero(get_c);
  FpZero(want_c);

  FpInv(get_c, a);
  FpMod(get_c);

  mpz_import(gmp_a, N, -1, sizeof(Data[0]), 0, 0, Data + 0);
  mpz_powm(gmp_c, gmp_a, prime_minus_two, prime);
  mpz_export(want_c, NULL, -1, N, 0, 0, gmp_c);
  assert(memcmp(get_c, want_c, N) == 0);

  mpz_clear(gmp_a);
  mpz_clear(gmp_c);
  mpz_clear(prime);
  mpz_clear(prime_minus_two);
  return 0;
}

#ifdef __cplusplus
}
#endif
