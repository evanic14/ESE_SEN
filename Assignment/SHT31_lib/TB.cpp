#include <gtest/gtest.h>
#include "SHT31.h"

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
    EXPECT_GE(temperature, -10.0);  // Lowest expected temp value
    EXPECT_LE(temperature, 80.0);   // Highest expected temp value
    EXPECT_GE(humidity, 0.0);       // Lowest expected hum value
    EXPECT_LE(humidity, 100.0);     // Highest expected hum value
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}