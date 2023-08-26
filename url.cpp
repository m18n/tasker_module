#include "url.h"
void url::init_api_url(crow::SimpleApp &app) {
  CROW_ROUTE(app, "/")
  ([]() { return "tasker node"; });
  
}