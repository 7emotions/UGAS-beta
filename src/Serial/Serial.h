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

class SerialUtil {
public:
  SerialUtil() {
    memset(&package, 0, sizeof(package));

    serial = open("/dev/CBoard", O_RDWR | O_NOCTTY | O_NDELAY);
    if (serial == -1) {
      // Can only operate on a valid file descriptor
      //THROW(IOException, "Invalid file descriptor, is the serial port open?");
	  throw std::runtime_error("Invalid file descriptor, is the serial port open?");
    }

    struct termios options; // The options for the file descriptor

    if (tcgetattr(serial, &options) == -1) {
      //THROW(IOException, "::tcgetattr");
	  throw std::runtime_error("tcgetattr error");
    }

    // set up raw mode / no echo / binary
    options.c_cflag |= (tcflag_t)(CLOCAL | CREAD);
    options.c_lflag &= (tcflag_t) ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL |
                                    ISIG | IEXTEN); //|ECHOPRT

    options.c_oflag &= (tcflag_t) ~(OPOST);
    options.c_iflag &= (tcflag_t) ~(INLCR | IGNCR | ICRNL | IGNBRK);
#ifdef IUCLC
    options.c_iflag &= (tcflag_t)~IUCLC;
#endif
#ifdef PARMRK
    options.c_iflag &= (tcflag_t)~PARMRK;
#endif

    // setup baud rate
    bool custom_baud = false;
    speed_t baud;
	auto baudrate_ = 115200;
    switch (baudrate_) {
#ifdef B0
    case 0:
      baud = B0;
      break;
#endif
#ifdef B50
    case 50:
      baud = B50;
      break;
#endif
#ifdef B75
    case 75:
      baud = B75;
      break;
#endif
#ifdef B110
    case 110:
      baud = B110;
      break;
#endif
#ifdef B134
    case 134:
      baud = B134;
      break;
#endif
#ifdef B150
    case 150:
      baud = B150;
      break;
#endif
#ifdef B200
    case 200:
      baud = B200;
      break;
#endif
#ifdef B300
    case 300:
      baud = B300;
      break;
#endif
#ifdef B600
    case 600:
      baud = B600;
      break;
#endif
#ifdef B1200
    case 1200:
      baud = B1200;
      break;
#endif
#ifdef B1800
    case 1800:
      baud = B1800;
      break;
#endif
#ifdef B2400
    case 2400:
      baud = B2400;
      break;
#endif
#ifdef B4800
    case 4800:
      baud = B4800;
      break;
#endif
#ifdef B7200
    case 7200:
      baud = B7200;
      break;
#endif
#ifdef B9600
    case 9600:
      baud = B9600;
      break;
#endif
#ifdef B14400
    case 14400:
      baud = B14400;
      break;
#endif
#ifdef B19200
    case 19200:
      baud = B19200;
      break;
#endif
#ifdef B28800
    case 28800:
      baud = B28800;
      break;
#endif
#ifdef B57600
    case 57600:
      baud = B57600;
      break;
#endif
#ifdef B76800
    case 76800:
      baud = B76800;
      break;
#endif
#ifdef B38400
    case 38400:
      baud = B38400;
      break;
#endif
#ifdef B115200
    case 115200:
      baud = B115200;
      break;
#endif
#ifdef B128000
    case 128000:
      baud = B128000;
      break;
#endif
#ifdef B153600
    case 153600:
      baud = B153600;
      break;
#endif
#ifdef B230400
    case 230400:
      baud = B230400;
      break;
#endif
#ifdef B256000
    case 256000:
      baud = B256000;
      break;
#endif
#ifdef B460800
    case 460800:
      baud = B460800;
      break;
#endif
#ifdef B500000
    case 500000:
      baud = B500000;
      break;
#endif
#ifdef B576000
    case 576000:
      baud = B576000;
      break;
#endif
#ifdef B921600
    case 921600:
      baud = B921600;
      break;
#endif
#ifdef B1000000
    case 1000000:
      baud = B1000000;
      break;
#endif
#ifdef B1152000
    case 1152000:
      baud = B1152000;
      break;
#endif
#ifdef B1500000
    case 1500000:
      baud = B1500000;
      break;
#endif
#ifdef B2000000
    case 2000000:
      baud = B2000000;
      break;
#endif
#ifdef B2500000
    case 2500000:
      baud = B2500000;
      break;
#endif
#ifdef B3000000
    case 3000000:
      baud = B3000000;
      break;
#endif
#ifdef B3500000
    case 3500000:
      baud = B3500000;
      break;
#endif
#ifdef B4000000
    case 4000000:
      baud = B4000000;
      break;
#endif
    default:
      custom_baud = true;
    }
    if (custom_baud == false) {
#ifdef _BSD_SOURCE
      ::cfsetspeed(&options, baud);
#else
      ::cfsetispeed(&options, baud);
      ::cfsetospeed(&options, baud);
#endif
    }
	auto bytesize_ = 8;
    // setup char len
    options.c_cflag &= (tcflag_t)~CSIZE;
    if (bytesize_ == 8)
      options.c_cflag |= CS8;
    else if (bytesize_ == 7)
      options.c_cflag |= CS7;
    else if (bytesize_ == 6)
      options.c_cflag |= CS6;
    else if (bytesize_ == 5)
      options.c_cflag |= CS5;
    else
      throw std::runtime_error("invalid char len");

	auto stopbits_ = 1;
    // setup stopbits
    if (stopbits_ == 1)
      options.c_cflag &= (tcflag_t) ~(CSTOPB);
    else if (stopbits_ == 3)
      // ONE POINT FIVE same as TWO.. there is no POSIX support for 1.5
      options.c_cflag |= (CSTOPB);
    else if (stopbits_ == 2)
      options.c_cflag |= (CSTOPB);
    else
      throw std::runtime_error("invalid stop bit");

	auto parity_ = 0;
    // setup parity
    options.c_iflag &= (tcflag_t) ~(INPCK | ISTRIP);
    if (parity_ == 0) {
      options.c_cflag &= (tcflag_t) ~(PARENB | PARODD);
    } else if (parity_ == 2) {
      options.c_cflag &= (tcflag_t) ~(PARODD);
      options.c_cflag |= (PARENB);
    } else if (parity_ == 1) {
      options.c_cflag |= (PARENB | PARODD);
    }
#ifdef CMSPAR
    else if (parity_ == 3) {
      options.c_cflag |= (PARENB | CMSPAR | PARODD);
    } else if (parity_ == 4) {
      options.c_cflag |= (PARENB | CMSPAR);
      options.c_cflag &= (tcflag_t) ~(PARODD);
    }
#else
    // CMSPAR is not defined on OSX. So do not support mark or space parity.
    else if (parity_ == parity_mark || parity_ == parity_space) {
      throw invalid_argument("OS does not support mark or space parity");
    }
#endif // ifdef CMSPAR
    else {
      throw std::runtime_error("invalid parity");
    }
    // setup flow control
	bool rtscts_,xonxoff_;

	auto flowcontrol_ = 0;
    if (flowcontrol_ == 0) {
      xonxoff_ = false;
      rtscts_ = false;
    }
    if (flowcontrol_ == 1) {
      xonxoff_ = true;
      rtscts_ = false;
    }
    if (flowcontrol_ == 2) {
      xonxoff_ = false;
      rtscts_ = true;
    }
    // xonxoff
#ifdef IXANY
    if (xonxoff_)
      options.c_iflag |= (IXON | IXOFF); //|IXANY)
    else
      options.c_iflag &= (tcflag_t) ~(IXON | IXOFF | IXANY);
#else
    if (xonxoff_)
      options.c_iflag |= (IXON | IXOFF);
    else
      options.c_iflag &= (tcflag_t) ~(IXON | IXOFF);
#endif
      // rtscts
#ifdef CRTSCTS
    if (rtscts_)
      options.c_cflag |= (CRTSCTS);
    else
      options.c_cflag &= (unsigned long)~(CRTSCTS);
#elif defined CNEW_RTSCTS
    if (rtscts_)
      options.c_cflag |= (CNEW_RTSCTS);
    else
      options.c_cflag &= (unsigned long)~(CNEW_RTSCTS);
#else
#error "OS Support seems wrong."
#endif

    // http://www.unixwiz.net/techtips/termios-vmin-vtime.html
    // this basically sets the read call up to be a polling read,
    // but we are using select to ensure there is data available
    // to read before each call, so we should never needlessly poll
    options.c_cc[VMIN] = 0;
    options.c_cc[VTIME] = 0;

    // activate settings
    ::tcsetattr(serial, TCSANOW, &options);

    // apply custom baud rate, if any
    if (custom_baud == true) {
      // OS X support
#if defined(MAC_OS_X_VERSION_10_4) &&                                          \
    (MAC_OS_X_VERSION_MIN_REQUIRED >= MAC_OS_X_VERSION_10_4)
      // Starting with Tiger, the IOSSIOSPEED ioctl can be used to set arbitrary
      // baud rates other than those specified by POSIX. The driver for the
      // underlying serial hardware ultimately determines which baud rates can
      // be used. This ioctl sets both the input and output speed.
      speed_t new_baud = static_cast<speed_t>(baudrate_);
      // PySerial uses IOSSIOSPEED=0x80045402
      if (-1 == ioctl(serial, IOSSIOSPEED, &new_baud, 1)) {
        THROW(IOException, errno);
      }
      // Linux Support
#elif defined(__linux__) && defined(TIOCSSERIAL)
      struct serial_struct ser;

      if (-1 == ioctl(serial, TIOCGSERIAL, &ser)) {
        THROW(IOException, errno);
      }

      // set custom divisor
      ser.custom_divisor = ser.baud_base / static_cast<int>(baudrate_);
      // update flags
      ser.flags &= ~ASYNC_SPD_MASK;
      ser.flags |= ASYNC_SPD_CUST;

      if (-1 == ioctl(serial, TIOCSSERIAL, &ser)) {
        THROW(IOException, errno);
      }
#else
      throw std::runtime_error("OS does not currently support custom bauds");
#endif
    }

    
  }

  size_t write (const uint8_t *data, size_t length)
{
  fd_set writefds;
  size_t bytes_written = 0;


  while (bytes_written < length) {
    // Only consider the timeout if it's not the first iteration of the loop
    // otherwise a timeout of 0 won't be allowed through
   


    FD_ZERO (&writefds);
    FD_SET (serial, &writefds);

    // Do the select
    int r = pselect (serial + 1, NULL, &writefds, NULL, NULL, NULL);

    // Figure out what happened by looking at select's response 'r'
    /** Error **/
    if (r < 0) {
      // Select was interrupted, try again
      if (errno == EINTR) {
        continue;
      }
    }
    /** Timeout **/
    if (r == 0) {
      break;
    }
    /** Port ready to write **/
    if (r > 0) {
      // Make sure our file descriptor is in the ready to write list
      if (FD_ISSET (serial, &writefds)) {
        // This will write some
        ssize_t bytes_written_now =
          ::write (serial, data + bytes_written, length - bytes_written);

        // even though pselect returned readiness the call might still be 
        // interrupted. In that case simply retry.
        if (bytes_written_now == -1 && errno == EINTR) {
          continue;
        }

        // write should always return some data as select reported it was
        // ready to write when we get to this point.
        if (bytes_written_now < 1) {
          // Disconnected devices, at least on Linux, show the
          // behavior that they are always ready to write immediately
          // but writing returns nothing.
          std::stringstream strs;
          strs << "device reports readiness to write but "
            "returned no data (device disconnected?)";
          strs << " errno=" << errno;
          strs << " bytes_written_now= " << bytes_written_now;
          strs << " bytes_written=" << bytes_written;
          strs << " length=" << length;
          throw std::runtime_error(strs.str().c_str());
        }
        // Update bytes_written
        bytes_written += static_cast<size_t> (bytes_written_now);
        // If bytes_written == size then we have written everything we need to
        if (bytes_written == length) {
          break;
        }
        // If bytes_written < size then we have more to write
        if (bytes_written < length) {
          continue;
        }
        // If bytes_written > size then we have over written, which shouldn't happen
        if (bytes_written > length) {
          throw std::runtime_error ("write over wrote, too many bytes where "
                                 "written, this shouldn't happen, might be "
                                 "a logical error!");
        }
      }
      // This shouldn't happen, if r > 0 our fd has to be in the list!
      throw std::runtime_error("select reports ready to write, but our fd isn't"
                          " in the list, this shouldn't happen!");
    }
  }
  return bytes_written;
}

    //     if (serial == -1) {
    //       std::cout << "Failed to open serial." << std::endl;
    //       throw std::runtime_error("Failed to open serial");
    //     }

    //     std::cout << "Serial opened." << std::endl;

    //     struct termios tty;
    //     cfsetispeed(&tty, 115200);
    //     cfsetospeed(&tty, 115200);
    //     // 设置没有校验
    //     tty.c_cflag &= ~PARENB;

    //     // 停止位 = 1
    //     tty.c_cflag &= ~CSTOPB;
    //     tty.c_cflag &= ~CSIZE;

    //     // 设置数据位 = 8
    //     tty.c_cflag |= CS8;

    //     tty.c_cflag &= ~CRTSCTS;
    //     tty.c_cflag |= CREAD | CLOCAL;

    //     // 关闭软件流动控制
    //     tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    //     // 设置操作模式
    //     tty.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    //     tty.c_oflag &= ~OPOST;

    //     if ((tcsetattr(serial, TCSANOW, &tty)) != 0) {
    //       std::cout << "Error in setting serial" << std::endl;
    //     } else {
    //       std::cout << "Baudrate=115200" << std::endl;
    //     }
    //   }

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

    uint8_t Get_CRC8_Check_Sum(uint8_t * pchMessage, uint16_t dwLength,
                               uint8_t ucCRC8) {
      uint8_t ucIndex;
      while (dwLength--) {
        ucIndex = ucCRC8 ^ (*pchMessage++);
        ucCRC8 = CRC8_TAB[ucIndex];
      }
      return (ucCRC8);
    }

    void Append_CRC8_Check_Sum(uint8_t * pchMessage, uint16_t dwLength) {
      uint8_t ucCRC = 0;
      if ((pchMessage == 0) || (dwLength <= 2))
        return;
      ucCRC = Get_CRC8_Check_Sum((uint8_t *)pchMessage, dwLength - 1, pkg_head);
      pchMessage[dwLength - 1] = ucCRC;
    }

    void recv() {
      char buff[1024] = {0};

      read(serial, buff, 1024);
      std::cout << "read:" << buff << std::endl;
    }

  private:
    static const size_t pkg_size = sizeof(Package) + 2;
    static const uint8_t pkg_head = 0xff;
    int serial;

    Package package;

    const uint8_t CRC8_TAB[256] = {
        0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83, 0xc2, 0x9c, 0x7e, 0x20,
        0xa3, 0xfd, 0x1f, 0x41, 0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e,
        0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc, 0x23, 0x7d, 0x9f, 0xc1,
        0x42, 0x1c, 0xfe, 0xa0, 0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62,
        0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d, 0x7c, 0x22, 0xc0, 0x9e,
        0x1d, 0x43, 0xa1, 0xff, 0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5,
        0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07, 0xdb, 0x85, 0x67, 0x39,
        0xba, 0xe4, 0x06, 0x58, 0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a,
        0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6, 0xa7, 0xf9, 0x1b, 0x45,
        0xc6, 0x98, 0x7a, 0x24, 0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b,
        0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9, 0x8c, 0xd2, 0x30, 0x6e,
        0xed, 0xb3, 0x51, 0x0f, 0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd,
        0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92, 0xd3, 0x8d, 0x6f, 0x31,
        0xb2, 0xec, 0x0e, 0x50, 0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c,
        0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee, 0x32, 0x6c, 0x8e, 0xd0,
        0x53, 0x0d, 0xef, 0xb1, 0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73,
        0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49, 0x08, 0x56, 0xb4, 0xea,
        0x69, 0x37, 0xd5, 0x8b, 0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4,
        0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16, 0xe9, 0xb7, 0x55, 0x0b,
        0x88, 0xd6, 0x34, 0x6a, 0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8,
        0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7, 0xb6, 0xe8, 0x0a, 0x54,
        0xd7, 0x89, 0x6b, 0x35,
    };
  };
