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

#ifndef CLOCKS_H
#define CLOCKS_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __INTEL_COMPILER
#define BARRIER __memory_barrier()
#else
#define BARRIER __asm__ __volatile__("" ::: "memory")
#endif

#define CLOCKS_RANDOM(RANDOM, LABEL, FUNCTION)                           \
  do {                                                                   \
    uint64_t start, end;                                                 \
    int64_t i_bench, j_bench;                                            \
    unsigned cycles_high0, cycles_low0;                                  \
    unsigned cycles_high1, cycles_low1;                                  \
    __asm__ __volatile__(                                                \
        "mfence\n\t"                                                     \
        "RDTSC\n\t"                                                      \
        "mov %%edx, %0\n\t"                                              \
        "mov %%eax, %1\n\t"                                              \
        : "=r"(cycles_high0), "=r"(cycles_low0)::"%rax", "%rbx", "%rcx", \
          "%rdx");                                                       \
    BARRIER;                                                             \
    i_bench = BENCH;                                                     \
    do {                                                                 \
      j_bench = BENCH;                                                   \
      RANDOM;                                                            \
      do {                                                               \
        FUNCTION;                                                        \
        j_bench--;                                                       \
      } while (j_bench != 0);                                            \
      i_bench--;                                                         \
    } while (i_bench != 0);                                              \
    BARRIER;                                                             \
    __asm__ __volatile__(                                                \
        "RDTSCP\n\t"                                                     \
        "mov %%edx, %0\n\t"                                              \
        "mov %%eax, %1\n\t"                                              \
        "mfence\n\t"                                                     \
        : "=r"(cycles_high1), "=r"(cycles_low1)::"%rax", "%rbx", "%rcx", \
          "%rdx");                                                       \
    start = (((uint64_t)cycles_high0) << 32) | cycles_low0;              \
    end = (((uint64_t)cycles_high1) << 32) | cycles_low1;                \
    printf("%-8s: %5lu cc\n", LABEL, (end - start) / (BENCH * BENCH));   \
  } while (0)

#define CLOCKS(LABEL, FUNCTION) CLOCKS_RANDOM(while (0), LABEL, FUNCTION)

#define oper_second(RANDOM, LABEL, FUNCTION)                 \
  do {                                                       \
    printf("%-8s: ", LABEL);                                 \
    RANDOM;                                                  \
                                                             \
    unsigned i;                                              \
    uint64_t start, end;                                     \
    const unsigned iterations = 10000;                       \
    uint64_t start_c, end_c;                                 \
                                                             \
    /* Load the caches*/                                     \
    for (i = 0; i < 500; ++i) {                              \
      FUNCTION;                                              \
    }                                                        \
                                                             \
    start = time_now();                                      \
    start_c = cycles_now();                                  \
    for (i = 0; i < iterations; ++i) {                       \
      FUNCTION;                                              \
    }                                                        \
    end = time_now();                                        \
    end_c = cycles_now();                                    \
                                                             \
    printf("%3lu µs, %8.1f oper/s, %6lu cycles/op\n",        \
           (uint64_t)((end - start) / iterations),      \
           iterations*(double)1e6 / (end - start),           \
           (uint64_t)((end_c - start_c) / iterations)); \
  } while (0)

uint64_t time_now();
uint64_t cycles_now(void);

#endif /* CLOCKS_H */
