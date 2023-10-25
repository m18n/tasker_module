#pragma once
#include"curl_wrapper.h"
#include"event.h"
#include<thread>

namespace communicate{
    extern event* ev;
    void init_ptr_event(event* e);
    class global_tasker{
        private:

        public:
            global_tasker(){
                curl.set_ip_server("127.0.0.1:3000");
            }
            void get_code(){
                crow::json::wvalue val=curl.get_page_json("/api/tasker/getcode");
                
            }
            // void get_event(){
            //     crow::json::wvalue val=curl.get_page_json("/api/web/getevent");
            // }
            // void answer_event(crow::json::wvalue val){
            //     std::string id=val["id_event"].dump(true);
            //     curl.get_page_json("/api/web/code/"+id,val.dump(true));
            // }
            // void start_loop(){
            //     work_loop=true;
            //     th=new std::thread(&global_tasker::loop,this);
            // }
            // void loop(){
            //     while(work_loop==true){
            //         get_event();
            //         sleep(1);
            //     }
            // }
            // void finish_loop(){
            //     work_loop=false;
            //     th->join();
            //     delete th;
            // }
            ~global_tasker(){
                //finish_loop();
            }
        private:
            std::thread* th;
            curl_wrapper curl;
            bool work_loop=false;
    };
}