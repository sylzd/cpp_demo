-- Copyright 2023 lzd
-- 
-- Licensed under the Apache License, Version 2.0 (the "License");
-- you may not use this file except in compliance with the License.
-- You may obtain a copy of the License at
-- 
--     http://www.apache.org/licenses/LICENSE-2.0
-- 
-- Unless required by applicable law or agreed to in writing, software
-- distributed under the License is distributed on an "AS IS" BASIS,
-- WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
-- See the License for the specific language governing permissions and
-- limitations under the License.

-- lua is more flexible than json
-- you can use comment/expression
-- and more lua pragram

config = {
    -- should be same as client
    -- you could change it to ipc    
    zmq_ipc = "tcp://127.0.0.1:5555",

    -- see http_study's lu code
    http_addr = "http://localhost/cpp_study?token=test",
    time_internal = 5, -- seconds
    keyword = "super",
    sold_criteria = 100,
    revenue_criteria = 1000,
    best_n = 3,
    max_buf_size = 4 * 1024,

}

others = {
    -- add more
}