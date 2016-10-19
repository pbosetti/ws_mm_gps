// mm_gps_utils.h

#ifndef mm_gps_utils_h
#define mm_gps_utils_h

#include <stdio.h>
#include <stdint.h>

typedef union {
  uint16_t w;
  struct __attribute__((packed)) {
    uint8_t lo;
    uint8_t hi;
  } b;
  uint8_t bs[2];
} bytes_t;


uint16_t CRC16(const void * buf, size_t buflen);
void hexprint(const char * buf, size_t buflen);





#endif /* mm_gps_util_h */