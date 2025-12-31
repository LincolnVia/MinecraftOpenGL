#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <filesystem.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <world.h>
#include <shader.h>
#include <camera.h>
#include <iostream>
#include <chunkData.h>
#include <basicVerts.h>

#include <stb_image.h>



const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;




class Engine {


  private:

    GLFWwindow* window = nullptr;
     Camera* camera = nullptr;
     World world;
     Shader* ourShader = nullptr;
    Shader* unlitShader = nullptr;
    Shader* lightShader = nullptr; 
  
      std::vector<unsigned int> textures;

    // Chunk mesh storage
  

public:
    Engine()
      : world(textures) // initialize the member with the textures vector
    {
        std::cout << "Engine created\n";
    }

    ~Engine();

    void init();
    void update();
    void render();
    void DisplayFPS()
{
    static int frameCount = 0;
    static double lastTime = glfwGetTime();
    
    frameCount++;
    double currentTime = glfwGetTime();
    double elapsed = currentTime - lastTime;
    
    if (elapsed >= 1.0) // Update every second
    {
        double fps = frameCount / elapsed;
        printf("FPS: %.1f\n", fps);
        
        // Optional: Update window title
        std::string title = "Engine - FPS: " + std::to_string(static_cast<int>(fps));
        glfwSetWindowTitle(window, title.c_str());
        
        frameCount = 0;
        lastTime = currentTime;
    }
}
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void processInput(GLFWwindow *window);

    float deltaTime = 0.0f;	// time between current frame and last frame
    float lastFrame = 0.0f; 

    float lastX = SCR_WIDTH / 2.0f;
    float lastY = SCR_HEIGHT / 2.0f;
    bool firstMouse = true;


        int frameCount = 0;


    GLFWwindow* getWindow(){ return window;}
   
};





#endif













