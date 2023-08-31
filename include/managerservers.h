#pragma once
#include<iostream>
#include<vector>
namespace servers{
    enum status_telegram{
        RESTART,
        STATE,
    };
struct data_telegram
{
    int id=-1;
    status_telegram status=status_telegram::STATE;
    void init(){
        this->id=-1;
        this->status=status_telegram::STATE;
        this->busy=false;
    }
    bool busy=false;
};

class manager_telegram{
    public:
    manager_telegram(){
        telegrams.resize(25);
        for(int i=0;i<telegrams.size();i++){
            telegrams[i].id=i;
        }
    }
    data_telegram get_new_telegram(){
         data_telegram t;
         t.busy=true;
         for(int i=0;i<telegrams.size();i++){
            if(telegrams[i].busy==false){
                telegrams[i].init();
                telegrams[i].id=i;
                telegrams[i].busy=true;
                return telegrams[i];
            }
         }
        t.id=telegrams.size();
        telegrams.push_back(t);
        return t;
    }
    void exit_auth(int id){
        telegrams[id].init();
    }
    void all_restart(){
        for(int i=0;i<telegrams.size();i++){
            if(telegrams[i].busy==true){
                telegrams[i].status=status_telegram::RESTART;
            }
        }
    }
    data_telegram get_data_id(int id){
        
        return telegrams[id];
    }
    private:
    std::vector<data_telegram> telegrams;
};
}