#include <assert.h>
#include <string.h>

#include <Hacl_Curve25519.h>
#include <rfc7748_precomputed.h>

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  if (Size != 2 * 32) return 0;

  unsigned char session1[32] = {0};
  unsigned char session2[32] = {0};

  unsigned char priv1[32] = {0};
  unsigned char priv2[32] = {0};

  unsigned char point1[32] = {0};

  memcpy(priv1, Data + 0, 32);
  memcpy(priv2, Data + 0, 32);
  point1[0] = 9;

  Hacl_Curve25519_crypto_scalarmult(session1, priv1, point1);
  X25519_KeyGen(session2, priv2);

  assert(memcmp(session1, session2, 32) == 0);
  return 0;
}
