#pragma once
#include "crow.h"
#include"telegram.h"
#include <thread>
#include"managerservers.h"

namespace controller {
    extern logintg* tg;
    extern servers::manager_telegram mt;
    namespace telegram{
        void get_auth_code(crow::request& req, crow::response& res,int id);
        void get_id(crow::request& req, crow::response& res);
        void exit_auth(crow::request& req, crow::response& res,int id);
    }
    namespace manager{

    }
    namespace chatgpt
    {
        
    }    
} // namespace controller
