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

#include "benchmark/benchmark.h"
#include "random.h"
#include <rfc7748_precomputed.h>

static void BM_X25519_KeyGen(benchmark::State &state) {
  X25519_KEY secret_key;
  X25519_KEY public_key;
  random_bytes(secret_key, X25519_KEYSIZE_BYTES);
  for (auto _ : state) {
    X25519_KeyGen(public_key, secret_key);
  }
}

static void BM_X25519_Shared(benchmark::State &state) {
  X25519_KEY secret_key;
  X25519_KEY public_key;
  X25519_KEY shared_key;
  random_bytes(secret_key, X25519_KEYSIZE_BYTES);
  random_bytes(public_key, X25519_KEYSIZE_BYTES);
  for (auto _ : state) {
    X25519_Shared(shared_key, public_key, secret_key);
  }
}

static void BM_X448_KeyGen(benchmark::State &state) {
  X448_KEY secret_key;
  X448_KEY public_key;
  random_bytes(secret_key, X448_KEYSIZE_BYTES);
  for (auto _ : state) {
    X448_KeyGen(public_key, secret_key);
  }
}

static void BM_X448_Shared(benchmark::State &state) {
  X448_KEY secret_key;
  X448_KEY public_key;
  X448_KEY shared_key;
  random_bytes(secret_key, X448_KEYSIZE_BYTES);
  random_bytes(public_key, X448_KEYSIZE_BYTES);
  for (auto _ : state) {
    X448_Shared(shared_key, public_key, secret_key);
  }
}

BENCHMARK(BM_X25519_KeyGen)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_X25519_Shared)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_X448_KeyGen)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_X448_Shared)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
