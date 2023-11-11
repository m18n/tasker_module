#pragma once

#include <iostream>
#include"web_server.h"
class tasker_node {
public:
  tasker_node() {
    
      web.set_name_server("tasker");
      web.add_conn_server("127.0.0.1:3001");
      web.start_server(3000);
      
  }
  
private:
  server::web_server web;
 
};