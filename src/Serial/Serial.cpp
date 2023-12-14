#include "Serial/Serial.h"

#include <opencv2/core/hal/interface.h>
#include <unistd.h>

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "Serial/crc/crc.h"

SerialUtil::SerialUtil(const char *port, int baudrate_, BYTESIZE bytesize_,
					   STOPBIT stopbits_, PARITY parity_) {
	memset(&package, 0, sizeof(package));

	serial = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
	if (serial == -1) {
		// Can only operate on a valid file descriptor
		// THROW(IOException, "Invalid file descriptor, is the serial port
		// open?");
		throw std::runtime_error(
			"Invalid file descriptor, is the serial port open?");
	}

	struct termios options;	 // The options for the file descriptor

	if (tcgetattr(serial, &options) == -1) {
		// THROW(IOException, "::tcgetattr");
		throw std::runtime_error("tcgetattr error");
	}

	// set up raw mode / no echo / binary
	options.c_cflag |= (tcflag_t)(CLOCAL | CREAD);
	options.c_lflag &= (tcflag_t) ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL |
									ISIG | IEXTEN);	 //|ECHOPRT

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
	speed_t baud = B115200;
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

	// setup char len
	options.c_cflag &= (tcflag_t)~CSIZE;
	if (bytesize_ == BYTE_EIGHT)
		options.c_cflag |= CS8;
	else if (bytesize_ == BYTE_SEVEN)
		options.c_cflag |= CS7;
	else if (bytesize_ == BYTE_SIX)
		options.c_cflag |= CS6;
	else if (bytesize_ == BYTE_FIVE)
		options.c_cflag |= CS5;
	else
		throw std::runtime_error("invalid char len");

	// setup stopbits
	if (stopbits_ == BIT_ONE)
		options.c_cflag &= (tcflag_t) ~(CSTOPB);
	else if (stopbits_ == BIT_THREE)
		// ONE POINT FIVE same as TWO.. there is no POSIX support for 1.5
		options.c_cflag |= (CSTOPB);
	else if (stopbits_ == BIT_TWO)
		options.c_cflag |= (CSTOPB);
	else
		throw std::runtime_error("invalid stop bit");

	// setup parity
	options.c_iflag &= (tcflag_t) ~(INPCK | ISTRIP);
	if (parity_ == PARITY_NONE) {
		options.c_cflag &= (tcflag_t) ~(PARENB | PARODD);
	} else if (parity_ == PARITY_EVEN) {
		options.c_cflag &= (tcflag_t) ~(PARODD);
		options.c_cflag |= (PARENB);
	} else if (parity_ == PARITY_ODD) {
		options.c_cflag |= (PARENB | PARODD);
	}

	else {
		throw std::runtime_error("invalid parity");
	}
	// setup flow control
	bool rtscts_, xonxoff_;

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
		options.c_iflag |= (IXON | IXOFF);	//|IXANY)
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
	options.c_cc[VMIN] = 0;
	options.c_cc[VTIME] = 0;

	// activate settings
	::tcsetattr(serial, TCSANOW, &options);

	// apply custom baud rate, if any
	if (custom_baud == true) {
		throw std::runtime_error("OS does not currently support custom bauds");
	}
}

size_t SerialUtil::write(const uint8_t *data, size_t length) {
	fd_set writefds;
	size_t bytes_written = 0;

	while (bytes_written < length) {
		FD_ZERO(&writefds);
		FD_SET(serial, &writefds);

		// Do the select
		int r = pselect(serial + 1, NULL, &writefds, NULL, NULL, NULL);

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
			if (FD_ISSET(serial, &writefds)) {
				// This will write some
				ssize_t bytes_written_now = ::write(
					serial, data + bytes_written, length - bytes_written);

				// even though pselect returned readiness the call might still
				// be interrupted. In that case simply retry.
				if (bytes_written_now == -1 && errno == EINTR) {
					continue;
				}

				// write should always return some data as select reported it
				// was ready to write when we get to this point.
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
				bytes_written += static_cast<size_t>(bytes_written_now);
				// If bytes_written == size then we have written everything we
				// need to
				if (bytes_written == length) {
					break;
				}
				// If bytes_written < size then we have more to write
				if (bytes_written < length) {
					continue;
				}
				// If bytes_written > size then we have over written, which
				// shouldn't happen
				if (bytes_written > length) {
					throw std::runtime_error(
						"write over wrote, too many bytes where "
						"written, this shouldn't happen, might be "
						"a logical error!");
				}
			}
			// This shouldn't happen, if r > 0 our fd has to be in the list!
			throw std::runtime_error(
				"select reports ready to write, but our fd isn't"
				" in the list, this shouldn't happen!");
		}
	}
	return bytes_written;
}

void SerialUtil::send() {
	uint8_t pkg[pkg_size] = {0};

	auto ptr = (uint8_t *)&package;

	pkg[0] = 0xff;

	for (size_t i = 0; i < pkg_size - 1; i++) {
		pkg[i + 1] = *(ptr + i);
	}

	Append_CRC8_Check_Sum(pkg, pkg_size);

	auto count = write(pkg, pkg_size);
	if (count != pkg_size) {
		std::cout << "send error" << std::endl;
	}
}
