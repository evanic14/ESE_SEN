    	#include "bme280.h"
    	
    	
    	
    	// Constants
    	const uint8_t BME280_RESET_REG = 0xE0;
    	const uint8_t BME280_SOFT_RESET_CODE = 0xB6;
    	const uint8_t BME280_CHIP_ID_REG = 0xD0;
    	const uint8_t BME280_CHIP_ID = 0x60;  // BME280 chip ID
    	
    	// Calibration data register addresses
    	// Add the specific addresses based on the datasheet
    	
    	bme280::bme280(const char* i2c_device, int i2c_address) : address(i2c_address) {
    	// Open the I2C device
    	file_i2c = open(i2c_device, O_RDWR);
    	if (file_i2c < 0) {
    	std::cerr << "Failed to open the I2C bus at " << i2c_device << ". Error: " << std::strerror(errno) << std::endl;
    	return;
    	}
    	
    	// Specify the address of the I2C Slave to communicate with
    	if (ioctl(file_i2c, I2C_SLAVE, address) < 0) {
    	std::cerr << "Failed to acquire bus access and/or talk to slave at address 0x" << std::hex << address;
    	std::cerr << ". Error: " << std::strerror(errno) << std::endl;
    	close(file_i2c);  // Close the I2C device if we can't set it up
    	}
    	}
    	
    	bool bme280::initialize() {
    	// Check if sensor, i.e., the chip ID is correct
    	uint8_t chipId = read8(BME280_REGISTER_CHIPID);
    	if (chipId != 0x60) {
    	return false;
    	}
    	
    	// Reset the device using soft-reset
    	write8(BME280_REGISTER_SOFTRESET, 0xB6);
    	
    	// Wait for chip to wake up.
    	usleep(10000);  // 10 milliseconds
    	
    	// If chip is still reading calibration, delay
    	while (isReadingCalibration()) {
    	usleep(20000);  // 10 milliseconds
    	}
    	
    	readCoefficients();  // Read trimming parameters
    	
    	setSampling();  // Use default sampling settings
    	
    	usleep(100000);  // 100 milliseconds
    	
    	return true;
    	}
    	
    	void bme280::write8(uint8_t reg, uint8_t value) {
    	uint8_t buffer[2];
    	buffer[0] = reg;
    	buffer[1] = value;
    	
    	if (write(file_i2c, buffer, 2) != 2) {
    	std::cerr << "Failed to write to the i2c bus." << std::endl;
    	}
    	}
    	uint8_t bme280::read8(uint8_t reg) {
    	uint8_t value;
    	
    	// Write the register we want to read from
    	if (write(file_i2c, &reg, 1) != 1) {
    	std::cerr << "Failed to write to the i2c bus." << std::endl;
    	}
    	
    	// Read from the register
    	if (read(file_i2c, &value, 1) != 1) {
    	std::cerr << "Failed to read from the i2c bus." << std::endl;
    	}
    	
    	return value;
    	}
    	
    	bool bme280::isReadingCalibration() {
			uint8_t const rStatus = read8(BME280_REGISTER_STATUS);

			return (rStatus & (1 << 0)) != 0;
    	}
    	
    	uint16_t bme280::read16(uint8_t reg) {
    	uint8_t buffer[2];
    	uint16_t value;
    	
    	// Write the register we want to read from
    	if (write(file_i2c, &reg, 1) != 1) {
    	std::cerr << "Failed to write to the i2c bus." << std::endl;
    	}
    	
    	// Read 2 bytes from the register
    	if (read(file_i2c, buffer, 2) != 2) {
    	std::cerr << "Failed to read from the i2c bus." << std::endl;
    	}
    	
    	// Combine the two bytes into a 16-bit value (little-endian)
    	value = (uint16_t(buffer[1]) << 8) | uint16_t(buffer[0]);
    	
    	return value;
    	}
    	
    	int16_t bme280::readS16_LE(uint8_t reg) {
    	// Read a 16-bit little-endian value using read16_LE
    	int16_t value = (int16_t)read16_LE(reg);
    	return value;
    	}
    	
    	uint16_t bme280::read16_LE(uint8_t reg) {
    	uint16_t temp = read16(reg);
    	return (temp >> 8) | (temp << 8);
    	}
    	
    	uint32_t bme280::read24(uint8_t reg) {
    	uint8_t buffer[3];
    	uint32_t value;
    	
    	// Write the register we want to read from
    	if (write(file_i2c, &reg, 1) != 1) {
    	std::cerr << "Failed to write to the i2c bus." << std::endl;
    	}
    	
    	// Read 3 bytes from the register
    	if (read(file_i2c, buffer, 3) != 3) {
    	std::cerr << "Failed to read from the i2c bus." << std::endl;
    	}
    	
    	// Combine the three bytes into a 24-bit value (big-endian)
    	value = (uint32_t(buffer[0]) << 16) | (uint32_t(buffer[1]) << 8) | uint32_t(buffer[2]);
    	
    	return value;
    	}
    	void bme280::readCoefficients() {
    	 _bme280_calib.dig_T1 = read16_LE(BME280_REGISTER_DIG_T1);
  _bme280_calib.dig_T2 = readS16_LE(BME280_REGISTER_DIG_T2);
  _bme280_calib.dig_T3 = readS16_LE(BME280_REGISTER_DIG_T3);

  _bme280_calib.dig_P1 = read16_LE(BME280_REGISTER_DIG_P1);
  _bme280_calib.dig_P2 = readS16_LE(BME280_REGISTER_DIG_P2);
  _bme280_calib.dig_P3 = readS16_LE(BME280_REGISTER_DIG_P3);
  _bme280_calib.dig_P4 = readS16_LE(BME280_REGISTER_DIG_P4);
  _bme280_calib.dig_P5 = readS16_LE(BME280_REGISTER_DIG_P5);
  _bme280_calib.dig_P6 = readS16_LE(BME280_REGISTER_DIG_P6);
  _bme280_calib.dig_P7 = readS16_LE(BME280_REGISTER_DIG_P7);
  _bme280_calib.dig_P8 = readS16_LE(BME280_REGISTER_DIG_P8);
  _bme280_calib.dig_P9 = readS16_LE(BME280_REGISTER_DIG_P9);

  _bme280_calib.dig_H1 = read8(BME280_REGISTER_DIG_H1);
  _bme280_calib.dig_H2 = readS16_LE(BME280_REGISTER_DIG_H2);
  _bme280_calib.dig_H3 = read8(BME280_REGISTER_DIG_H3);
  _bme280_calib.dig_H4 = ((int8_t)read8(BME280_REGISTER_DIG_H4) << 4) |
                         (read8(BME280_REGISTER_DIG_H4 + 1) & 0xF);
  _bme280_calib.dig_H5 = ((int8_t)read8(BME280_REGISTER_DIG_H5 + 1) << 4) |
                         (read8(BME280_REGISTER_DIG_H5) >> 4);
  _bme280_calib.dig_H6 = (int8_t)read8(BME280_REGISTER_DIG_H6);
    	}
    	
    	void bme280::setSampling(sensor_mode mode,
    	sensor_sampling tempSampling,
    	sensor_sampling pressSampling,
    	sensor_sampling humSampling,
    	sensor_filter filter,
    	standby_duration duration) {
    	// Create the configuration bytes for the registers
    	uint8_t ctrl_hum = humSampling;
    	uint8_t config = (filter << 2) | duration;
    	uint8_t ctrl_meas = (mode << 5) | (tempSampling << 2) | pressSampling;
    	
    	// Set the sensor to sleep mode before configuring
    	write8(BME280_REGISTER_CONTROL, MODE_SLEEP);
    	
    	// Set the humidity control register
    	write8(BME280_REGISTER_CONTROLHUMID, ctrl_hum);
    	
    	// Set the configuration register
    	write8(BME280_REGISTER_CONFIG, config);
    	
    	// Set the control measurement register
    	write8(BME280_REGISTER_CONTROL, ctrl_meas);
    	}	
    	
    	float bme280::readTemperature(void) {
    	  int32_t var1, var2;

  int32_t adc_T = read24(BME280_REGISTER_TEMPDATA);
  std::cout << "adc_T: " << adc_T << " Pa" << std::endl;
  if (adc_T == 0x800000) // value in case temp measurement was disabled
    return 0;
  adc_T >>= 4;

  var1 = (int32_t)((adc_T / 8) - ((int32_t)_bme280_calib.dig_T1 * 2));
  var1 = (var1 * ((int32_t)_bme280_calib.dig_T2)) / 2048;
  var2 = (int32_t)((adc_T / 16) - ((int32_t)_bme280_calib.dig_T1));
  var2 = (((var2 * var2) / 4096) * ((int32_t)_bme280_calib.dig_T3)) / 16384;
	std::cout << "_bme280_calib.dig_T1: " << _bme280_calib.dig_T1 << " " << std::endl;
	std::cout << "_bme280_calib.dig_T2: " << _bme280_calib.dig_T2 << " " << std::endl;
std::cout << "_bme280_calib.dig_T3 " << _bme280_calib.dig_T3 << " " << std::endl;
  t_fine = var1 + var2 + t_fine_adjust;
	std::cout << "t_fine: " << t_fine << " Pa" << std::endl;

  int32_t T = (t_fine * 5 + 128) / 256;

  return (float)T / 100;
    	}
    	
    	
    	float bme280::readHumidity(void) 
    	{  	
    	
    	//!< to pressure and humidity readings
    	int32_t var1, var2, var3, var4, var5;
    	
    	readTemperature(); // must be done first to get t_fine
    	
    	int32_t adc_H = read16(BME280_REGISTER_HUMIDDATA);
    	if (adc_H == 0x8000) // value in case humidity measurement was disabled
    	return 0;
    	var1 = t_fine - ((int32_t)76800);
    	var2 = (int32_t)(adc_H * 16384);
    	var3 = (int32_t)(((int32_t)_bme280_calib.dig_H4) * 1048576);
    	var4 = ((int32_t)_bme280_calib.dig_H5) * var1;
    	var5 = (((var2 - var3) - var4) + (int32_t)16384) / 32768;
    	var2 = (var1 * ((int32_t)_bme280_calib.dig_H6)) / 1024;
    	var3 = (var1 * ((int32_t)_bme280_calib.dig_H3)) / 2048;
    	var4 = ((var2 * (var3 + (int32_t)32768)) / 1024) + (int32_t)2097152;
    	var2 = ((var4 * ((int32_t)_bme280_calib.dig_H2)) + 8192) / 16384;
    	var3 = var5 * var2;
    	var4 = ((var3 / 32768) * (var3 / 32768)) / 128;
    	var5 = var3 - ((var4 * ((int32_t)_bme280_calib.dig_H1)) / 16);
    	var5 = (var5 < 0 ? 0 : var5);
    	var5 = (var5 > 419430400 ? 419430400 : var5);
    	uint32_t H = (uint32_t)(var5 / 4096);
    	
    	return (float)H / 1024.0;
    	}
    	float bme280::readPressure(void) {
    	int64_t var1, var2, var3, var4;
    	
    	readTemperature(); // Must be done first to get t_fine
    	
    	int32_t adc_P = read24(BME280_REGISTER_PRESSUREDATA);
    	if (adc_P == 0x800000) // Value in case pressure measurement was disabled
    	return 0;
    	adc_P >>= 4;
    	
    	var1 = ((int64_t)t_fine) - 128000;
    	var2 = var1 * var1 * (int64_t)_bme280_calib.dig_P6;
    	var2 = var2 + ((var1 * (int64_t)_bme280_calib.dig_P5) * 131072);
    	var2 = var2 + (((int64_t)_bme280_calib.dig_P4) * 34359738368);
    	var1 = ((var1 * var1 * (int64_t)_bme280_calib.dig_P3) / 256) +
    	((var1 * ((int64_t)_bme280_calib.dig_P2) * 4096));
    	var3 = ((int64_t)1) * 140737488355328;
    	var1 = (var3 + var1) * ((int64_t)_bme280_calib.dig_P1) / 8589934592;
    	
    	if (var1 == 0) {
    	return 0; // Avoid exception caused by division by zero
    	}
    	
    	var4 = 1048576 - adc_P;
    	var4 = (((var4 * 2147483648) - var2) * 3125) / var1;
    	var1 = (((int64_t)_bme280_calib.dig_P9) * (var4 / 8192) * (var4 / 8192)) /
    	33554432;
    	var2 = (((int64_t)_bme280_calib.dig_P8) * var4) / 524288;
    	var4 = ((var4 + var1 + var2) / 256) + (((int64_t)_bme280_calib.dig_P7) * 16);
    	
    	float P = var4 / 256.0;
    	
    	return P;
    	}
