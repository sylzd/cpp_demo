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

// rm -f ./bookstore_server; g++ server.cpp -std=c++14 -I./LuaBridge/Source -I/usr/local/include/luajit-2.1 -lluajit-5.1 -ldl -lzmq -lpthread -lcurl -g -O0 -o bookstore_server;./bookstore_server

// 写完hpp，准备好各个对象之后，就开始组合，做成一个完整的程序
#include <iostream>
#include <future>

#include "cpplang.hpp"
#include "summary.hpp"
#include "myzmq.hpp"
#include "config.hpp"
#include "spin_lock.hpp"
#include "nlohmann/json.hpp" // brew install nlohmann-json

using namespace std;
using namespace cpp_study;
using json_t = nlohmann::json;

static auto debug_print = [](SalesData& b) {
    json_t j;

    j["id"] = b.id();
    j["sold"] = b.sold();
    j["revenue"] = b.revenue();

    cout << j.dump(4) << endl; // indent = 4
};

int main() {
try {
    cout << "Hello, I am a Book Store Server!" << endl;
    
    Config conf;
    conf.load("conf.lua");
    cout << conf.get<string>("config.zmq_ipc_addr") << endl;

    Summary sum;
    std::atomic_int count{0};

    //TODO: try-catch
    auto recv_cycle = [&]() {  // 主循环lambda,捕获所有变量
        using zmq_ctx = ZMQContext<1>; // 单线程接收客户端消息
        auto sock = zmq_ctx::recv_sock(); // 自动类型推导，获取接收socket

        sock.bind(conf.get<string>("config.zmq_ipc_addr")); // 绑定zmq端口
        assert(sock.connected());

        while (true) {
            auto msg_ptr = std::make_shared<zmq_message_t>(); // msg自动指针
            cout << "wait message" << endl;
            sock.recv(msg_ptr.get()); // ZMQ 阻塞式接收数据
            cout << "get a message" << endl;

            ++count; // 原子计数+1
            printf("message count = %d", static_cast<int>(count));

            // asyn process msg
            std::thread([msg_ptr, &sum]() { // 新起一个线程异步处理消息,这里用值捕获智能指针，否则可能会引用失效
                SalesData b;
                msgpack::object_handle oh = msgpack::unpack(static_cast<const char*>(msg_ptr->data()), msg_ptr->size());
                oh.get().convert(b);

                sum.add_sales(b);
                debug_print(b);
            }).detach(); // aysnc
        } // while
    }; // recv_cyle lambda

    auto log_cycle = [&]() {
        auto time_interval = conf.get<int>("config.time_interval");
        cout << "time_interval " << time_interval << endl;

        for (;;) {
            std::this_thread::sleep_for(std::chrono::seconds(time_interval));

            json_t j;
            j["count"] = static_cast<int>(count);
            j["minmax"] = sum.minmax_sales();

            cout << j.dump(4) << endl;
        }
    }; // log_cycle lambda

    // launch log_cycle
    auto t1 = std::async(std::launch::async, log_cycle); // include future

    // launch recv_cycle then wait
    auto t2 = std::async(std::launch::async, recv_cycle);

    t2.wait();
} // try

catch(std::exception& e) {
    cerr << e.what() << endl;
}

}