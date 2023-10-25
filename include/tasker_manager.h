#pragma once
#include<iostream>
#include<vector>
#include <nlohmann/json.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <openssl/sha.h>
#include <ctime>
using t_json = nlohmann::json;
#define NAME_SERVER "tasker"
// namespace servers{
//     enum status_telegram{
//         RESTART,
//         STATE,
//     };
// struct data_telegram
// {
//     int id=-1;
//     status_telegram status=status_telegram::STATE;
//     void init(){
//         this->id=-1;
//         this->status=status_telegram::STATE;
//         this->busy=false;
//     }
//     bool busy=false;
// };
std::string sha256(const std::string& input);
struct client{
    std::string hash_id;
    std::string group;
   bool busy=false;
   time_t last_update=0;
};
void init_client(client* cl);
struct event{   
    std::string hash_id;
    t_json json;
    bool busy=false;
    bool process=false;
};
void init_event(event* ev);
struct respon_id{
    int id=-1;
};
class group_clients{
public:
group_clients(){
    group="";
    clients.resize(25);
    events.resize(100);
    ids.resize(100);
}
private:

public:
void set_group(std::string group){
    this->group=group;
}
void init(std::string server_hash){
    this->server_hash=server_hash;
}
std::string get_group(){
    return group;
}

respon_id get_respon_id(){
    for(int i=0;i<ids.size();i++){
        if(ids[i].id==-1){
            ids[i].id=i;
            return ids[i];
        }
    }
    respon_id r;
    r.id=ids.size();
    ids.push_back(r);
    return r;
}
void finish_respon_id(respon_id r){
    if(r.id<ids.size()){
        ids[r.id].id=-1;
    }
    
}
client get_new_client(std::string time,std::string ip){
        client cl;
        cl.hash_id=sha256(time+ip);
        cl.group=this->group;
        cl.last_update=std::time(nullptr);
        for(int i=0;i<clients.size();i++){
            if(clients[i].busy==false){
                clients[i]=cl;
                return cl;
            }
        }
        clients.push_back(cl);
        return cl;
    }
     void exit_client(std::string hash_id){
        for(int i=0;i<clients.size();i++){
            if(clients[i].hash_id==hash_id){
                init_client(&clients[i]);
            }
        }
        
    }
    int start_event(std::string event_id){
        
        for(int i=0;i<events.size();i++){
            if(events[i].hash_id==event_id){
                if(events[i].process==true)
                    return -2;
                events[i].process=true;
                return 0;
            }
        }
        return -3;
    }
    int end_event(std::string event_id){
        for(int i=0;i<events.size();i++){
             if(events[i].hash_id==event_id){
                if(events[i].json["meta"]["$server_hash"]==server_hash&&events[i].json["meta"]["$type_event"]=="res"){
                    respon_id r;
                    r.id=events[i].json["meta"]["$respon_id"];
                    finish_respon_id(r);
                }
                init_event(&events[i]);
                return 0;
            }
        }
        return -2;
    }
    int add_new_event(event ev,std::string server_hash){
        
        if(ev.json["meta"]["$type_event"]=="req"){
            time_t currentTime=time(nullptr); 
            std::string time=std::to_string(currentTime);
            std::string strindex;
            ev.busy=true;
            for(int i=0;i<events.size();i++){
                if(events[i].busy==false){
                    events[i]=ev;
                    strindex=std::to_string(i);
                    events[i].hash_id=sha256(server_hash+time+strindex);
                    return events[i].json["meta"]["$respon_id"];
                }
            }
            strindex=events.size();
            ev.hash_id=sha256(server_hash+time+strindex);
            events.push_back(ev);
            return ev.json["meta"]["$respon_id"];
        }else if(ev.json["meta"]["$type_event"]=="res"){
            std::cout<<"JSON ADD: "<<ev.json.dump()<<"\n";
            time_t currentTime=time(nullptr); 
            std::string time=std::to_string(currentTime);
            std::string strindex;
            ev.busy=true;
            for(int i=0;i<events.size();i++){
                if(events[i].busy==false){
                    events[i]=ev;
                    strindex=std::to_string(i);
                    events[i].hash_id=sha256(server_hash+time+strindex);
                    return -2;
                }
            }
            strindex=events.size();
            ev.hash_id=sha256(server_hash+time+strindex);
            events.push_back(ev);
            return -2;
        }
        
    }
    t_json get_events_json(std::string hash_id){
        t_json json;
        int n=0;
        for(int i=0;i<events.size();i++){
            if(events[i].busy==true&&events[i].process==false){
                json["events"][n]={{"id",events[i].hash_id},{"meta",events[i].json["meta"]},{"data",events[i].json["data"]}};
                n++;
            }
        }
        return json;
    }
    
private:
    std::vector<client> clients;
    std::vector<event> events;
    std::vector<respon_id> ids;
    std::string group="";
    std::string server_hash;
};

class tasker_manager{
private:
int find_group(std::string group){
    for(int i=0;i<clients.size();i++){
            if(clients[i].get_group()==group){
                return i;
            }
        }
    return -1;
}
std::string gethash(){
    
    time_t currentTime=time(nullptr); 
    //std::cout<<"TIME: "<<currentTime<<"\n";
    std::string hash=sha256(std::to_string(currentTime));
    return hash;
}
public:
    tasker_manager(){
        server_hash=gethash();

    }

    ~tasker_manager(){

    }
    std::string get_server_hash(){
        return server_hash;
    }
    client get_new_client(std::string time,std::string ip,std::string group){
        for(int i=0;i<clients.size();i++){
            if(clients[i].get_group()==group){
                return clients[i].get_new_client(time,ip);
            }
        }
        group_clients gr;
        gr.set_group(group);
        gr.init(server_hash);
        clients.push_back(std::move(gr));
        return clients[clients.size()-1].get_new_client(time,ip);
    }
    void exit_client(std::string hash_id,std::string group){
        int index=find_group(group);
        if(index!=-1)   
            clients[index].exit_client(hash_id);
    }
    t_json get_events_json(std::string group,std::string hash_id){
        int index=find_group(group);
        if(index!=-1)
            return clients[index].get_events_json(hash_id);
        t_json t;
        return t;
    }
    t_json start_event(std::string group,std::string event_id){
        t_json res;
        res["$status"]=-1;
        int index=find_group(group);
        if(index!=-1){
            int s=clients[index].start_event(event_id);
            res["$status"]=s;
        }
        return res;
    }
    t_json end_event(std::string group,std::string event_id){
        t_json res;
        res["$status"]=-1;
        int index=find_group(group);
         if(index!=-1){
            int s=clients[index].end_event(event_id);
            res["$status"]=s;
            
         }
        return res;
    }
    int add_new_event(event ev){
        
        std::string group;
        int size_list=ev.json["meta"]["$list_servers"].size();
        std::cout<<"LIST: "<<ev.json["meta"]["$list_servers"].dump()<<"\n";
        for(int i=0;i<size_list;i++){
            if(ev.json["meta"]["$list_servers"][i]["name"]==NAME_SERVER){
                int n=i;
                if(i+1!=size_list)
                    n++;
                group=ev.json["meta"]["$list_servers"][n]["name"];
            }
        }
        int index=find_group(group);    
        if(index!=-1){
            if(ev.json["meta"]["$type_event"]=="req"){
                if(!ev.json["meta"].contains("$respon_id")){
                    std::string group=ev.json["meta"]["$list_servers"][0]["name"];
                    int index=find_group(group);
                    if(index!=-1){
                        respon_id r=clients[index].get_respon_id();
                        ev.json["meta"]["$respon_id"]=r.id;
                        ev.json["meta"]["$server_hash"]=server_hash;
                    }
                }
            }
            return clients[index].add_new_event(ev,server_hash);
        }
        return -1;
    }

private:
    std::string server_hash;
    std::vector<group_clients> clients;
    
};
// class manager_telegram{
//     public:
//     manager_telegram(){
//         telegrams.resize(25);
//         for(int i=0;i<telegrams.size();i++){
//             telegrams[i].id=i;
//         }
//     }
//     data_telegram get_new_telegram(){
//          data_telegram t;
//          t.busy=true;
//          for(int i=0;i<telegrams.size();i++){
//             if(telegrams[i].busy==false){
//                 telegrams[i].init();
//                 telegrams[i].id=i;
//                 telegrams[i].busy=true;
//                 return telegrams[i];
//             }
//          }
//         t.id=telegrams.size();
//         telegrams.push_back(t);
//         return t;
//     }
//     void exit_auth(int id){
//         telegrams[id].init();
//     }
//     void all_restart(){
//         for(int i=0;i<telegrams.size();i++){
//             if(telegrams[i].busy==true){
//                 telegrams[i].status=status_telegram::RESTART;
//             }
//         }
//     }
//     data_telegram get_data_id(int id){
        
//         return telegrams[id];
//     }
//     private:
//     std::vector<data_telegram> telegrams;
// };
// }