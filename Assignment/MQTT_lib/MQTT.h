#ifndef MQTT_H
#define MQTT_H

#include <mqtt/async_client.h>
#include <string>
#include <memory>

class MqttPublisher {
public:
	MqttPublisher(const std::string& brokerAddress, const std::string& clientId);
    ~MqttPublisher();
    
    void connect();
    void publish(const std::string& topic, const std::string& message, int qos = 0);
    void disconnect();
       

private:
    std::string brokerAddress_;
    std::string clientId_;
    std::unique_ptr<mqtt::async_client> client_;
};

#endif

