#pragma once

#include <csignal>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <termios.h>
#include <unistd.h>

;
#pragma pack(push, 1)
typedef struct PackageInfo {
  float yaw, pitch;        // 0，4
  uint16_t rect_x, rect_y; // 8, 10
  uint8_t color;           // 12 13 16
} Package;
#pragma pack(pop)

typedef enum { BYTE_FIVE = 5, BYTE_SIX, BYTE_SEVEN, BYTE_EIGHT } BYTESIZE;

typedef enum { BIT_ONE = 1, BIT_TWO, BIT_THREE } STOPBIT;

typedef enum { PARITY_NONE, PARITY_ODD, PARITY_EVEN } PARITY;

class SerialUtil {
public:
  SerialUtil(const char *port = "/dev/CBoard", int baudrate_ = 115200,
             BYTESIZE bytesize_ = BYTE_EIGHT, STOPBIT stopbits_ = BIT_ONE,
             PARITY parity_ = PARITY_NONE);

  ~SerialUtil() { close(serial); }

  /**
   * @brief pack date
   *
   * @param yaw uint in degree
   * @param pitch uint in degree
   * @param rect_x roi_x
   * @param rect_y roi_y
   * @param color color to detect
   */
  void pack(float yaw, float pitch, uint16_t rect_x, uint16_t rect_y,
            uint8_t color) {
    package.yaw = yaw;
    package.pitch = pitch;
    package.rect_x = rect_x;
    package.rect_y = rect_y;
    package.color = color;
  }

  void send();

protected:
  size_t write(const uint8_t *data, size_t length);

private:
  static const size_t pkg_size = sizeof(Package) + 2;
  static const uint8_t pkg_head = 0xff;
  int serial;

  Package package;
};
