#pragma once

#include <atomic>
#include <functional>
#include <map>
#include <string>
#include <thread>
#include <chrono>

namespace overmind {

    class Backoff {
        public:
            void reset() { step_ = 0; }

            void idle() {
                if (step_ < 10) {
                    ++step_;
                    std::this_thread::yield();
                } else if (step_ < 20) {
                    ++step_;
                    std::this_thread::sleep_for(std::chrono::microseconds(50));
                } else {
                    std::this_thread::sleep_for(std::chrono::microseconds(200));
                }
            }

        private:
            int step_ = 0;
    };

    class Host;

    class Runtime {
        public:
            using PollFunc     = std::function<bool(Host&)>;
            using BlockingFunc = std::function<void(Host&)>;
            using TickFunc     = std::function<void(Host&)>;

            explicit Runtime(Host& host) : host_(host) {}

            void request_stop();
            bool stopping() const { return stop_.load(std::memory_order_relaxed); }

            void spawn_poll(std::string name, PollFunc fn);
            void spawn_blocking(std::string name, BlockingFunc fn);
            void spawn_tick(std::string name, std::chrono::milliseconds period, TickFunc fn);

            void join_all();

        private:
            struct Slot {
                std::thread th;
            };

            Host& host_;
            std::atomic<bool> stop_{false};
            std::map<std::string, Slot> threads_;

            void spawn_impl(std::string name, std::function<void()>&& body);
    };

} // namespace overmind
