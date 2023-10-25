#include"tasker_manager.h"
std::string sha256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.length());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return ss.str();
}
void init_event(event* ev){
    ev->hash_id="";
    ev->busy=false;
    ev->json.clear();
    ev->process=false;
}
void init_client(client* cl){
    cl->hash_id="";
    cl->group="";
    cl->busy=false;
}