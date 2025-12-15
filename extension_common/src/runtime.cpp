#include <overmind/common/runtime.hpp>
#include <overmind/common/host.hpp>
#include <log.hpp>

namespace overmind {

    void Runtime::request_stop() {
        stop_.store(true, std::memory_order_relaxed);
    }

    void Runtime::spawn_impl(std::string name, std::function<void()>&& body) {
        auto& slot = threads_[name];
        if (slot.th.joinable()) {
            LOG_INFO("Thread %s already running; join before respawn\n", name.c_str());
            std::terminate();
        }
        slot.th = std::thread([name = std::move(name), body = std::move(body)]() mutable {
            body();
        });
    }

    void Runtime::spawn_poll(std::string name, PollFunc fn) {
        spawn_impl(std::move(name), [this, fn = std::move(fn)] {
            Backoff bo;
            while (!stopping()) {
                try {
                    bool didWork = fn(host_);
                    if (didWork) bo.reset();
                    else bo.idle();
                } catch (const std::exception& e) {
                    LOG_INFO("poll worker exception: %s\n", e.what());
                    bo.idle();
                } catch (...) {
                    LOG_INFO("poll worker unknown exception\n");
                    bo.idle();
                }
            }
        });
    }

    void Runtime::spawn_blocking(std::string name, BlockingFunc fn) {
        spawn_impl(std::move(name), [this, fn = std::move(fn)] {
            while (!stopping()) {
                try {
                    fn(host_);
                } catch (const std::exception& e) {
                    LOG_INFO("blocking worker exception: %s\n", e.what());
                } catch (...) {
                    LOG_INFO("blocking worker unknown exception\n");
                }
            }
        });
    }

    void Runtime::spawn_tick(std::string name, std::chrono::milliseconds period, TickFunc fn) {
        spawn_impl(std::move(name), [this, period, fn = std::move(fn)] {
            auto next = std::chrono::steady_clock::now();
            while (!stopping()) {
                next += period;
                try { fn(host_); } catch (...) {}
                std::this_thread::sleep_until(next);
            }
        });
    }

    void Runtime::join_all() {
        for (auto& [name, slot] : threads_) {
            if (slot.th.joinable()) slot.th.join();
        }
        threads_.clear();
    }
} // namespace overmind
