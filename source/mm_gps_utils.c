// mm_gps_utils.c
// Utilities

#include "mm_gps_utils.h"

uint16_t CRC16(const void *buf, size_t buflen) {
  uint8_t *arr = (uint8_t *)buf;
  bytes_t crc;
  crc.w = 0xffff;
  while (length--) {
    char i;
    uint16_t odd;
    crc.b.lo ^= *arr++;
    for (i = 0; i < 8; i++) {
      odd = crc.w & 0x01;
      crc.w >>= 1;
      if (odd)
        crc.w ^= 0xa001;
    }
  }
  return (uint16_t)crc.w;
}

void hexprint(const char *buf, size_t buflen) {
  size_t i;
  for (i = 0; i < buflen; i++) {
    printf(" %02X", (uint8_t)buf[i]);
  }
  putchar('\n');
}
