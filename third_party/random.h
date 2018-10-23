#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void random_bytes(uint8_t *A, int length);
void print_bytes(uint8_t *A, int num_bytes);

#ifdef __cplusplus
}
#endif

#endif /* RANDOM_H */
