#include "include/Engine.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



#include <Engine.h>

int main()
{
    Engine engine;
    engine.init();
    
  
               
    // render loop
    // -----------
    while (!glfwWindowShouldClose(engine.getWindow()))
    {
        engine.update();
        engine.render();

    }

        return 0;
}

