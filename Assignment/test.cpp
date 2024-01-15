#include <iostream>
#include <cstring>
#include <stdexcept>

#include "test.h"
#include <mosquitto.h>

using namespace std;

test::test(const char *_id, const char *_topic, const char *_host, int _port) : mosquittopp(_id)
{
  this->keepalive = 60;
  this->id = _id;
  this->port = _port;
  this->host = _host;
  this->topic = _topic;

  /**
  * error handling
  * whether broker is running ?
  */

  if (connect(host, port, keepalive) == MOSQ_ERR_ERRNO)
  {
    throw runtime_error("##ERROR##");
  }

  loop_start();
};

test::~test()
{
  loop_stop();
}

void test::on_connect(int rc)
{
  if (rc == 0)
  {
    cout << " ##-Connected with Broker-## " << std::endl;
  }
  else
  {
    cout << "##-Unable to Connect Broker-## " << std::endl;
  }
}

bool test::send_msg(const char *message)
{
  int ret = publish(NULL, this->topic, strlen(message), message, 2, false);

  return (ret == MOSQ_ERR_SUCCESS);
}

void test::on_disconnect()
{
  cout << " ##-Disconnected from Broker-## " << std::endl;
}

void test::on_publish(int mid)
{
  cout << "## - Message published successfully" << endl;
}
