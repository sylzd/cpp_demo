// Copyright 2023 lzd
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// rm -f bookstore_client; g++ client.cpp -std=c++14 -I./LuaBridge/Source  -I/usr/local/include/luajit-2.1 -lluajit-5.1 -ldl -lzmq -lpthread -lcurl -g -O0 -o bookstore_client;./bookstore_client

#include <iostream>
#include <thread>

#include "config.hpp"
#include "cpplang.hpp"
#include "sales_data.hpp"
#include "myzmq.hpp"

#include "nlohmann/json.hpp" // brew install nlohmann-json

using namespace std;
using namespace cpp_study;

using string_view_t = const std::string&;
using uint_t = unsigned int;
using currency_t = double;

static auto debug_print = [](SalesData& b) {
    using json_t = nlohmann::json;
    json_t j;

    j["id"] = b.id();
    j["sold"] = b.sold();
    j["revenue"] = b.revenue();

    cout << j.dump(4) << endl; // indent = 4
};

//-> msgpack::sbuffer
static auto make_sales = [](const string_view_t& id, uint_t s, currency_t r) { 
    return SalesData(id, s, r);
};

// zmq send
static auto send_sales = [](const string& addr, const msgpack::sbuffer& buf) {
    using zmq_ctx = ZMQContext<1>; // 单线程
    auto sock = zmq_ctx::send_sock(); // 自动类型推导，获取接收socket

    sock.connect(addr); // 绑定zmq端口
    assert(sock.connected());

    auto len = sock.send(buf.data(), buf.size());
    assert(len == buf.size());

    cout << "send len = " << len << endl;
};

int main() {
try {
    cout << "Hello, I am a Book Store Client!" << endl;
    
    Config conf;
    conf.load("conf.lua");
    auto addr = conf.get<string>("config.zmq_ipc_addr");
    cout << "send to: " << addr << endl;

    auto b1 = make_sales("book1", 1, 1.0);
    auto b2 = make_sales("book2", 2, 9.5);
    debug_print(b1);
    debug_print(b2);

    send_sales(addr, b1.pack());

    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    send_sales(addr, b2.pack());
}

catch (const std::exception& e) {
    cout << e.what() << endl;
}
// any exception
catch (...) {
    cout << "unknown exception" << endl;
}

}