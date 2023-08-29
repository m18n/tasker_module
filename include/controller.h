#pragma once
#include "crow.h"
#include"telegram.h"
#include <thread>

namespace controller {
    extern managertg* tg;
    
    namespace telegram{
        void get_auth_code(crow::request& req, crow::response& res);
    }
    namespace manager{

    }
    namespace chatgpt
    {
        
    }    
} // namespace controller
