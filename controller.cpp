#include "controller.h"
logintg controller::tg;
servers::manager_telegram controller::mt;
void controller::telegram::get_auth_code(crow::request &req,
                                         crow::response &res,int id) {
    servers::data_telegram d=mt.get_data_id(id);
    std::cout<<"stat: "<<d.status<<"\n";
    if(mt.get_data_id(id).status!=servers::status_telegram::RESTART&&tg.startauth() != 0){
        res.body = "{\"code\":\"0\"}";
        res.end();
        return;
    }       
    mt.all_restart();
    res.body = "{\"code\":\"-1\"}";
    res.end();
}
void controller::telegram::get_id(crow::request& req, crow::response& res){
    servers::data_telegram data= mt.get_new_telegram();
    res.body = "{\"id\":"+std::to_string(data.id)+"}";
    res.end();
}