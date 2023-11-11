#pragma once
#include "url.h"

#include <iostream>
#include"connector_manager.h"
class tasker_node {
public:
  tasker_node() {
    //controller::tg=&tg;
    url::init_api_url(tasker_node_api);
    int n=0;
    while(server::isPortOccupied(3000+n)==true){
      n++;
    }
    
    tasker_node_api.port(3000+n).run();
  }
  void telegram_auth(){
    
  }
private:
  //logintg tg;
  crow::SimpleApp tasker_node_api;
  connector::connector_manager conn;
 
};