// mm_gps_beacon.c

#include <stdlib.h>
#include <string.h>

#include "mm_gps_beacon.h"
#include "mm_gps_utils.h"



#define BUFLEN(gps) gps->buffer.head - gps->buffer.packet.b

// mm_gps constructor
mm_gps * mm_gps_init(void *data) {
  mm_gps *gps = (mm_gps*)malloc(sizeof(mm_gps));
  memset(gps, 0, sizeof(gps));
  gps->buffer.head = gps->buffer.packet.b;
  gps->userdata = data;
  return gps;
}

// mm_gps destructor
void mm_gps_free(mm_gps *gps) {
  free(gps);
}

// Sets the char reader function
void mm_gps_set_reader(mm_gps *gps, mm_gps_char_getter f) {
  gps->get_next_char = f;
}
  

// Parses next raw packet
uint16_t mm_gps_next_raw_packet(mm_gps *gps) {
  uint16_t i, result;
BEGIN:
  for (i = 0; i < MMGPS_MAX_PACKET_LEN; i++) {
    *gps->buffer.head = (*gps->get_next_char)(gps->userdata);
    gps->buffer.head++;
    if (memcmp(gps->buffer.head-MMGPS_SEPLEN, MMGPS_SEPARATOR, MMGPS_SEPLEN) == 0) {
      if (gps->buffer.packet.hedge.code == HEDGEHOG && BUFLEN(gps) < (MMGPS_SEPLEN+21+MMGPS_SEPLEN)) {
        // NO-OP (packet incomplete)
      }
      else if (gps->buffer.packet.hedge.code == FROZEN && BUFLEN(gps) < (MMGPS_SEPLEN+4+gps->buffer.packet.beacons.n_beacons*8+2+MMGPS_SEPLEN)) {
        // NO-OP (packet incomplete)
      }
      else {
        break;
      }
    }
  }
  if (strncmp(gps->buffer.packet.b, MMGPS_SEPARATOR, MMGPS_SEPLEN) != 0) {
    // Packet DOES NOT start with sep: discarding all data until next sep
    result = 0;
    gps->buffer.crc16 = -1;
    memset(gps->buffer.packet.b, 0, MMGPS_MAX_PACKET_LEN);
    strncpy(gps->buffer.packet.b, MMGPS_SEPARATOR, MMGPS_SEPLEN);
    gps->buffer.head = gps->buffer.packet.b + MMGPS_SEPLEN;
  }
  else { // Packet starts with sep: deal with it!
    if (gps->buffer.head <= gps->buffer.packet.b + MMGPS_SEPLEN)
      goto BEGIN;
    memset(gps->buffer.head-MMGPS_SEPLEN, 0, MMGPS_SEPLEN);
    result = gps->buffer.head - gps->buffer.packet.b - MMGPS_SEPLEN;
    gps->buffer.head = gps->buffer.packet.b + MMGPS_SEPLEN;
    gps->buffer.crc16 = CRC16(gps->buffer.packet.b, result);
  }
  return result;  
}

double mm_gps_time(mm_gps * gps) {
  if (gps->buffer.crc16 != 0 || gps->buffer.packet.hedge.code == FROZEN) {
    return 0.0;
  }
  return gps->buffer.packet.hedge.time / 64.0; // Endianess??
}

double mm_gps_x(mm_gps * gps) {
  if (gps->buffer.crc16 != 0 || gps->buffer.packet.hedge.code == FROZEN) {
    return 0.0;
  }
  return gps->buffer.packet.hedge.x / 100.0; // Endianess ??
}

double mm_gps_y(mm_gps * gps) {
  if (gps->buffer.crc16 != 0 || gps->buffer.packet.hedge.code == FROZEN) {
    return 0.0;
  }
  return gps->buffer.packet.hedge.y / 100.0; // Endianess??
}

double mm_gps_z(mm_gps * gps) {
  if (gps->buffer.crc16 != 0 || gps->buffer.packet.hedge.code == FROZEN) {
    return 0.0;
  }
  return gps->buffer.packet.hedge.z / 100.0; // Endianess??
}

void mm_gps_coords(mm_gps *gps, double * coords) {
  coords[0] = mm_gps_x(gps);
  coords[1] = mm_gps_y(gps);
  coords[2] = mm_gps_z(gps);
}
