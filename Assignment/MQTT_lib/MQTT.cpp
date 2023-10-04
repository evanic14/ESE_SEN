#include "MQTT.h"

#include <mqtt/async_client.h>
#include <string>


	MqttPublisher::MqttPublisher(const std::string& brokerAddress, const std::string& clientId)
        : brokerAddress_(brokerAddress), clientId_(clientId) 
    {
        client_ = std::make_unique<mqtt::async_client>(brokerAddress, clientId);
    }

    MqttPublisher::~MqttPublisher() 
    {
        disconnect();
    }

    void MqttPublisher::connect() 
    {
        mqtt::connect_options conn_opts;
        conn_opts.set_keep_alive_interval(20);
        conn_opts.set_clean_session(true);

        client_->connect(conn_opts);
    }

    void MqttPublisher::publish(const std::string& topic, const std::string& message, int qos) 
    {
        mqtt::message_ptr msg = mqtt::make_message(topic, message);
        msg->set_qos(qos);

        client_->publish(msg)->wait_for(5000);
    }

    void MqttPublisher::disconnect() 
    {
        if (client_->is_connected()) 
        {
            client_->disconnect();
        }
    }


