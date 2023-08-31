#pragma once
#include "url.h"

#include <iostream>

class tasker_node {
public:
  tasker_node() {
    controller::tg=&tg;
    url::init_api_url(tasker_node_api);
    tasker_node_api.port(3000).run();
  }
  void telegram_auth(){
    
  }
private:
  logintg tg;
  crow::SimpleApp tasker_node_api;
  
 
};