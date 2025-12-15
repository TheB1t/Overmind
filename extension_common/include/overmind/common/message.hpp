#pragma once

#include <atomic>
#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

namespace overmind {

    struct MessageId {
        uint64_t node = 0;
        uint64_t seq  = 0;

            std::string to_string() const {
                std::ostringstream oss;
                oss << std::hex << std::setfill('0')
                    << std::setw(16) << node
                    << "-"
                    << std::setw(16) << seq;
                return oss.str();
            }

            operator std::string() const {
                return to_string();
            }
    };

    class MessageIdGen {
        public:
            MessageIdGen(uint64_t node_seed) : node_(node_seed) {}
            MessageId next() {
                return MessageId{ node_, seq_.fetch_add(1, std::memory_order_relaxed) + 1 };
            }

        private:
            uint64_t node_;
            std::atomic<uint64_t> seq_{0};
    };

    struct InboundCall {
        MessageId id;
        std::string func;
        std::vector<std::string> args;
    };

    struct OutboundEvent {
        MessageId corr;
        std::string payload;
    };
} // namespace overmind
