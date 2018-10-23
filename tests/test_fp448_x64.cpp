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

#include <fp448_x64.h>
#include <gtest/gtest.h>
#include "random.h"

#define TEST_TIMES 50000

static void random_EltFp448_1w_x64(uint64_t *A) {
  random_bytes(reinterpret_cast<uint8_t *>(A), SIZE_BYTES_FP448);
}

// Verifies that (a^2-b^2) == (a+b)(a-b)
TEST(FP448, MUL_VS_SQR) {
  int64_t i;
  int64_t cnt = 0;
  EltFp448_1w_x64 a, b, c, e, f;
  EltFp448_1w_Buffer_x64 buffer_1w;

  for (i = 0; i < TEST_TIMES; i++) {
    random_EltFp448_1w_x64(a);
    random_EltFp448_1w_x64(b);

    add_EltFp448_1w_x64(e, a, b);
    sub_EltFp448_1w_x64(f, a, b);
    mul_EltFp448_1w_x64(e, e, f);

    sqr_EltFp448_1w_x64(a);
    sqr_EltFp448_1w_x64(b);
    sub_EltFp448_1w_x64(c, a, b);

    fred_EltFp448_1w_x64(c);
    fred_EltFp448_1w_x64(e);

    EXPECT_EQ(memcmp(c, e, SIZE_BYTES_FP448), 0) << "got:  " << c
                                                 << "want: " << e;
    cnt++;
  }
  EXPECT_EQ(cnt, TEST_TIMES) << "passed: " << cnt << "/" << TEST_TIMES
                             << std::endl;
}

// Verifies that (a*b*a^1) == (b)
TEST(FP448, MUL_VS_INV) {
  int64_t i;
  int64_t cnt = 0;
  EltFp448_1w_x64 a, b, d;
  EltFp448_1w_Buffer_x64 buffer_1w;

  for (i = 0; i < TEST_TIMES; i++) {
    random_EltFp448_1w_x64(a);
    random_EltFp448_1w_x64(b);

    inv_EltFp448_1w_x64(d, a);
    mul_EltFp448_1w_x64(a, a, b);
    mul_EltFp448_1w_x64(a, a, d);
    fred_EltFp448_1w_x64(a);
    fred_EltFp448_1w_x64(b);

    EXPECT_EQ(memcmp(a, b, SIZE_BYTES_FP448), 0) << "got:  " << a
                                                 << "want: " << b;
    cnt++;
  }
  EXPECT_EQ(cnt, TEST_TIMES) << "passed: " << cnt << "/" << TEST_TIMES
                             << std::endl;
}
