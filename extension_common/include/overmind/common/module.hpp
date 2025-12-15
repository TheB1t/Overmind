#pragma once

#include <memory>

namespace overmind {
    class Host;
    class Runtime;

    struct IModule {
        virtual ~IModule() = default;
        virtual void init(Host& host, Runtime& rt) = 0;
        virtual void deinit(Host& host, Runtime& rt) = 0;
    };

    std::unique_ptr<IModule> create_module();
} // namespace overmind
