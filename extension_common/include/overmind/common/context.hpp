#pragma once

#include <mutex>
#include <string>
#include <vector>

namespace overmind {

    class ContextSnapshot {
        public:
            std::vector<std::string> args;
    };

    class Context {
        public:
            void update(const char* const* argv, int argc) {
                std::lock_guard<std::mutex> lk(m_);
                args_.clear();
                if (argc <= 0 || argv == nullptr) return;
                args_.reserve((size_t)argc);
                for (int i = 0; i < argc; ++i)
                    args_.emplace_back(argv[i] ? argv[i] : "");
            }
            ContextSnapshot snapshot() const {
                std::lock_guard<std::mutex> lk(m_);
                return ContextSnapshot{ args_ };
            }

        private:
            mutable std::mutex m_;
            std::vector<std::string> args_;
    };
} // namespace overmind