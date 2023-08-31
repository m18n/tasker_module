#pragma once
//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov
// (arseny30@gmail.com) 2014-2023
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>
#include <vector>
#include<thread>
// Simple single-threaded example of TDLib usage.
// Real world programs should use separate thread for the user input.
// Example includes user authentication, receiving updates, getting chat list
// and sending text messages.

// overloaded
namespace detail {
template <class... Fs> struct overload;

template <class F> struct overload<F> : public F {
  explicit overload(F f) : F(f) {}
};
template <class F, class... Fs>
struct overload<F, Fs...> : public overload<F>, public overload<Fs...> {
  overload(F f, Fs... fs) : overload<F>(f), overload<Fs...>(fs...) {}
  using overload<F>::operator();
  using overload<Fs...>::operator();
};
} // namespace detail

template <class... F> auto overloaded(F... f) {
  return detail::overload<F...>(f...);
}

namespace td_api = td::td_api;

class TelegramClient {
protected:
  using Object = td_api::object_ptr<td_api::Object>;
  std::unique_ptr<td::ClientManager> client_manager_;
  std::int32_t client_id_{0};
  td_api::object_ptr<td_api::AuthorizationState> authorization_state_;
  bool are_authorized_{false};
  bool need_restart_{false};
  std::uint64_t current_query_id_{0};
  std::uint64_t authentication_query_id_{0};
  std::map<std::uint64_t, std::function<void(Object)>> handlers_;
  std::map<std::int64_t, td_api::object_ptr<td_api::user>> users_;
  bool auth = false;

public:
  TelegramClient() {
    td::ClientManager::execute(
        td_api::make_object<td_api::setLogVerbosityLevel>(1));
    client_manager_ = std::make_unique<td::ClientManager>();
    client_id_ = client_manager_->create_client_id();
    send_query(td_api::make_object<td_api::getOption>("version"), {});
    process_response(client_manager_->receive(0));
  }
  ~TelegramClient() {}

protected:
  virtual void AuthComplite() = 0;
  virtual void AuthInputCode() = 0;
  std::string get_user_name(std::int64_t user_id) const;
  std::uint64_t next_query_id();
  void send_query(td_api::object_ptr<td_api::Function> f,
                  std::function<void(Object)> handler);
  void process_response(td::ClientManager::Response response);
  auto create_authentication_query_handler();

  void on_authorization_state_update();
  void check_authentication_error(Object object);
  void process_update(td_api::object_ptr<td_api::Object> update);
};
struct channel {
  channel(int64_t id, std::string name) {
    this->id = id;
    this->name = name;
  }
  int64_t id;
  std::string name;
};

class tgauth : public TelegramClient {
public:
  tgauth() : TelegramClient() {}
  ~tgauth() {
    std::cout << "END TG\n";
    stat_loop=-1;
  }
  void Loop() {
    std::cout << "LOOP\n";
    stat_loop = 0;
    while (auth == false) {
      process_response(client_manager_->receive(0));
    }
    stat_loop=1;
  }

  int stat_loop = -1;

private:
  std::vector<channel> channels;

private:
  int last_chat = 0;
  int total_chat = 0;

private:
  virtual void AuthComplite() { auth = true; }
  virtual void AuthInputCode() {
    std::string code = "";
    // code = conn_m.get_auth_code();
    std::cin >> code;
    send_query(td_api::make_object<td_api::checkAuthenticationCode>(code),
               [this, id = authentication_query_id_](Object object) {
                 if (id == authentication_query_id_) {
                   check_authentication_error(std::move(object));
                 }
               });
  }
};
class logintg {
public:
  logintg() { 
    tg_auth = new tgauth(); 
  }
  int startauth() {
    std::cout<<"AUTH\n";
    if (tg_auth->stat_loop == -1) {
      std::cout << "START LOOP\n";
      std::thread th(&tgauth::Loop, tg_auth);
      th.detach();
      return 1;
    }else if(tg_auth->stat_loop==1){
      std::cout<<"CLEAR TG\n";
      
      delete tg_auth;
      tg_auth=new tgauth();
      return 0;
    }
    return -1;
  }
  ~logintg() { delete tg_auth; }

private:
  tgauth *tg_auth;
};