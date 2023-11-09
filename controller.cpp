#include "controller.h"
//logintg* controller::tg;
tasker_manager controller::tm;
// void controller::telegram::get_auth_code(crow::request &req,
//                                          crow::response &res,int id) {
//     servers::data_telegram d=mt.get_data_id(id);
//     std::cout<<"stat: "<<d.status<<"\n";
//     if(mt.get_data_id(id).status!=servers::status_telegram::RESTART&&tg->startauth() != 0){
//         res.body = "{\"code\":\"0\"}";
//         res.end();
//         return;
//     }       
//     mt.all_restart();
    
//     res.body = "{\"code\":\"-1\"}";
//     res.end();
// }
void controller::get_events(crow::request& req, crow::response& res,std::string server_hash,std::string group,std::string hash_worker){
    std::string test=req.url;
    if(server_hash!=tm.get_server_hash()){
        t_json json;
        json["$error"]="server_hash";
        res.body=json.dump();
        res.end();
        return;
    }
    std::string str_res=tm.get_events_json(group,hash_worker);
    char* ad=&str_res[0];
    res.body=str_res;
    res.end();
}
void controller::send_event(crow::request& req, crow::response& res,std::string server_hash,std::string group,std::string hash_worker){
    std::string test=req.url;
    if(server_hash!=tm.get_server_hash()){
        t_json json;
        json["$error"]="server_hash";
        res.body=json.dump();
        res.end();
        return;
    }
    t_json respon;
    event ev;
    ev.json=t_json::parse(req.body);
    ev.json["$server_hash"]=server_hash;
    respon["$respon_id"]=tm.add_new_event(ev);
    
    res.body=respon.dump();
    res.end();
}
void controller::start_event(crow::request& req, crow::response& res,std::string server_hash,std::string group,std::string hash_worker,std::string event_id){
    res.body=tm.start_event(group,hash_worker,event_id).dump();
    res.end();
}  
    void controller::end_event(crow::request& req, crow::response& res,std::string server_hash,std::string group,std::string hash_worker,std::string event_id){
        res.body=tm.end_event(group,event_id).dump();
         res.end();
    }  
void controller::get_id(crow::request& req, crow::response& res,std::string group){
    t_json meta=t_json::parse(req.body);
    std::cout<<"META: "<<meta.dump()<<"\n";
    client cl=tm.get_new_client(meta["$time"],meta["$ip"],group);
    t_json json;
    json["$hash_worker"]=cl.hash_worker;
    json["$server_hash"]=tm.get_server_hash();
    res.body = json.dump();
    res.end();
}
void controller::exit_auth(crow::request& req, crow::response& res,std::string server_hash,std::string group,std::string hash_worker){
    if(server_hash!=tm.get_server_hash()){
        t_json json;
        json["$error"]="server_hash";
        res.body=json.dump();
        res.end();
        return;
    }
    tm.exit_client(hash_worker,group);
    res.body = "{}";
    res.end();
}