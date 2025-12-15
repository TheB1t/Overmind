#include <overmind/common/module.hpp>
#include <overmind/common/host.hpp>
#include <overmind/common/runtime.hpp>
#include <log.hpp>

#include <vector>

using namespace overmind;

namespace {

    class ServerModule final : public IModule {
        public:
            void init(Host& host, Runtime& rt) override {
                rt.spawn_poll("dispatcher", [&](Host& h) -> bool {
                    static thread_local std::vector<InboundCall> batch;
                    size_t n = h.pop_inbound_batch(batch);
                    if (n == 0) return false;

                    for (auto& c : batch) {
                        (void)c;
                    }
                    return true;
                });

                rt.spawn_tick("callback_pump", std::chrono::milliseconds(5), [&](Host& h) {
                    static thread_local std::vector<OutboundEvent> out;
                    if (h.pop_outbound_batch(out) == 0) return;
                    for (auto& ev : out) h.send_callback(ev);
                });

                // rt.spawn_blocking("net_rx", ...)

                LOG_INFO("ServerModule initialized\n");
            }

            void deinit(Host& host, Runtime& rt) override {
                (void)host; (void)rt;
                LOG_INFO("ServerModule deinit\n");
            }
    };

} // namespace

std::unique_ptr<IModule> overmind::create_module() {
    return std::make_unique<ServerModule>();
}
