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

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>
#include <gmp.h>
#include <string.h>

#define getTwoPowerK(X, K) \
  mpz_init_set_ui(X, 1);   \
  mpz_mul_2exp(X, X, K);

#if FIELD == 25519

#include <fp25519_x64.h>
#define N SIZE_BYTES_FP25519
#define NUM_WORDS NUM_WORDS_ELTFP25519_X64
#define K 256
#define A24 121666
#define TYPE EltFp25519_1w_x64
#define TYPE_BUFFER EltFp25519_1w_Buffer_x64
#define getPrime(X)        \
  mpz_init_set_ui(X, 1);   \
  mpz_mul_2exp(X, X, 255); \
  mpz_sub_ui(X, X, 19);
#define getTwoPrime(X)     \
  mpz_init_set_ui(X, 1);   \
  mpz_mul_2exp(X, X, 256); \
  mpz_sub_ui(X, X, 38);
#define getTwoPower(X) getTwoPowerK(X, K)
#define getPModTwoK(X) mpz_init_set_ui(X, 38);
#define getPminus2(X)      \
  mpz_init_set_ui(X, 1);   \
  mpz_mul_2exp(X, X, 255); \
  mpz_sub_ui(X, X, 21);

#define FpZero setzero_EltFp25519_1w_x64
#define FpAdd add_EltFp25519_1w_x64
#define FpSub sub_EltFp25519_1w_x64
#define FpMul mul_EltFp25519_1w_x64
#define FpSqr sqr_EltFp25519_1w_x64
#define FpRed red_EltFp25519_1w_x64
#define FpInv inv_EltFp25519_1w_x64
#define FpMod fred_EltFp25519_1w_x64
#define FpMulA24 mul_a24_EltFp25519_1w_x64

#elif FIELD == 448

#include <fp448_x64.h>
#define N SIZE_BYTES_FP448
#define NUM_WORDS NUM_WORDS_ELTFP448_X64
#define K 448
#define A24 39082
#define TYPE EltFp448_1w_x64
#define TYPE_BUFFER EltFp448_1w_Buffer_x64
#define getPrime(X)        \
  mpz_init_set_ui(X, 1);   \
  mpz_mul_2exp(X, X, 224); \
  mpz_sub_ui(X, X, 1);     \
  mpz_mul_2exp(X, X, 224); \
  mpz_sub_ui(X, X, 1);
#define getTwoPrime(X) getPrime(X)
#define getTwoPower(X) getTwoPowerK(X, K)
#define getPModTwoK(X)     \
  mpz_init_set_ui(X, 1);   \
  mpz_mul_2exp(X, X, 224); \
  mpz_add_ui(X, X, 1);
#define getPminus2(X)      \
  mpz_init_set_ui(X, 1);   \
  mpz_mul_2exp(X, X, 224); \
  mpz_sub_ui(X, X, 1);     \
  mpz_mul_2exp(X, X, 224); \
  mpz_sub_ui(X, X, 3);
#define FpZero setzero_EltFp448_1w_x64
#define FpAdd add_EltFp448_1w_x64
#define FpSub sub_EltFp448_1w_x64
#define FpMul mul_EltFp448_1w_x64
#define FpSqr sqr_EltFp448_1w_x64
#define FpRed red_EltFp448_1w_x64
#define FpInv inv_EltFp448_1w_x64
#define FpMod fred_EltFp448_1w_x64
#define FpMulA24 mul_a24_EltFp448_1w_x64

#endif

#ifdef __cplusplus
}
#endif
