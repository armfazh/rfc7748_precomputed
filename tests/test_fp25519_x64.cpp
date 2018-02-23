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
#include <fp25519_x64.h>
#include <gmp.h>
#include <gtest/gtest.h>
#include "random.h"

#define TEST_TIMES 50000

static void random_EltFp25519_1w_x64(uint64_t *A) {
  random_bytes(reinterpret_cast<uint8_t *>(A), SIZE_BYTES_FP25519);
}

static std::ostream &operator<<(std::ostream &os,
                                const EltFp25519_1w_x64 &number) {
  os << "0x";
  for (int i = NUM_WORDS_ELTFP25519_X64 - 1; i >= 0; i--) {
    os << std::setbase(16) << std::setfill('0') << std::setw(16) << number[i];
  }
  return os << std::endl;
}

/* Verifies that (a^2-b^2) == (a+b)(a-b) */
TEST(FP25519, MUL_VS_SQR) {
  int64_t i;
  int64_t cnt = 0;
  EltFp25519_1w_x64 a, b, c, e, f;
  EltFp25519_1w_Buffer_x64 buffer_1w;

  for (i = 0; i < TEST_TIMES; i++) {
    random_EltFp25519_1w_x64(a);
    random_EltFp25519_1w_x64(b);

    add_EltFp25519_1w_x64(e, a, b);
    sub_EltFp25519_1w_x64(f, a, b);
    mul_EltFp25519_1w_x64(e, e, f);

    sqr_EltFp25519_1w_x64(a);
    sqr_EltFp25519_1w_x64(b);
    sub_EltFp25519_1w_x64(c, a, b);

    fred_EltFp25519_1w_x64(c);
    fred_EltFp25519_1w_x64(e);

    ASSERT_EQ(memcmp(c, e, SIZE_BYTES_FP25519), 0) << "got:  " << c
                                                   << "want: " << e;
    cnt++;
  }
  EXPECT_EQ(cnt, TEST_TIMES) << "passed: " << cnt << "/" << TEST_TIMES
                             << std::endl;
}

/* Verifies that (a*b*a^1) == (b) */
TEST(FP25519, MUL_VS_INV) {
  int64_t i;
  int64_t cnt = 0;
  EltFp25519_1w_x64 a, b, d;
  EltFp25519_1w_Buffer_x64 buffer_1w;

  for (i = 0; i < TEST_TIMES; i++) {
    random_EltFp25519_1w_x64(a);
    random_EltFp25519_1w_x64(b);

    inv_EltFp25519_1w_x64(d, a);
    mul_EltFp25519_1w_x64(a, a, b);
    mul_EltFp25519_1w_x64(a, a, d);
    fred_EltFp25519_1w_x64(a);
    fred_EltFp25519_1w_x64(b);

    ASSERT_EQ(memcmp(a, b, SIZE_BYTES_FP25519), 0) << "got:  " << a
                                                   << "want: " << b;
    cnt++;
  }
  EXPECT_EQ(cnt, TEST_TIMES) << "passed: " << cnt << "/" << TEST_TIMES
                             << std::endl;
}

/* Verifies that 0 <= c=a+b < 2^256 and that c be congruent to a+b mod p */
TEST(FP25519, ADDITION) {
  int count = 0;
  EltFp25519_1w_x64 a, b, get_c, want_c;

  mpz_t gmp_a, gmp_b, gmp_c, two_prime, two_to_256;
  mpz_init(gmp_a);
  mpz_init(gmp_b);
  mpz_init(gmp_c);

  // two_prime = 2^256-38
  mpz_init_set_ui(two_prime, 1);
  mpz_mul_2exp(two_prime, two_prime, 256);
  mpz_sub_ui(two_prime, two_prime, 38);

  // two_to_256 = 2^256
  mpz_init_set_ui(two_to_256, 1);
  mpz_mul_2exp(two_to_256, two_to_256, 256);

  for (int i = 0; i < TEST_TIMES; i++) {
    setzero_EltFp25519_1w_x64(get_c);
    setzero_EltFp25519_1w_x64(want_c);

    random_EltFp25519_1w_x64(a);
    random_EltFp25519_1w_x64(b);

    add_EltFp25519_1w_x64(get_c, a, b);

    mpz_import(gmp_a, NUM_WORDS_ELTFP25519_X64, -1, sizeof(a[0]), 0, 0, a);
    mpz_import(gmp_b, NUM_WORDS_ELTFP25519_X64, -1, sizeof(b[0]), 0, 0, b);

    mpz_add(gmp_c, gmp_a, gmp_b);
    while (mpz_cmp(gmp_c, two_to_256) >= 0) {
      mpz_sub(gmp_c, gmp_c, two_prime);
    }
    mpz_export(want_c, NULL, -1, SIZE_BYTES_FP25519, 0, 0, gmp_c);

    ASSERT_EQ(memcmp(get_c, want_c, SIZE_BYTES_FP25519), 0)
        << "a: " << a << "b: " << b << "got:  " << get_c << "want: " << want_c;
    count++;
  }
  EXPECT_EQ(count, TEST_TIMES) << "passed: " << count << "/" << TEST_TIMES
                               << std::endl;

  mpz_clear(gmp_a);
  mpz_clear(gmp_b);
  mpz_clear(gmp_c);
  mpz_clear(two_prime);
  mpz_clear(two_to_256);
}

/* Verifies that 0 <= c=a-b < 2^256 and that c be congruent to a-b mod p */
TEST(FP25519, SUBTRACTION) {
  int count = 0;
  EltFp25519_1w_x64 a, b, get_c, want_c;

  mpz_t gmp_a, gmp_b, gmp_c, two_prime, zero;
  mpz_init(gmp_a);
  mpz_init(gmp_b);
  mpz_init(gmp_c);

  // two_prime = 2^256-38
  mpz_init_set_ui(two_prime, 1);
  mpz_mul_2exp(two_prime, two_prime, 256);
  mpz_sub_ui(two_prime, two_prime, 38);

  // zero = 0
  mpz_init_set_ui(zero, 0);

  for (int i = 0; i < TEST_TIMES; i++) {
    setzero_EltFp25519_1w_x64(get_c);
    setzero_EltFp25519_1w_x64(want_c);

    random_EltFp25519_1w_x64(a);
    random_EltFp25519_1w_x64(b);

    sub_EltFp25519_1w_x64(get_c, a, b);

    mpz_import(gmp_a, NUM_WORDS_ELTFP25519_X64, -1, sizeof(a[0]), 0, 0, a);
    mpz_import(gmp_b, NUM_WORDS_ELTFP25519_X64, -1, sizeof(b[0]), 0, 0, b);

    mpz_sub(gmp_c, gmp_a, gmp_b);
    while (mpz_cmp(gmp_c, zero) < 0) {
      mpz_add(gmp_c, gmp_c, two_prime);
    }
    mpz_export(want_c, NULL, -1, SIZE_BYTES_FP25519, 0, 0, gmp_c);

    ASSERT_EQ(memcmp(get_c, want_c, SIZE_BYTES_FP25519), 0)
        << "a: " << a << "b: " << b << "got:  " << get_c << "want: " << want_c;
    count++;
  }
  EXPECT_EQ(count, TEST_TIMES) << "passed: " << count << "/" << TEST_TIMES
                               << std::endl;

  mpz_clear(gmp_a);
  mpz_clear(gmp_b);
  mpz_clear(gmp_c);
  mpz_clear(two_prime);
  mpz_clear(zero);
}

/* Verifies that 0 <= c=a*b < 2^512 */
TEST(FP25519, MULTIPLICATION) {
  int count = 0;
  EltFp25519_1w_x64 a, b;
  EltFp25519_1w_Buffer_x64 get_c, want_c;

  mpz_t gmp_a, gmp_b, gmp_c;
  mpz_init(gmp_a);
  mpz_init(gmp_b);
  mpz_init(gmp_c);

  for (int i = 0; i < TEST_TIMES; i++) {
    setzero_EltFp25519_1w_x64(get_c);
    setzero_EltFp25519_1w_x64(get_c + NUM_WORDS_ELTFP25519_X64);
    setzero_EltFp25519_1w_x64(want_c);
    setzero_EltFp25519_1w_x64(want_c + NUM_WORDS_ELTFP25519_X64);

    random_EltFp25519_1w_x64(a);
    random_EltFp25519_1w_x64(b);

    mul_256x256_integer_x64(get_c, a, b);

    mpz_import(gmp_a, NUM_WORDS_ELTFP25519_X64, -1, sizeof(a[0]), 0, 0, a);
    mpz_import(gmp_b, NUM_WORDS_ELTFP25519_X64, -1, sizeof(b[0]), 0, 0, b);

    mpz_mul(gmp_c, gmp_a, gmp_b);
    mpz_export(want_c, NULL, -1, 2 * SIZE_BYTES_FP25519, 0, 0, gmp_c);

    ASSERT_EQ(memcmp(get_c, want_c, 2 * SIZE_BYTES_FP25519), 0)
        << "a: " << a << "b: " << b << "got:  " << get_c << "want: " << want_c;
    count++;
  }
  EXPECT_EQ(count, TEST_TIMES) << "passed: " << count << "/" << TEST_TIMES
                               << std::endl;

  mpz_clear(gmp_a);
  mpz_clear(gmp_b);
  mpz_clear(gmp_c);
}

/* Verifies that 0 <= c=a*a < 2^512 */
TEST(FP25519, SQUARING) {
  int count = 0;
  EltFp25519_1w_x64 a;
  EltFp25519_1w_Buffer_x64 get_c, want_c;

  mpz_t gmp_a, gmp_c;
  mpz_init(gmp_a);
  mpz_init(gmp_c);

  for (int i = 0; i < TEST_TIMES; i++) {
    setzero_EltFp25519_1w_x64(get_c);
    setzero_EltFp25519_1w_x64(get_c + NUM_WORDS_ELTFP25519_X64);
    setzero_EltFp25519_1w_x64(want_c);
    setzero_EltFp25519_1w_x64(want_c + NUM_WORDS_ELTFP25519_X64);

    random_EltFp25519_1w_x64(a);

    sqr_256x256_integer_x64(get_c, a);

    mpz_import(gmp_a, NUM_WORDS_ELTFP25519_X64, -1, sizeof(a[0]), 0, 0, a);

    mpz_mul(gmp_c, gmp_a, gmp_a);
    mpz_export(want_c, NULL, -1, 2 * SIZE_BYTES_FP25519, 0, 0, gmp_c);

    ASSERT_EQ(memcmp(get_c, want_c, 2 * SIZE_BYTES_FP25519), 0)
        << "a: " << a << "got:  " << get_c << "want: " << want_c;
    count++;
  }
  EXPECT_EQ(count, TEST_TIMES) << "passed: " << count << "/" << TEST_TIMES
                               << std::endl;

  mpz_clear(gmp_a);
  mpz_clear(gmp_c);
}

/* Verifies that 0 <= c=a^-1 < 2^256 and that c be congruent to a^-1 mod p */
TEST(FP25519, INVERSION) {
  int count = 0;
  EltFp25519_1w_x64 a, get_c, want_c;

  mpz_t gmp_a, gmp_c, two_prime, prime_minus_two;
  mpz_init(gmp_a);
  mpz_init(gmp_c);

  // two_prime = 2^256-38
  mpz_init_set_ui(two_prime, 1);
  mpz_mul_2exp(two_prime, two_prime, 256);
  mpz_sub_ui(two_prime, two_prime, 38);

  // expo = p-2 = 2^255-19-2 = 2^255-21
  mpz_init_set_ui(prime_minus_two, 1);
  mpz_mul_2exp(prime_minus_two, prime_minus_two, 255);
  mpz_sub_ui(prime_minus_two, prime_minus_two, 21);

  for (int i = 0; i < TEST_TIMES; i++) {
    setzero_EltFp25519_1w_x64(get_c);
    setzero_EltFp25519_1w_x64(want_c);

    random_EltFp25519_1w_x64(a);

    inv_EltFp25519_1w_x64(get_c, a);

    mpz_import(gmp_a, NUM_WORDS_ELTFP25519_X64, -1, sizeof(a[0]), 0, 0, a);
    mpz_powm(gmp_c, gmp_a, prime_minus_two, two_prime);
    mpz_export(want_c, NULL, -1, SIZE_BYTES_FP25519, 0, 0, gmp_c);

    ASSERT_EQ(memcmp(get_c, want_c, SIZE_BYTES_FP25519), 0)
        << "a: " << a << "got:  " << get_c << "want: " << want_c;
    count++;
  }
  EXPECT_EQ(count, TEST_TIMES) << "passed: " << count << "/" << TEST_TIMES
                               << std::endl;

  mpz_clear(gmp_a);
  mpz_clear(gmp_c);
  mpz_clear(two_prime);
  mpz_clear(prime_minus_two);
}

/* Verifies that 0<= c=a*b < 2^256 and that c be congruent to a*b mod p */
TEST(FP25519, REDUCTION) {
  int count = 0;
  EltFp25519_1w_x64 a, b, get_c, want_c;
  EltFp25519_1w_Buffer_x64 buffer_c;

  mpz_t gmp_a, gmp_b, gmp_c, gmp_low, gmp_high, two_to_256;
  mpz_init(gmp_a);
  mpz_init(gmp_b);
  mpz_init(gmp_c);
  mpz_init(gmp_low);
  mpz_init(gmp_high);

  // two_to_256 = 2^256
  mpz_init_set_ui(two_to_256, 1);
  mpz_mul_2exp(two_to_256, two_to_256, 256);

  for (int i = 0; i < TEST_TIMES; i++) {
    setzero_EltFp25519_1w_x64(get_c);
    setzero_EltFp25519_1w_x64(want_c);

    random_EltFp25519_1w_x64(a);
    random_EltFp25519_1w_x64(b);

    mul_256x256_integer_x64(buffer_c, a, b);
    red_EltFp25519_1w_x64(get_c, buffer_c);

    mpz_import(gmp_a, NUM_WORDS_ELTFP25519_X64, -1, sizeof(a[0]), 0, 0, a);
    mpz_import(gmp_b, NUM_WORDS_ELTFP25519_X64, -1, sizeof(b[0]), 0, 0, b);
    mpz_mul(gmp_c, gmp_a, gmp_b);

    while (mpz_cmp(gmp_c, two_to_256) >= 0) {
      mpz_mod_2exp(gmp_low, gmp_c, 256);
      mpz_div_2exp(gmp_high, gmp_c, 256);
      mpz_mul_ui(gmp_high, gmp_high, 38);
      mpz_add(gmp_c, gmp_low, gmp_high);
    }
    mpz_export(want_c, NULL, -1, SIZE_BYTES_FP25519, 0, 0, gmp_c);

    ASSERT_EQ(memcmp(get_c, want_c, SIZE_BYTES_FP25519), 0)
        << "a: " << a << "b: " << b << "got:  " << get_c << "want: " << want_c;
    count++;
  }
  EXPECT_EQ(count, TEST_TIMES) << "passed: " << count << "/" << TEST_TIMES
                               << std::endl;

  mpz_clear(gmp_a);
  mpz_clear(gmp_b);
  mpz_clear(gmp_c);
  mpz_clear(gmp_low);
  mpz_clear(gmp_high);
  mpz_clear(two_to_256);
}

/* Verifies that 0<= c=a24*a < 2^256 and that c be congruent to a24*a mod p */
TEST(FP25519, MULA24) {
  int count = 0;
  const uint64_t a24 = 121666;
  EltFp25519_1w_x64 a, get_c, want_c;

  mpz_t gmp_a, gmp_c, gmp_low, gmp_high, two_to_256;
  mpz_init(gmp_a);
  mpz_init(gmp_c);
  mpz_init(gmp_low);
  mpz_init(gmp_high);

  // two_to_256 = 2^256
  mpz_init_set_ui(two_to_256, 1);
  mpz_mul_2exp(two_to_256, two_to_256, 256);

  for (int i = 0; i < TEST_TIMES; i++) {
    setzero_EltFp25519_1w_x64(get_c);
    setzero_EltFp25519_1w_x64(want_c);

    random_EltFp25519_1w_x64(a);
    mul_a24_EltFp25519_1w_x64(get_c, a);

    mpz_import(gmp_a, NUM_WORDS_ELTFP25519_X64, -1, sizeof(a[0]), 0, 0, a);
    mpz_mul_ui(gmp_c, gmp_a, a24);

    while (mpz_cmp(gmp_c, two_to_256) >= 0) {
      mpz_mod_2exp(gmp_low, gmp_c, 256);
      mpz_div_2exp(gmp_high, gmp_c, 256);
      mpz_mul_ui(gmp_high, gmp_high, 38);
      mpz_add(gmp_c, gmp_low, gmp_high);
    }
    mpz_export(want_c, NULL, -1, SIZE_BYTES_FP25519, 0, 0, gmp_c);

    ASSERT_EQ(memcmp(get_c, want_c, SIZE_BYTES_FP25519), 0)
                  << "a: " << a << "got:  " << get_c << "want: " << want_c;
    count++;
  }
  EXPECT_EQ(count, TEST_TIMES) << "passed: " << count << "/" << TEST_TIMES
                               << std::endl;

  mpz_clear(gmp_a);
  mpz_clear(gmp_c);
  mpz_clear(gmp_low);
  mpz_clear(gmp_high);
  mpz_clear(two_to_256);
}

/* Verifies that 0 <= c=a mod p < 2^255-19 for a number 0 <= a < 2^256 */
TEST(FP25519, FREDUCTION) {
  int count = 0;
  EltFp25519_1w_x64 a, get_c, want_c;

  mpz_t gmp_a, gmp_c, prime;
  mpz_init(gmp_a);
  mpz_init(gmp_c);

  // prime = p-2 = 2^255-19
  mpz_init_set_ui(prime, 1);
  mpz_mul_2exp(prime, prime, 255);
  mpz_sub_ui(prime, prime, 19);

  for (int i = 0; i < TEST_TIMES; i++) {
    setzero_EltFp25519_1w_x64(get_c);
    setzero_EltFp25519_1w_x64(want_c);

    random_EltFp25519_1w_x64(a);
    copy_EltFp25519_1w_x64(get_c,a);
    mpz_import(gmp_a, NUM_WORDS_ELTFP25519_X64, -1, sizeof(a[0]), 0, 0, a);

    fred_EltFp25519_1w_x64(get_c);
    mpz_mod(gmp_c, gmp_a, prime);

    mpz_export(want_c, NULL, -1, SIZE_BYTES_FP25519, 0, 0, gmp_c);

    ASSERT_EQ(memcmp(get_c, want_c, SIZE_BYTES_FP25519), 0)
                  << "a: " << a << "got:  " << get_c << "want: " << want_c;
    count++;
  }
  EXPECT_EQ(count, TEST_TIMES) << "passed: " << count << "/" << TEST_TIMES
                               << std::endl;

  mpz_clear(gmp_a);
  mpz_clear(gmp_c);
  mpz_clear(prime);
}
