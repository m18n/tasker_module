#pragma once
#include<iostream>
#include"crow.h"
class event
{
private:
    std::string events;
    
public:
    event(){

    }
    void add_event(crow::json::wvalue json_event){
        events+=json_event.dump(true);
    }
    std::string get_events(){
        return events;
    }
    ~event(){
        
    }
};

