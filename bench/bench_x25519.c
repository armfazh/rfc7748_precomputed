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
#include <rfc7748_precomputed.h>
#include "clocks.h"
#include "random.h"

static void random_X25519_key(argKey key) {
  random_bytes(key, X25519_KEYSIZE_BYTES);
}

void bench_x25519(void) {
  X25519_KEY secret_key;
  X25519_KEY public_key;
  X25519_KEY shared_secret;

  printf("===== X225519  =====\n");
  oper_second(random_X25519_key(secret_key), "KeyGen",
              X25519_KeyGen(public_key, secret_key));
  oper_second(random_X25519_key(secret_key);
              random_X25519_key(public_key), "Shared",
              X25519_Shared(shared_secret, public_key, secret_key));
}
