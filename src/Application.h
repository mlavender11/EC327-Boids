#ifndef APPLICATION_H
#define APPLICATION_H

#include "visual/GraphicsEngine.h"

class Application
{
public:
    Application();
    void Run();

private:
    GraphicsEngine graphics;
    std::vector<glm::mat4> dummyBoidData;
};

#endif