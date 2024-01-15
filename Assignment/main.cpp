#include <iostream>
#include </home/emiel/Documents/ESE_SEN/Assignment/SHT31.h>
#include </home/emiel/Documents/ESE_SEN/Assignment/paho.mqtt.cpp/src/mqtt/client.h>

const std::string ADDRESS("tcp://localhost:1884");
const std::string CLIENT_ID("reciever");
const std::string TOPIC_TEMP("sens/temp");
const std::string TOPIC_HUM("sens/hum");

float temperature, humidity;

mqtt::client client(ADDRESS, CLIENT_ID);

int main ()
{
	try 
	{
		// Connect to MQTT Broker
		client.connect();
		
		// Create new instance of SHT31 class
		SHT31 sht;
		
		// Start I2C connection
		if (!sht.begin()) 
		{
			std::cerr << "Failed to initialize SHT31 sensor." << std::endl;
			return 1;
		}

		// Read sensor data
		if (sht.readData(temperature, humidity)) 
		{
			std::cout << "Temperature: " << temperature << "°C" << std::endl;
			std::cout << "Humidity: " << humidity << "%" << std::endl;
		} 
		else 
		{			
			std::cerr << "Failed to read data from SHT31 sensor." << std::endl;
			return 1;
		}

		// Publish sensor data using mqtt::message_ptr
		client.publish(mqtt::make_message(TOPIC_TEMP, std::to_string(temperature)));
		client.publish(mqtt::make_message(TOPIC_HUM, std::to_string(humidity)));

		// Disconnect
		client.disconnect();
	} 
	
	// Print error when one has occured
	catch (const mqtt::exception& exc) 
	{
		std::cerr << exc.what() << std::endl;
		return 1;
	}
	
	return 0;
}
