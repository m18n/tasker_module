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
    int id=0;
    status_telegram status=status_telegram::STATE;
};

class manager_telegram{
    public:
    data_telegram get_new_telegram(){
         data_telegram t;
        if(telegrams.size()!=0){
            t.id=telegrams[telegrams.size()-1].id+1;
        }
        telegrams.push_back(t);
        return t;
    }
    void all_restart(){
        for(int i=0;i<telegrams.size();i++){
            telegrams[i].status=status_telegram::RESTART;
        }
    }
    data_telegram get_data_id(int id){
        for(int i=0;i<telegrams.size();i++){
            if(telegrams[i].id==id){
                return telegrams[i];
            }
        }
        data_telegram t;
        return t;
    }
    private:
    std::vector<data_telegram> telegrams;
};
}