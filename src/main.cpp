#include "Application.h"
#include "PathResolver.h"

int main()
{
    // Call the path resolver before starting the engine
    SetupWorkingDirectory();

    Application app;
    app.Run();

    return 0;
}