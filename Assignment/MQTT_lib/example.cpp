#include "MQTT.h"

int main() {
	try 
	{
    const std::string brokerAddress = "tcp://broker.mosquitto.com:1883";
    const std::string clientId = "mqtt_publisher10";
    const std::string topic = "test";

    MqttPublisher publisher(brokerAddress, clientId);
    publisher.connect();

    std::string message = "Hello, MQTT!";
    publisher.publish(topic, message);

    publisher.disconnect();
    
    
    // MQTT code here
	} catch (const mqtt::exception& exc) 
	{
		std::cerr << "MQTT Exception: " << exc.what() << std::endl;
	}


    return 0;
}
