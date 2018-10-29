#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

/** Random number Generator:
 * Taken from: https://github.com/relic-toolkit/relic/src/rand/relic_rand_call.c
 *
 * @warning Provide a secure random number generator.
 * @param buffer
 * @param num_bytes
 */
void random_bytes(uint8_t *buffer, int num_bytes) {
  int c, l, fd = open("/dev/urandom", O_RDONLY);

  if (fd == -1) {
    printf("Error opening /dev/urandom\n");
  }

  l = 0;
  do {
    c = read(fd, buffer + l, num_bytes - l);
    l += c;
    if (c == -1) {
      printf("Error reading /dev/urandom\n");
    }
  } while (l < num_bytes);

  close(fd);
}

void print_bytes(uint8_t *A, int num_bytes) {
  int i;

  printf("0x");
  for (i = num_bytes - 1; i >= 0; i--) {
    printf("%02x", A[i]);
  }
  printf("\n");
}
