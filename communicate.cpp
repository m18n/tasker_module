#include"communicate.h"
event* communicate::ev;
void communicate::init_ptr_event(event* e){
    ev=e;
}