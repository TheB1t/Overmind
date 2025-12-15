#pragma once

#include <cstdint>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include <overmind/common/message.hpp>
#include <overmind/common/queue_swap.hpp>
#include <overmind/common/context.hpp>
#include <overmind/common/runtime.hpp>
#include <overmind/common/module.hpp>

namespace overmind {

    using arma3_ext_cb_t = int(*)(char const* name, char const* function, char const* data);

    class Host {
        public:
            explicit Host(std::string load_path);

            void register_callback(arma3_ext_cb_t cb);
            void process_context(const char* const* argv, int argc);

            int process_call(char* out, int outSize, const char* func, const char* const* argv, int argc);

            void push_outbound(OutboundEvent&& ev);
            size_t pop_inbound_batch(std::vector<InboundCall>& out);
            size_t pop_outbound_batch(std::vector<OutboundEvent>& out);

            ContextSnapshot context_snapshot() const { return ctx_.snapshot(); }

            void start();
            void stop();
            bool running() const { return running_; }

            MessageId new_id() { return idgen_.next(); }

            void send_callback(const OutboundEvent& ev);

            const std::string& load_path() const { return load_path_; }

        private:
            std::string load_path_;
            arma3_ext_cb_t cb_ = nullptr;

            Context ctx_;
            MessageIdGen idgen_;

            SwapQueue<InboundCall>  in_;
            SwapQueue<OutboundEvent> out_;

            std::unique_ptr<Runtime> rt_;
            std::unique_ptr<IModule> mod_;
            bool running_ = false;

            uint64_t make_node_seed() const;
    };
} // namespace overmind
