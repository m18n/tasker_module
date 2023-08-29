#include "controller.h"
managertg *controller::tg;
void controller::telegram::get_auth_code(crow::request &req,
                                         crow::response &res) {
  if (tg->startauth() == 0) {
    res.body = "{\"code\":\"-1\"}";
    res.end();
  } else {
    res.body = "{\"code\":\"0\"}";
    res.end();
  }
}