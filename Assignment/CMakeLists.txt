cmake_minimum_required(VERSION 3.10)

project(MqttSensorExample)

set(CMAKE_CXX_STANDARD 11)

# Add the directory containing SHT31.h to the include path
include_directories(/home/emiel/Documents/ESE_FORK/ESE_SEN/Assignment/SHT31_lib)

# Add the directory containing mqtt/async_client.h to the include path
include_directories(/home/emiel/paho.mqtt.cpp/src)

# Add the executable target
#add_executable(MqttSensorExample MQTT_SHT31.cpp)
add_executable(MqttSensorExample MQTT_SHT31.cpp /home/emiel/Documents/ESE_FORK/ESE_SEN/Assignment/SHT31_lib /home/emiel/paho.mqtt.cpp/src/async_client.cpp)


# Link the necessary libraries
target_link_libraries(MqttSensorExample pthread)  # pthread library is required for Paho MQTT

# Add other dependencies if needed
# target_link_libraries(MqttSensorExample other_library)

