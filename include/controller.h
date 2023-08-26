#pragma once
#include "crow.h"
namespace controller {
    namespace telegram{
        void get_auth_code(crow::request &req, crow::response &res);
        void new_auth_code(crow::request &req, crow::response &res);
    }
    namespace web{
        void get_tasks(crow::request &req, crow::response &res);
    }
} // namespace controller
