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
#include <stdio.h>
#include "clocks.h"
#include "random.h"

static void random_EltFp448_1w_x64(uint64_t *A) {
  random_bytes((uint8_t *)A, SIZE_BYTES_FP448);
}

void bench_fp448_x64(void) {
  int BENCH = 3000;

  EltFp448_1w_x64 a, b, c;
  EltFp448_1w_Buffer_x64 buffer_1w;

  random_EltFp448_1w_x64(a);
  random_EltFp448_1w_x64(b);
  random_EltFp448_1w_x64(c);

  printf("= GF(2^448-2^224-1) =\n");
  printf("== 1-way x64 \n");
  CLOCKS("add", add_EltFp448_1w_x64(c, a, b));
  CLOCKS("sub", sub_EltFp448_1w_x64(c, a, b));
  CLOCKS("mul", mul_EltFp448_1w_x64(c, c, b));
  CLOCKS("m24", mul_a24_EltFp448_1w_x64(c, a));
  CLOCKS("sqr", sqr_EltFp448_1w_x64(c));

  BENCH /= 10;
  CLOCKS("inv", inv_EltFp448_1w_x64(c, a));
}
