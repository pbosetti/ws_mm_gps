// main.c


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#include "mm_gps_beacon.h"

typedef struct {
  int argc;
  char ** argv;
  char *  filename;
  FILE *  handle;
  int     port;
  uint32_t baudrate;
} gps_userdata;

int open_stream(gps_userdata *data) {
  if (strncmp(data->filename, "/dev", 4) == 0) { // serial port
    return -1;
  }
  else {
    if ((data->handle = fopen(data->filename, "rb")) == NULL) {
      return -2;
    }
    data->port = -1;
  }
  return 0;
}

char dumped_char(gps_userdata *data) {
  char result;
  if (data->handle == NULL) { // reading a port handle
    char buf[1];
    while (read(data->port, buf, 1) != 1 ) {}
    result = *buf;
  }
  else { // reading a file
    result = fgetc(data->handle);
  }
  return result;
}


int main(int argc, const char ** argv) {
  size_t i, len, code = 0;
  double coords[3];
  mm_gps *gps;
  gps_userdata data = {.argc = argc, .argv = (char **)argv, .baudrate = 115200};
  data.filename = (char *)argv[1];
  
  if ((code = open_stream(&data)) != 0) {
    printf("Code %zu, could not open port %s\n", code, data.filename);
    perror("Error was");
    return -1;
  }
  
  gps = mm_gps_init(&data);
  mm_gps_set_reader(gps, (mm_gps_char_getter)&dumped_char);
  
  for (i=0; i<100; i++) {
    if (!(len = mm_gps_next_raw_packet(gps))) {
      i--; // if len is zero, we are dealing with a fragment packet: discard it!
    }
    else {
      if (gps->buffer.packet.hedge.code == 2) {
        printf("#%03zu (%02zu bytes, %04X CRC16) [--------FROZEN packet--------]:", i, len, gps->buffer.crc16);
      }
      else if (gps->buffer.crc16 != 0) {
        printf("#%03zu (%02zu bytes, %04X CRC16) [---------CRC16 error---------]:", i, len, gps->buffer.crc16);
      }
      else {
        mm_gps_coords(gps, coords);
        printf("#%03zu (%02zu bytes, %04X CRC16) [%8.2f %6.2f %6.2f %6.2f]:", i, len, gps->buffer.crc16, mm_gps_time(gps), coords[0], coords[1], coords[2]);
      }
      // print the HEX description of the raw packet
      hexprint(gps->buffer.packet.b, len);
    }
  }
  
  // Close file handle
  if (data.handle) {
    fclose(data.handle);
  }
  else {
    close(data.port);
  }
  // Free the gps object
  mm_gps_free(gps);
  return 0;
}

