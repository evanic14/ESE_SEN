#include <gtest/gtest.h>
#include "SHT31_MQTT.h"

class SHT31Test : public ::testing::Test {
protected:
    SHT31 sensor;

    void SetUp() override {
        // Set up any necessary resources before each test
    }

    void TearDown() override {
        // Clean up any allocated resources after each test
    }
};

TEST_F(SHT31Test, Initialization) {
    // Test SHT31 initialization
    EXPECT_TRUE(sensor.begin());
}

TEST_F(SHT31Test, ReadData) {
    // Test SHT31 readData method
    float temperature, humidity;
    EXPECT_TRUE(sensor.begin()); // Ensure sensor is initialized successfully

    // Assuming the sensor is connected and providing valid data
    EXPECT_TRUE(sensor.readData(temperature, humidity));
    EXPECT_GE(temperature, -45.0);  // Replace with actual expected values
    EXPECT_LE(temperature, 80.0);
    EXPECT_GE(humidity, 0.0);
    EXPECT_LE(humidity, 100.0);
}

TEST_F(SHT31Test, Connection) {
    // Test MQTT publisher connection
    EXPECT_NO_THROW(publisher.connect());
    EXPECT_TRUE(publisher.isConnected());
}

TEST_F(SHT31Test, Disconnection) {
    // Test MQTT publisher disconnection
    publisher.connect(); // Connect before disconnecting
    EXPECT_NO_THROW(publisher.disconnect());
    EXPECT_FALSE(publisher.isConnected());
}

TEST_F(SHT31Test, Publish) {
    // Test MQTT message publishing
    publisher.connect(); // Connect before publishing

    const std::string topic = "test_topic";
    const std::string message = "Hello, MQTT!";
    int qos = 1;

    EXPECT_NO_THROW(publisher.publish(topic, message, qos));
    // Add more specific checks if needed
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}