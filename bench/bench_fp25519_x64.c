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
#include <stdio.h>
#include "clocks.h"
#include "random.h"

static void random_EltFp25519_1w_x64(uint64_t *A) {
  random_bytes((uint8_t *)A, SIZE_BYTES_FP25519);
}

void bench_fp25519_x64(void) {
  int BENCH = 3000;

  EltFp25519_1w_x64 a, b, c;
  EltFp25519_2w_x64 BB, CC;
  EltFp25519_1w_Buffer_x64 buffer_1w;
  EltFp25519_2w_Buffer_x64 buffer_2w;

  random_EltFp25519_1w_x64(a);
  random_EltFp25519_1w_x64(b);
  random_EltFp25519_1w_x64(c);

  printf("=== GF(2^255-19) ===\n");
  printf("== 1-way x64 \n");
  CLOCKS("add", add_EltFp25519_1w_x64(c, a, b));
  CLOCKS("sub", sub_EltFp25519_1w_x64(c, a, b));
  CLOCKS("mul", mul_EltFp25519_1w_x64(c, c, b));
  CLOCKS("sqr", sqr_256x256_integer_x64(buffer_1w, c));
  CLOCKS("mul", mul_256x256_integer_x64(buffer_1w, c, b));
  CLOCKS("mul", mulx_256x256_integer_x64(buffer_1w, c, b));
  CLOCKS("mul", mulxadx_256x256_integer_x64(buffer_1w, c, b));
  CLOCKS("m24", mul_a24_EltFp25519_1w_x64(c, a));
  CLOCKS("sqr", sqr_EltFp25519_1w_x64(c));

  BENCH /= 10;
  CLOCKS("inv", inv_EltFp25519_1w_x64(c, a));
  BENCH *= 10;

  printf("== 2-way x64 \n");
  CLOCKS("mul", mul_EltFp25519_2w_x64(CC, CC, BB));
  CLOCKS("sqr", sqr_EltFp25519_2w_x64(CC));
}
