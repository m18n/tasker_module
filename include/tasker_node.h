#pragma once
#include "url.h"
#include <iostream>
class tasker_node {
public:
  tasker_node() {
    url::init_api_url(tasker_node_api);
    tasker_node_api.port(3000).run();
  }

private:
  crow::SimpleApp tasker_node_api;
};