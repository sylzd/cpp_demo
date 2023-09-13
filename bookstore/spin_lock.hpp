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

#ifndef SPIN_LOCK_HPP
#define SPIN_LOCK_HPP

#include <atomic>
#include <thread>
#include "cpplang.hpp"

namespace cpp_study {

class SpinLock final { // final: forbid inheritance
public:
    using this_type = SpinLock;
    using atomic_type = std::atomic_flag;
public:
    SpinLock() = default; // default constructor
    ~SpinLock() = default; // default destructor

    SpinLock(const this_type&) = delete; // forbid copy 
    SpinLock& operator=(const this_type&) = delete; // delete copy assignment operator
private:
    atomic_type m_lock = ATOMIC_FLAG_INIT; // init atomic_flag

public:
    void lock() noexcept { // noexcept: no exception
        while (m_lock.test_and_set(std::memory_order_acquire)) { // TAS: test and set, if lock is set, then spin
        }
        std::this_thread::yield(); // yield to other threads
    }
    
    void unlock() noexcept { // noexcept: no exception
        m_lock.clear(std::memory_order_release); // clear lock
    }
};

class SpinLockGuard final { // spin lock guard RAII, auto unlock
public:
    using this_type = SpinLockGuard;
    using spin_lock_type = SpinLock;

public:
    SpinLockGuard(const this_type&) = delete; // forbid copy
    SpinLockGuard& operator=(const this_type&) = delete; // delete copy assignment operator

private:
    spin_lock_type& m_lock; // reference to spin lock

public:
    explicit SpinLockGuard(spin_lock_type& lock) noexcept : m_lock(lock) { // explicit: forbid implicit conversion
        // noexcept: no exception
        // 初始化列表 initialization list: _lock(lock)
        m_lock.lock(); // lock
    }
    ~SpinLockGuard() noexcept { // noexcept: no exception
        m_lock.unlock(); // unlock
    }
};

}

#endif // SPIN_LOCK_HPP