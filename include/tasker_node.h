#pragma once

#include <iostream>
#include"web_server.h"
class tasker_node {
public:
  tasker_node() {
      connector::init_logg_connector(&log_conn);
      server::init_logg_server(&log_serv);
      web.set_name_server("tasker");
      web.add_conn_server("127.0.0.1:3001");
      web.start_server(3000);
      
  }
  
private:
  connector::Logger log_conn;
  server::Logger log_serv;
  server::web_server web;
 
};