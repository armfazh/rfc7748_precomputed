#include <Hacl_Curve25519.h>
#include <assert.h>
#include <rfc7748_precomputed.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define N X25519_KEYSIZE_BYTES

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  if (Size != 2 * N) return 0;

  X25519_KEY shared1, shared2;
  X25519_KEY priv1, priv2;
  X25519_KEY point1, point2;

  memcpy(priv1, Data + 0, N);
  memcpy(priv2, Data + 0, N);
  memcpy(point1, Data + N, N);
  memcpy(point2, Data + N, N);

  Hacl_Curve25519_crypto_scalarmult(shared1, priv1, point1);
  X25519_Shared(shared2, point2, priv2);

  assert(memcmp(shared1, shared2, N) == 0);
  return 0;
}

#ifdef __cplusplus
}
#endif
