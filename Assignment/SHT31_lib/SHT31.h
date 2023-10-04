#ifndef SHT31_H
#define SHT31_H

#include <cstdint>

class SHT31 
{
	public: 
		SHT31(const char* i2cDevice = "/dev/i2c-1", int i2cAddr = 0x44);
		~SHT31();
		
		bool begin();
		bool readData(float& temperature, float& humidity);
		
	private:
		const char* _i2cDevice;
		int _i2cAddr;
		int _i2cFile;
};

#endif
