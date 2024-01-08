#include <iostream>
#include <string>
#include "bme280.h"
#include "mqtt/client.h"

int main() {
// MQTT setup
const std::string ADDRESS = "tcp://localhost:1884";
const std::string CLIENT_ID = "BME280_Client";
const std::string TEMPERATURE_TOPIC = "sensor/temperature";
const std::string PRESSURE_TOPIC = "sensor/pressure";
const std::string HUMIDITY_TOPIC = "sensor/humidity";	

mqtt::client client(ADDRESS, CLIENT_ID);

try {
// Connect to MQTT Broker
client.connect();

// Create an instance of the bme280 class
bme280 sensor("/dev/i2c-1", 0x76);  // Adjust as needed

// Initialize the sensor
if (!sensor.initialize()) {
std::cerr << "Failed to initialize the BME280 sensor." << std::endl;
return 1;
}

// Read sensor data
double temperature = sensor.readTemperature();
double pressure = sensor.readPressure();
double humidity = sensor.readHumidity();

// Print the sensor data
std::cout << "Temperature: " << temperature << " °C" << std::endl;
std::cout << "Pressure: " << pressure << " Pa" << std::endl;
std::cout << "Humidity: " << humidity << " %" << std::endl;
// Publish sensor data using mqtt::message_ptr
client.publish(mqtt::make_message(TEMPERATURE_TOPIC, std::to_string(temperature)));
client.publish(mqtt::make_message(PRESSURE_TOPIC, std::to_string(pressure)));
client.publish(mqtt::make_message(HUMIDITY_TOPIC, std::to_string(humidity)));





// Disconnect
client.disconnect();
} catch (const mqtt::exception& exc) {
std::cerr << exc.what() << std::endl;
return 1;
}

return 0;
}
