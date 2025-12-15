#include <arma_exports.hpp>
#include <overmind/common/host.hpp>
#include <log.hpp>

std::string full_path;

static overmind::Host& host_singleton() {
    size_t last_separator_pos = full_path.find_last_of("/\\");
    std::string path = full_path.substr(0, last_separator_pos + 1);

    static overmind::Host h{path};
    static bool started = false;
    if (!started) { 
        h.start();
        started = true;
    }
    return h;
}

extern "C" {

    void STDCALL RVExtensionVersion(char* output, int outputSize) {
        std::snprintf(output, outputSize, "overmind %d.%d", EXTENSION_VERSION_MAJOR, EXTENSION_VERSION_MINOR);
    }

    void STDCALL RVExtension(char* output, int outputSize, const char* function) {
        const char* argv[1] = { nullptr };
        RVExtensionArgs(output, outputSize, function, argv, 0);
    }

    int STDCALL RVExtensionArgs(char* output, int outputSize, const char* function, const char** argv, int argc) {
        auto& h = host_singleton();
        return h.process_call(output, outputSize, function, argv, argc);
    }

    void STDCALL RVExtensionRegisterCallback(int(*cb)(char const* name, char const* function, char const* data)) {
        host_singleton().register_callback(cb);
    }

    void STDCALL RVExtensionContext(const char** argv, int argc) {
        host_singleton().process_context(argv, argc);
    }
}
