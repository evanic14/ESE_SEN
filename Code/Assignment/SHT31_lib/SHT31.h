#ifndef SHT31_H
#define SHT31_H

class SHT31 
{
	public: 
		SHT31();
		void begin();
		void readData(float& temperature, float& humidity);
		
	private:
		int i2cFile;
};

#endif
