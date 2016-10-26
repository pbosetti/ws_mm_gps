// mm_gps_beacon.h
// "class-like" C

#ifndef mm_gps_beacon_h
#define mm_gps_beacon_h


#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>

#define MMGPS_NUMBER_OF_BEACONS 5
#define MMGPS_SEPARATOR "\xFFG"
#define MMGPS_SEPLEN 2

#define MMGPS_MAX_PACKET_LEN MMGPS_SEPLEN + 4 + MMGPS_NUMBER_OF_BEACONS * 8 + 2 + MMGPS_SEPLEN



typedef char (*mm_gps_char_getter)(void *data);

// Buffer parsing structure
typedef union {
  char b[MMGPS_MAX_PACKET_LEN];
  
  struct __attribute__((packed)) {
    uint8_t address;
    uint8_t type;
    uint8_t code;
    uint8_t code_h;
    uint8_t size;
    uint32_t time;
    uint16_t x;
    uint16_t y;
    uint16_t z;
    uint8_t flag;
    char reserved[5];
    uint16_t crc16;
  } hedge;
  
  struct __attribute__((packed)){
    uint8_t address;
    uint8_t type;
    uint8_t code;
    uint8_t code_h;
    uint8_t size;
    uint8_t n_beacons;
    struct __attribute__((packed)){
      uint16_t x;
      uint16_t y;
      uint16_t z;
      uint8_t reserved;
    } beacon[MMGPS_NUMBER_OF_BEACONS];
  } beacons;
  
} mm_gps_packet;

// "Object" structure
typedef struct {
  struct __attribute__((packed)) {
    mm_gps_packet packet;
    char * head;
    uint16_t crc16;
  } buffer;
  void * userdata;
  mm_gps_char_getter get_next_char;
} mm_gps;


// mm_gps constructor
mm_gps * mm_gps_init(void *data);

// mm_gps destructor
void mm_gps_free(mm_gps *gps);

// Sets the char reader function
void mm_gps_set_reader(mm_gps *gps, mm_gps_char_getter f);

// Parses next raw packet
uint16_t mm_gps_next_raw_packet(mm_gps *gps);

double mm_gps_time(mm_gps * gps);
double mm_gps_x(mm_gps * gps);
double mm_gps_y(mm_gps * gps);
double mm_gps_z(mm_gps * gps);
void mm_gps_coords(mm_gps *gps, double * coords);




#endif /* mm_gps_beacon_h */