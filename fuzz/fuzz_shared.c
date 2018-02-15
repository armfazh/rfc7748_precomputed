#include <assert.h>
#include <string.h>

#include <Hacl_Curve25519.h>
#include <rfc7748_precomputed.h>

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  if (Size != 2 * 32) return 0;

  unsigned char shared1[32] = {0};
  unsigned char shared2[32] = {0};

  unsigned char priv1[32] = {0};
  unsigned char priv2[32] = {0};

  unsigned char point1[32] = {0};
  unsigned char point2[32] = {0};

  memcpy(priv1, Data + 0, 32);
  memcpy(priv2, Data + 0, 32);

  memcpy(point1, Data + 32, 32);
  memcpy(point2, Data + 32, 32);

  Hacl_Curve25519_crypto_scalarmult(shared1, priv1, point1);
  X25519_Shared(shared2, point2, priv2);

  assert(memcmp(shared1, shared2, 32) == 0);
  return 0;
}
