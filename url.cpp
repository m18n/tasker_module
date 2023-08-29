#include "url.h"
void url::init_api_url(crow::SimpleApp &app) {
  CROW_ROUTE(app, "/")
  ([]() { return "tasker node"; });
  // TELEGRAM
  CROW_ROUTE(app, "/api/telegram/getauthcode")
  ([]( crow::request &req, crow::response &res)  { controller::telegram::get_auth_code(req,res); });
  //MANAGER

  //CHATGPT


}