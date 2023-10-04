#include <iostream>
#include "sht31.h"

int main() {
    SHT31 sht;

    if (!sht.begin()) {
        std::cerr << "Failed to initialize SHT31 sensor." << std::endl;
        return 1;
    }

    float temperature, humidity;

    if (sht.readData(temperature, humidity)) {
        std::cout << "Temperature: " << temperature << "°C" << std::endl;
        std::cout << "Humidity: " << humidity << "%" << std::endl;
    } else {
        std::cerr << "Failed to read data from SHT31 sensor." << std::endl;
        return 1;
    }

    return 0;
}
