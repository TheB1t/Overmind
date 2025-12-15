#pragma once

#include <cstddef>

#if defined(___WINDOWS___)
    #define EXPORT __declspec(dllexport)
    #define STDCALL __stdcall
#elif defined(___UNIX___)
    #define EXPORT
    #define STDCALL
#else
#error "This platform is not supported"
#endif

extern "C" {
    EXPORT void STDCALL RVExtension(char* output, int outputSize, const char* function);
    EXPORT int  STDCALL RVExtensionArgs(char* output, int outputSize, const char* function, const char** argv, int argc);
    EXPORT void STDCALL RVExtensionVersion(char* output, int outputSize);
	EXPORT void STDCALL RVExtensionRegisterCallback(int(*callbackProc)(char const *name, char const *function, char const *data));
	EXPORT void STDCALL RVExtensionContext(const char** argv, int argc);
}