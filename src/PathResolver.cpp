#include "PathResolver.h"
#include <filesystem>
#include <iostream>

#ifdef __APPLE__
#include <mach-o/dyld.h>
#endif

void SetupWorkingDirectory()
{
#ifdef __APPLE__
    char path[1024];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0)
    {
        std::filesystem::path execPath(path);

        // Scenario A: We are inside a .app bundle!
        std::filesystem::path bundleResources = execPath.parent_path().parent_path() / "Resources";

        if (std::filesystem::exists(bundleResources))
        {
            std::filesystem::current_path(bundleResources);
            std::cout << "🍎 Running inside .app bundle. Path set to Resources." << std::endl;
            return;
        }

        // Scenario B: We are running from the Terminal build folder!
        std::filesystem::path projectRoot = execPath.parent_path().parent_path();
        if (std::filesystem::exists(projectRoot / "assets"))
        {
            std::filesystem::current_path(projectRoot);
            std::cout << "💻 Running from build folder. Path set to Project Root." << std::endl;
        }
    }
#endif
}