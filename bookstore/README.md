<!--
 Copyright 2023 lzd
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
     http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
-->

## 运行方式一 ： 手动编译
1. Server：先启动server常驻进程
编译命令如下：
```
rm -f ./bookstore_server; g++ server.cpp -std=c++14 -I./LuaBridge/Source -I/usr/local/include/luajit-2.1 -lluajit-5.1 -ldl -lzmq -lpthread -lcurl -g -O0 -o bookstore_server;./bookstore_server
```

2. Client
再启动client，买几本书，观察server端输出
编译命令如下：
```

```

3. 观察Server日志
```
TODO，结果还有点问题，可能是msgpack这里的问题，需要调试下
```

## 运行方式二 ： CMake编译
```
mkdir -p ${workspaceFolder}/bookstore/build && rm -rf ${workspaceFolder}/bookstore/build/* && cd ${workspaceFolder}/bookstore/build && cmake -DCMAKE_BUILD_TYPE=Debug .. && cmake --build .
# 生成server和client两个二进制，分别执行
./build/server
./build/client
```