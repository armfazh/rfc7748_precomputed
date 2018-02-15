/**
 * Copyright (c) 2017 Armando Faz <armfazh@ic.unicamp.br>.
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

    EXPECT_EQ(memcmp(c, e, SIZE_BYTES_FP448), 0)
        << "got:  " << c << "want: " << e;
    cnt++;
  }
  EXPECT_EQ(cnt, TEST_TIMES)
      << "passed: " << cnt << "/" << TEST_TIMES << std::endl;
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

    EXPECT_EQ(memcmp(a, b, SIZE_BYTES_FP448), 0)
        << "got:  " << a << "want: " << b;
    cnt++;
  }
  EXPECT_EQ(cnt, TEST_TIMES)
      << "passed: " << cnt << "/" << TEST_TIMES << std::endl;
}
