#pragma once
#include "url.h"

#include <iostream>
#include"telegram.h"
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
  managertg tg;
  crow::SimpleApp tasker_node_api;
  
 
};