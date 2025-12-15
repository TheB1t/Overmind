#pragma once

#include <mutex>
#include <vector>
#include <utility>

namespace overmind {
    template <class T>
    class SwapQueue {
        public:
            void push(T&& v) {
                std::lock_guard<std::mutex> lk(m_);
                buf_.push_back(std::move(v));
            }

            void push(const T& v) {
                std::lock_guard<std::mutex> lk(m_);
                buf_.push_back(v);
            }

            size_t pop_all(std::vector<T>& out) {
                std::lock_guard<std::mutex> lk(m_);
                if (buf_.empty())
                    return 0;
                out.clear();
                out.swap(buf_);
                return out.size();
            }

            size_t size_approx() const {
                std::lock_guard<std::mutex> lk(m_);
                return buf_.size();
            }

        private:
            mutable std::mutex m_;
            std::vector<T> buf_;
    };
} // namespace overmind
