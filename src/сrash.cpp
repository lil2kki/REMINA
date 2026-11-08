#include <.hpp>
// even having all this inlcudes in geode im not sure about non-windows ones
#include <signal.h>
#ifdef GEODE_IS_WINDOWS
#include <windows.h>
#include <dbghelp.h>
#elif defined(GEODE_IS_MACOS) || defined(GEODE_IS_IOS)
#include <execinfo.h>
#include <mach/mach.h>
#elif defined(GEODE_IS_ANDROID)
#include <unwind.h>
#include <dlfcn.h>
#endif

using namespace geode::prelude;

void saveGame(std::string message = "") {
    log::error("Detected the {}", message);
    log::info("Saving game...");
    AppDelegate::sharedApplication()->trySaveGame(false);
    log::info(" 1. Called virtual AppDelegate::trySaveGame");
    GameManager::get()->save();
    log::info(" 2. Called inherited GameManager::save");
    LocalLevelManager::get()->save();
    log::info(" 3. Called inherited LocalLevelManager::save");
    //FileSaveManager::get()->save(); //wooow hey aa what is that thingggg
    if (auto editor = GameManager::sharedState()->m_levelEditorLayer; editor) {
        if (auto pause = EditorPauseLayer::create(editor)) {
            pause->saveLevel();
            log::info(" '4. Saved level for current editor!");
        }
    }
}

#ifdef GEODE_IS_WINDOWS
inline LONG WINAPI windowsExceptionHandler(EXCEPTION_POINTERS* exceptionInfo) {
    std::string crashInfo = fmt::format(
        "Windows Exception: Code 0x{:X} at address 0x{:X}",
        exceptionInfo->ExceptionRecord->ExceptionCode,
        reinterpret_cast<uintptr_t>(exceptionInfo->ExceptionRecord->ExceptionAddress)
    );

    saveGame(crashInfo);
    return EXCEPTION_EXECUTE_HANDLER;
}

inline void signalHandler(int signal) {
    std::string crashInfo = fmt::format("Signal received: {}", signal);
    saveGame(crashInfo);
}

inline void setupWindowsHandlers() {
    // structured exception handling (might be overtaken...)
    SetUnhandledExceptionFilter(windowsExceptionHandler);
    // signal handlers
    signal(SIGSEGV, signalHandler);
    signal(SIGABRT, signalHandler);
    signal(SIGFPE, signalHandler);
    signal(SIGILL, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);
}

#elif defined(GEODE_IS_MACOS) || defined(GEODE_IS_IOS)
inline void unixSignalHandler(int signal, siginfo_t* info, void* context) {
    std::string crashInfo;

    switch (signal) {
    case SIGSEGV:
        crashInfo = fmt::format(
            "Segmentation fault at address 0x{:X}",
            reinterpret_cast<uintptr_t>(info->si_addr)
        );
        break;
    case SIGBUS:
        crashInfo = "Bus error";
        break;
    case SIGFPE:
        crashInfo = "Floating point exception";
        break;
    case SIGILL:
        crashInfo = "Illegal instruction";
        break;
    default:
        crashInfo = fmt::format("Unix signal {}", signal);
        break;
    }

    // get stack trace
    void* callstack[128];
    int frames = backtrace(callstack, 128);
    char** symbols = backtrace_symbols(callstack, frames);

    if (symbols) {
        crashInfo += "\nStack trace:\n";
        for (int i = 0; i < frames; ++i) {
            crashInfo += fmt::format("  {}: {}\n", i, symbols[i]);
        }
        free(symbols);
    }

    saveGame(crashInfo);
}

inline void setupUnixHandlers() {
    struct sigaction sa;
    sa.sa_sigaction = unixSignalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;

    sigaction(SIGSEGV, &sa, nullptr);
    sigaction(SIGBUS, &sa, nullptr);
    sigaction(SIGFPE, &sa, nullptr);
    sigaction(SIGILL, &sa, nullptr);
}

#elif defined(GEODE_IS_ANDROID)
inline void androidSignalHandler(int signal, siginfo_t* info, void* context) {
    std::string crashInfo = fmt::format("Android signal {} at address 0x{:X}",
        signal, reinterpret_cast<uintptr_t>(info->si_addr));

    saveGame(crashInfo);
    std::abort();
}

inline void setupAndroidHandlers() {
    struct sigaction sa;
    sa.sa_sigaction = androidSignalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_SIGINFO;

    sigaction(SIGSEGV, &sa, nullptr);
    sigaction(SIGBUS, &sa, nullptr);
    sigaction(SIGFPE, &sa, nullptr);
    sigaction(SIGILL, &sa, nullptr);
}
#endif

void ModLoaded();
$on_mod(Loaded) { ModLoaded(); }
inline void ModLoaded() {
#ifdef GEODE_IS_WINDOWS
    setupWindowsHandlers();
#elif defined(GEODE_IS_MACOS) || defined(GEODE_IS_IOS)
    setupUnixHandlers();
#elif defined(GEODE_IS_ANDROID)
    setupAndroidHandlers();
#endif
}