#ifndef TEST_H
#define TEST_H

#include <mosquitto.h>

class test : public mosqpp::mosquittopp
{
  private:
   const char * host;
   const char * id;
   const char * topic;
   int		port;
   int		keepalive;

   void on_connect(int rc);
   void on_disconnect();
   void on_publish(int mid);

  public:
   test(const char *id, const char * _topic, const char *host, int port);
   ~test();
   bool send_msg(const char *message);
   std::string qrcam();

};

#endif
