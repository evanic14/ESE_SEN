/*
 * main.cxx
 * 
 * Copyright 2023  <rubenk@raspberrypi>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */
 #include <iostream>

#include "bme280.h"


	int main() {
    // Create an instance of the bme280 class
    bme280 sensor("/dev/i2c-1", 0x77);  // Adjust the I2C device and address as needed

    // Initialize the sensor
    if (!sensor.initialize()) {
        std::cerr << "Failed to initialize the BME280 sensor." << std::endl;
        return 1;
    }


    // Read temperature, pressure, and humidity
    double temperature = sensor.readTemperature();
    double pressure = sensor.readPressure();
    double humidity = sensor.readHumidity();

    // Print the sensor data
    std::cout << "Temperature: " << temperature << " °C" << std::endl;
    std::cout << "Pressure: " << pressure << " Pa" << std::endl;
    std::cout << "Humidity: " << humidity << " %" << std::endl;

    return 0;
}



