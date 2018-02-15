#include <rfc7748_precomputed.h>
#include <stdio.h>
#include "random.h"

static void random_X25519_key(argKey key) {
  random_bytes(key, X25519_KEYSIZE_BYTES);
}

static void print_X25519_key(argKey key) {
  print_bytes(key, X25519_KEYSIZE_BYTES);
}

int main(void) {
  X25519_KEY alice_private_key, alice_session_key, alice_shared;
  X25519_KEY bob_private_key, bob_session_key, bob_shared;

  random_X25519_key(alice_private_key);
  printf("Alice's Private Key:   ");
  print_X25519_key(alice_private_key);

  random_X25519_key(bob_private_key);
  printf("Bob's Private Key:     ");
  print_X25519_key(bob_private_key);

  X25519_KeyGen(alice_session_key, alice_private_key);
  printf("Alice's Session Key:   ");
  print_X25519_key(alice_session_key);

  X25519_KeyGen(bob_session_key, bob_private_key);
  printf("Bob's Session Key:     ");
  print_X25519_key(bob_session_key);

  X25519_Shared(alice_shared, bob_session_key, alice_private_key);
  printf("Alice's Shared Secreet:");
  print_X25519_key(alice_shared);

  X25519_Shared(bob_shared, alice_session_key, bob_private_key);
  printf("Bob's Shared Secreet:  ");
  print_X25519_key(bob_shared);

  return 0;
}
