#pragma once

#include <iostream>
#include"web_server.h"
class tasker_node {
public:
  tasker_node() {
      web.start_server(3000);
  }
  
private:
  server::web_server web;
 
};