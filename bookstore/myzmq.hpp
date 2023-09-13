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
// Source: https://github.com/chronolaw/cpp_study

#ifndef MYZMQ_HPP
#define MYZMQ_HPP

#include "cpplang.hpp"
#include <zmq.hpp> //  brew search cppzmq, don't forget to link zmq, source from https://github.com/zeromq/cppzmq

namespace cpp_study {

using zmq_context_t = zmq::context_t;
using zmq_socket_t = zmq::socket_t;
using zmq_message_t = zmq::message_t;


template<int thread_num = 1>
class ZMQContext final {
public:
    ZMQContext() = default;
    ~ZMQContext() = default;
public:
    // static member function replace static member variable so that we can use it in hpp file
    static zmq_context_t& context() noexcept {
        static zmq_context_t ctx(thread_num);
        return ctx;
    }
public:
    static zmq_socket_t recv_sock(int hwm = 1000, int linger = 10) {
        zmq_socket_t sock(context(), ZMQ_PULL);
        sock.setsockopt(ZMQ_RCVHWM, hwm);
        sock.setsockopt(ZMQ_LINGER, linger); // wait for 10ms

        return sock;
    }
    static zmq_socket_t send_sock(int hwm = 1000, int linger = 10) {
        zmq_socket_t sock(context(), ZMQ_PUSH);
        sock.setsockopt(ZMQ_SNDHWM, hwm);
        sock.setsockopt(ZMQ_LINGER, linger); // wait for 10ms

        return sock;
    }
};

} // namespace cpp_study

#endif // MYZMQ_HPP