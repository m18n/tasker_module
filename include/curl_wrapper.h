#pragma once
#include"curl/curl.h"
#include<iostream>
#include <exception>
#include "crow.h"
#define IP_MANAGER "127.0.0.1:3000"
class curl_wrapper{
public:
    curl_wrapper() {
        curl_global_init(CURL_GLOBAL_ALL);
        curl_handle = curl_easy_init();
    }

    ~curl_wrapper() {
        if (curl_handle) {
            curl_easy_cleanup(curl_handle);
        }
        curl_global_cleanup();
    }
    void set_ip_server(std::string ip){
        this->ip=ip;
    }
    std::string get_page(const std::string& url) {
        std::string path=IP_MANAGER+url;
        std::string downloadedData;

        if (curl_handle) {
            curl_easy_setopt(curl_handle, CURLOPT_URL, path.c_str());
            curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &downloadedData);

            CURLcode res = curl_easy_perform(curl_handle);
            
            // if (res != CURLE_OK) {
            //     std::cerr << "Curl error: " << curl_easy_strerror(res) << std::endl;
            // }
        }

        return downloadedData;
    }
    std::string get_page(const std::string& url,std::string json) {
        std::string path=IP_MANAGER+url;
        std::string downloadedData;

        if (curl_handle) {

            curl_easy_setopt(curl_handle, CURLOPT_URL, path.c_str());
            curl_easy_setopt(curl_handle, CURLOPT_POST, 1);
            curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, json.c_str());
            curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, "Content-Type: application/json");
            curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);
            curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &downloadedData);

            CURLcode res = curl_easy_perform(curl_handle);
            
            // if (res != CURLE_OK) {
            //     std::cerr << "Curl error: " << curl_easy_strerror(res) << std::endl;
            // }
        }

        return downloadedData;
    }
    crow::json::wvalue get_page_json(const std::string& url) {
        std::string downloadedData=get_page(url);
        crow::json::wvalue jsondata;
        try
        {
            jsondata = crow::json::load(downloadedData);
        }
        catch(const std::exception &e)
        {
               
        }
        return jsondata;
    }
    crow::json::wvalue get_page_json(const std::string& url,std::string json) {
        std::string downloadedData=get_page(url,json);
        crow::json::wvalue jsondata;
        try
        {
            jsondata = crow::json::load(downloadedData);
        }
        catch(const std::exception &e)
        {
               
        }
        return jsondata;
    }
private:
    CURL* curl_handle;
    std::string ip="";
    static size_t write_callback(void* contents, size_t size, size_t nmemb, void* user_data) {
        size_t real_size = size * nmemb;
        std::string* data = static_cast<std::string*>(user_data);
        data->append(static_cast<char*>(contents), real_size);
        return real_size;
    }
};