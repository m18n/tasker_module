#pragma once
#include "url.h"
#include <iostream>
class manager {
public:
  manager() {
    url::init_api_url(manager_api);
    manager_api.port(3000).run();
  }

private:
  crow::SimpleApp manager_api;
};