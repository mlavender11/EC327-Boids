#ifndef PATH_RESOLVER_H
#define PATH_RESOLVER_H

#include <string>

class PathResolver
{
public:
    // Call this once at start of main()
    static void Initialize();

private:
    static std::string GetExecutableDir();
};

#endif