#include "PathResolver.h"
#include <filesystem>
#include <iostream>

// Include OS-specific headers
#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#elif defined(__linux__)
#include <unistd.h>
#include <limits.h>
#endif

std::string PathResolver::GetExecutableDir()
{
    std::filesystem::path execDirPath;

#if defined(_WIN32)
    // --- WINDOWS LOGIC ---
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    execDirPath = std::filesystem::path(path).parent_path();

#elif defined(__APPLE__)
    // --- MACOS LOGIC ---
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0)
    {
        execDirPath = std::filesystem::path(path).parent_path();
    }

#elif defined(__linux__)
    // --- LINUX LOGIC ---
    char path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
    if (count != -1)
    {
        path[count] = '\0';
        execDirPath = std::filesystem::path(path).parent_path();
    }
#else
    // Fallback if OS is unknown
    execDirPath = std::filesystem::current_path();
#endif

    return execDirPath.string();
}

void PathResolver::Initialize()
{
    std::filesystem::path basePath = GetExecutableDir();

#if defined(__APPLE__)
    // If running inside a macOS .app bundle, step over to the Resources folder
    std::string baseStr = basePath.string();
    if (baseStr.find("Contents/MacOS") != std::string::npos)
    {
        basePath = basePath.parent_path() / "Resources";
    }
#endif

    // Change the Global Current Working Directory
    std::error_code ec;
    std::filesystem::current_path(basePath, ec);

    if (ec)
    {
        std::cerr << "Warning: Failed to set working directory: " << ec.message() << std::endl;
    }
    else
    {
        std::cout << "Working directory successfully set to: " << std::filesystem::current_path() << std::endl;
    }
}