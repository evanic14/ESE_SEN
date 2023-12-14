#include <iostream>
#include </home/emiel/Documents/ESE_FORK/ESE_SEN/Assignment/SHT31_lib/SHT31.h>
#include <cstring>
#include </home/emiel/paho.mqtt.cpp/src/mqtt/async_client.h>

const std::string SERVER_ADDRESS("tcp://broker.example.com:1883");
const std::string CLIENT_ID("cpp_publisher");
const std::string TOPIC("your_topic");
float temperature, humidity;

class callback : public virtual mqtt::callback {
public:
    virtual void connection_lost(const std::string& cause) {
        std::cout << "\nConnection lost" << std::endl;
        if (!cause.empty())
            std::cout << "\tcause: " << cause << std::endl;
    }

    virtual void delivery_complete(mqtt::delivery_token_ptr token) {
        std::cout << "\nDelivery complete for token: " << (token ? token->get_message_id() : -1) << std::endl;
    }
};


int sens() {
	
	SHT31 sht;

    if (!sht.begin()) {
        std::cerr << "Failed to initialize SHT31 sensor." << std::endl;
        return 1;
    }

    //float temperature, humidity;

    if (sht.readData(temperature, humidity)) {
        std::cout << "Temperature: " << temperature << "°C" << std::endl;
        std::cout << "Humidity: " << humidity << "%" << std::endl;
    } else {
        std::cerr << "Failed to read data from SHT31 sensor." << std::endl;
        return 1;
    }

    return 0; 	
}

int main(int argc, char* argv[]) {
	sens();

    try {
        mqtt::async_client client(SERVER_ADDRESS, CLIENT_ID);

        callback cb;
        client.set_callback(cb);

        mqtt::connect_options connOpts;
        connOpts.set_keep_alive_interval(20);
        connOpts.set_clean_session(true);

        std::cout << "Connecting to server '" << SERVER_ADDRESS << "'..." << std::flush;
        client.connect(connOpts)->wait();
        std::cout << "OK\n" << std::endl;

        //std::string payload = "Hello, MQTT!";
        std::string payload = std::to_string(temperature) + ", " + std::to_string(humidity);
        mqtt::message_ptr pubmsg = mqtt::make_message(TOPIC, payload);

        client.publish(pubmsg)->wait();

        std::cout << "Message published: " << payload << std::endl;

        client.disconnect()->wait();
    } catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        return 1;
    }

    return 0;
}

