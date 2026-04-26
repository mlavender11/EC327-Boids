#include "Application.h"
#include "PathResolver.h"

int main()
{
    // Call the path resolver before starting the engine
    PathResolver::Initialize();

    Application app;
    app.Run();

    return 0;
}