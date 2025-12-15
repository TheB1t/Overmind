#include <overmind/common/host.hpp>
#include <overmind/common/runtime.hpp>
#include <overmind/common/module.hpp>
#include <log.hpp>

#include <random>

namespace overmind {

    static uint64_t rd64() {
        std::random_device rd;
        uint64_t a = ((uint64_t)rd() << 32) ^ (uint64_t)rd();
        uint64_t b = ((uint64_t)rd() << 32) ^ (uint64_t)rd();
        return a ^ (b + 0x9e3779b97f4a7c15ull);
    }

    uint64_t Host::make_node_seed() const { return rd64(); }

    Host::Host(std::string load_path)
        : load_path_(std::move(load_path)), idgen_(make_node_seed())
    {
        LOG_SET_LOG_FILE(load_path_ + "runtime.log");
        LOG_INFO("Loaded from: %s\n", load_path_.c_str());
    }

    void Host::register_callback(arma3_ext_cb_t cb) {
        cb_ = cb;
    }

    void Host::process_context(const char* const* argv, int argc) {
        ctx_.update(argv, argc);
    }

    int Host::process_call(char* out, int outSize, const char* func, const char* const* argv, int argc) {
        InboundCall call;
        call.id   = new_id();
        call.func = (func ? func : "");

        if (argc > 0 && argv) {
            call.args.reserve((size_t)argc);
            for (int i = 0; i < argc; ++i)
                call.args.emplace_back(argv[i] ? argv[i] : "");
        }

        in_.push(std::move(call));

        if (out && outSize > 0) {
            auto s = (std::string)(in_.size_approx() ? call.id : MessageId{0,0});
            std::snprintf(out, outSize, "OK %s", s.c_str());
        }
        return 0;
    }

    void Host::push_outbound(OutboundEvent&& ev) {
        out_.push(std::move(ev));
    }

    size_t Host::pop_inbound_batch(std::vector<InboundCall>& out) {
        return in_.pop_all(out);
    }

    size_t Host::pop_outbound_batch(std::vector<OutboundEvent>& out) {
        return out_.pop_all(out);
    }

    void Host::send_callback(const OutboundEvent& ev) {
        if (!cb_)
            return;

        cb_("overmind", "ExtensionCallback", ev.payload.c_str());
    }

    void Host::start() {
        if (running_)
            return;

        rt_ = std::make_unique<Runtime>(*this);
        mod_ = create_module();
        if (mod_)
            mod_->init(*this, *rt_);

        running_ = true;
    }

    void Host::stop() {
        if (!running_)
            return;

        if (rt_)
            rt_->request_stop();

        if (mod_)
            mod_->deinit(*this, *rt_);

        if (rt_)
            rt_->join_all();

        mod_.reset();
        rt_.reset();
        running_ = false;
    }
} // namespace overmind
