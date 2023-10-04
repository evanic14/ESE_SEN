#include "SHT31.h"
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

SHT31::SHT31()
{
	i2cFile = -1;
}

void SHT31::begin()
{
	i2cFile = open("/dev/i2c-1", O_RDWR);
    if (i2cFile < 0) {
        return false;
    }

    if (ioctl(i2cFile, I2C_SLAVE, 0x44) < 0) {
        close(i2cFile);
        return false;
    }

    return true;
}

void SHT31::readData(float& temperature, float& humidity) {
    unsigned char cmd[2] = {0x2C, 0x06};
    if (write(i2cFile, cmd, 2) != 2) {
        return false;
    }
    usleep(100000);

    unsigned char data[6];
    if (read(i2cFile, data, 6) != 6) {
        return false;
    }

    temperature = (((data[0] * 256.0 + data[1]) * 175) / 65535.0) - 45;
    humidity = 	  (((data[3] * 256.0 + data[4]) * 100) / 65535.0);

    return true;
}
