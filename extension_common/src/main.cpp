#include <overmind/common/host.hpp>

#if defined(___WINDOWS___)
    #include <windows.h>
#elif defined(___UNIX___)
    #include <dlfcn.h>
#else
#error "This platform is not supported"
#endif

extern std::string full_path;

inline void mainInit(std::string path) {
    full_path = path;
}

inline void mainFree() {
    
}

#if defined(___WINDOWS___)
EXTERN_C IMAGE_DOS_HEADER __ImageBase;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH:
            char path[MAX_PATH + 1];
            GetModuleFileNameA((HINSTANCE)&__ImageBase, path, (MAX_PATH + 1));

            mainInit(std::string(path));
            break;

        case DLL_PROCESS_DETACH:
            mainFree();
            break;

        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            break;
    };

    return true;
}
#elif defined(___UNIX___)
__attribute__((constructor)) void load() {
    Dl_info dl_info;
	dladdr((void*)load, &dl_info);

    mainInit(std::string(dl_info.dli_fname));
}

__attribute__((destructor)) void free() {
    mainFree();
}
#else
#error "This platform is not supported"
#endif