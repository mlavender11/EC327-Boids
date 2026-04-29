#include "Application.h"
#include "PathResolver.h"

int main()
{
    // Find where it is before running
    PathResolver::Initialize();

    Application app;
    app.Run();

    return 0;
}
