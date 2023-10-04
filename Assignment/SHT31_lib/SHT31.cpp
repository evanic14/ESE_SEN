#include "SHT31.h"
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

SHT31::SHT31(const char* i2cDevice, int i2cAddr) : _i2cDevice(i2cDevice), _i2cAddr(i2cAddr), _i2cFile(-1) {}

SHT31::~SHT31() {
    if (_i2cFile != -1) close(_i2cFile);
}

bool SHT31::begin()
{
	_i2cFile = open(_i2cDevice, O_RDWR);
    if (_i2cFile < 0) {
        return false;
    }

    if (ioctl(_i2cFile, I2C_SLAVE, _i2cAddr) < 0) {
        close(_i2cFile);
        return false;
    }

    return true;
}

bool SHT31::readData(float& temperature, float& humidity) {
    unsigned char cmd[2] = {0x2C, 0x06};
    if (write(_i2cFile, cmd, 2) != 2) {
        return false;
    }
    usleep(10000);

    unsigned char data[6];
    if (read(_i2cFile, data, 6) != 6) {
        return false;
    }

    temperature = (((data[0] * 256.0 + data[1]) * 175) / 65535.0) - 45;
    humidity = 	  (((data[3] * 256.0 + data[4]) * 100) / 65535.0);

    return true;
}
