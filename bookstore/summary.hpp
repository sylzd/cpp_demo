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

#ifndef SUMMARY_HPP
#define SUMMARY_HPP

#include "cpplang.hpp"
#include "sales_data.hpp"
#include "spin_lock.hpp"

namespace cpp_study {

class Summary final { // final: forbid inheritance
public:  // alias
    using this_t = Summary;

    using sales_data_t = SalesData;
    using lock_t = SpinLock;
    using lock_guard_t = SpinLockGuard;
    
    using string_t = std::string;
    using map_t = std::map<string_t, sales_data_t>; // or unordered_map
    using minmax_sales_t = std::pair<string_t, string_t>;
public: // 显式构造析构
    Summary() = default;
    ~Summary() = default;

    Summary(const this_t&) = delete;
    Summary& operator=(const this_t&) = delete;
private:
    mutable lock_t m_lock; // 自旋锁
    map_t m_sales; // 销售记录主要存储结构

public:
    void add_sales(const sales_data_t& sales) noexcept { // 非const
        lock_guard_t guard(m_lock); // 自动锁定，自动解锁
        const auto id = sales.id(); // 自动类型推导string_view_t

        // not found, 没找到记录就添加
        if (m_sales.find(id) == m_sales.end()) {
            m_sales[id] = sales;
        }
        // found, 找到记录就累加
        m_sales[id].inc_sold(sales.sold());
        m_sales[id].inc_revenue(sales.revenue());
    }

    minmax_sales_t minmax_sales() const { // const function
        lock_guard_t guard(m_lock);

        if (m_sales.empty()) {
            return minmax_sales_t("", "");
        }

        // algo by lambda
        auto ret = std::minmax_element(
            std::begin(m_sales), std::end(m_sales),
            // lambda
            [](
                // map 由 pair 组成, pair 的 first 是 key, second 是 value
                const std::pair<string_t, sales_data_t>& lhs, 
                const std::pair<string_t, sales_data_t>& rhs) {
                return lhs.second.sold() < rhs.second.sold();
            }
        );
        return {ret.first->second.id(), ret.second->second.id()};
    }

};

}

#endif // SUMMARY_HPP