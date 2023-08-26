#include "controller.h"
void controller::telegram::get_auth_code(crow::request &req,
                                         crow::response &res) {
  res.body = "{\"code\":\"0000\"}";
  res.end();
}
void controller::telegram::new_auth_code(crow::request &req,
                                         crow::response &res) {
  res.body = "okey";
  res.end();
}
void controller::web::get_tasks(crow::request &req, crow::response &res) {
  res.body = "test";
  res.end();
}