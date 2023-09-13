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

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "cpplang.hpp"
#include <regex>

extern "C" {
#include <luajit.h> // brew install luajit
#include <lualib.h>
#include <lauxlib.h>
}

#include "LuaBridge/LuaBridge.h" // git clone https://github.com/vinniefalco/LuaBridge.git --depth=1

namespace cpp_study {

class Config final {
public:
    using vm_t = std::shared_ptr<lua_State>;
    using value_t = luabridge::LuaRef;

    using string_t = std::string;
    using string_view_t = const std::string&;
    using regex_t = std::regex;
    using match_t = std::smatch;
private:
    vm_t m_vm{luaL_newstate(), lua_close};
    const regex_t m_reg {R"(^(\w+)\.(\w+)$)"};
    //TODO: can use the nullptr as the initial value?
    mutable value_t m_conf {nullptr}; // mutable: can be modified in const function：可编程的配置文件

public:
    Config() noexcept {
        assert(m_vm!= nullptr);
        luaL_openlibs(m_vm.get());
    };
    ~Config() = default;

public:
    void load(string_view_t filename) const {
        auto status = luaL_dofile(m_vm.get(), filename.c_str());
        if (status != LUA_OK) {
            throw std::runtime_error(lua_tostring(m_vm.get(), -1));
        }
    }

    // template can explicitly be used by different type, such as string,int
    template<typename T>
    T get(string_view_t key) const {
        if (!std::regex_match(key, m_conf, m_reg)) {
            throw std::runtime_error("invalid key");
        }
        auto w1 = m_conf[1].cast<string_t>();
        auto w2 = m_conf[2].cast<string_t>();

        using namespace luabridge;

        auto v = getGlobal(vm_.get(), w1.c_str());

        return LuaRef_cast<T>(v[w2.c_str()]);
    }

};

}

#endif // CONFIG_HPP