#include <fp25519_x64.h>
#include <gtest/gtest.h>
#include "random.h"

#define TEST_TIMES 50000

static void random_EltFp25519_1w_x64(uint64_t *A) {
  random_bytes((uint8_t *)A, SIZE_BYTES_FP25519);
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

    EXPECT_EQ(memcmp(c, e, SIZE_BYTES_FP25519), 0)
        << "got:  " << c << "want: " << e;
    cnt++;
  }
  EXPECT_EQ(cnt, TEST_TIMES)
      << "passed: " << cnt << "/" << TEST_TIMES << std::endl;
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

    EXPECT_EQ(memcmp(a, b, SIZE_BYTES_FP25519), 0)
        << "got:  " << a << "want: " << b;
    cnt++;
  }
  EXPECT_EQ(cnt, TEST_TIMES)
      << "passed: " << cnt << "/" << TEST_TIMES << std::endl;
}
