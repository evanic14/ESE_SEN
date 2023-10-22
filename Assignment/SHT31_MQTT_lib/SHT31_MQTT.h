#ifndef SHT31_MQTT_H
#define SHT31_MQTT_H

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

class MQTT
{
public:
	MQTT(const std::string& brokerAddress, const std::string& clientId);
    ~MQTT();
    
    void connect();
    void publish(const std::string& topic, const std::string& message, int qos = 0);
    void disconnect();
       

private:
    std::string brokerAddress_;
    std::string clientId_;
    std::unique_ptr<mqtt::async_client> client_;
};

class MAIN
{
public: 
	MAIN();
	~MAIN();
	
	void init();
	void send();
	void close();
}

#endif
