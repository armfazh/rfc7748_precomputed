#include <Hacl_Curve25519.h>
#include <assert.h>
#include <rfc7748_precomputed.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define N X25519_KEYSIZE_BYTES

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  if (Size != N) return 0;

  X25519_KEY session1, session2;
  X25519_KEY priv1, priv2;
  X25519_KEY point1;

  memcpy(priv1, Data + 0, N);
  memcpy(priv2, Data + 0, N);
  memset(point1, 0, N);
  point1[0] = 9;

  Hacl_Curve25519_crypto_scalarmult(session1, priv1, point1);
  X25519_KeyGen(session2, priv2);

  assert(memcmp(session1, session2, N) == 0);
  return 0;
}

#ifdef __cplusplus
}
#endif
