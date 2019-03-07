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


  a[3] = 0x3f47df857ea7659a;
  a[2] = 0x42d6b2fbd1c2b20b;
  a[1] = 0x477aa8802f8cab29;
  a[0] = 0x16286264ce178fd5;

  b[3] = 0xc51ee1569ed169fd;
  b[2] = 0xaf07c7fba6fd3335;
  b[1] = 0x588bfa6b1b4fccf3;
  b[0] = 0x31afe84f2cf91ce0;



  print_bytes(a,32);
  print_bytes(b,32);
 mul_EltFp25519_1w_x64(c, a, b);
 print_bytes(c,32);


  random_EltFp25519_1w_x64(a);
  random_EltFp25519_1w_x64(b);
  random_EltFp25519_1w_x64(c);

  printf("=== GF(2^255-19) ===\n");
  printf("== 1-way x64 \n");
  CLOCKS("add", add_EltFp25519_1w_x64(c, a, b));
  CLOCKS("sub", sub_EltFp25519_1w_x64(c, a, b));
  CLOCKS("mul", mul_EltFp25519_1w_x64(c, c, b));
  CLOCKS("m24", mul_a24_EltFp25519_1w_x64(c, a));
  CLOCKS("sqr", sqr_EltFp25519_1w_x64(c));

  BENCH /= 10;
  CLOCKS("inv", inv_EltFp25519_1w_x64(c, a));
  BENCH *= 10;

  printf("== 2-way x64 \n");
  CLOCKS("mul", mul_EltFp25519_2w_x64(CC, CC, BB));
  CLOCKS("sqr", sqr_EltFp25519_2w_x64(CC));
}
