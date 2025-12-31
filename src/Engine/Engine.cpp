
// Engine.cpp
#include <Engine.h>
#include <iostream>
#include <texture.h>
#include <chunkData.h>

Engine::~Engine() { std::cout << "Engine destroyed\n";
    
  glfwTerminate();

}

 

void Engine::init() {

     glfwInit();
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  #ifdef __APPLE__
      glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif

      // glfw window creation
      // --------------------
       window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearOpenGL", NULL, NULL);
      if (window == NULL)
      {
          std::cout << "Failed to create GLFW window" << std::endl;
          glfwTerminate();
          return;
      }
      glfwMakeContextCurrent(window);
      glfwSetWindowUserPointer(window, this);
      glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
      glfwSetCursorPosCallback(window, mouse_callback);
      glfwSetScrollCallback(window, scroll_callback);


      // tell GLFW to capture our mouse
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

      // glad: load all OpenGL function pointers
      // ---------------------------------------
      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
      {
          std::cout << "Failed to initialize GLAD" << std::endl;
          return;
      }
       glEnable(GL_DEPTH_TEST);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

      
    // load and create a texture 
    // -------------------------
        // ---------
        // texture 2
    // ---------
    unsigned int dirtTex = loadTexture("assets/minecraft/textures/block/dirt.png");
    textures.push_back(dirtTex);
   ourShader = new Shader("assets/shaders/default.vs", "assets/shaders/default.fs");
   unlitShader = new Shader("assets/shaders/unlit.vs", "assets/shaders/unlit.fs");
   lightShader = new Shader("assets/shaders/lightcube.vs", "assets/shaders/lightcube.fs"); 

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    unlitShader->use();
    unlitShader->setInt("atlas", 0);


    camera = new Camera((glm::vec3(0.0f, 20.0f, 0.0f)));



   
    world.Generate();
    world.PrintChunks();
 }






void Engine::update() {

 // activate shader
  processInput(window);
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;  

        unlitShader->use();

        // pass projection matrix to shader (note that in this case it could change every frame)
         glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        unlitShader->setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera->GetViewMatrix();
        unlitShader->setMat4("view", view);


// DisplayFPS();  
}





void Engine::render() {



   glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        // bind textures on corresponding texture units
        world.Render(unlitShader, camera->Position);



  glfwSwapBuffers(window);
  glfwPollEvents();

}


void Engine::processInput(GLFWwindow *window)
{
    Engine* eng = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (!eng) return;


    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        eng->camera->ProcessKeyboard(FORWARD,eng->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
       eng->camera->ProcessKeyboard(BACKWARD, eng->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
       eng->camera->ProcessKeyboard(LEFT, eng->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        eng->camera->ProcessKeyboard(RIGHT, eng->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            eng->camera->ProcessKeyboard(DOWN, eng->deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            eng->camera->ProcessKeyboard(UP, eng->deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Engine::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Engine::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
  Engine* eng = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (!eng) return;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (eng->firstMouse) {
        eng->lastX = xpos;
        eng->lastY = ypos;
        eng->firstMouse = false;
    }

    float xoffset = xpos - eng->lastX;
    float yoffset = eng->lastY - ypos;

    eng->lastX = xpos;
    eng->lastY = ypos;

    eng->camera->ProcessMouseMovement(xoffset, yoffset);
}

void Engine::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    Engine* eng = static_cast<Engine*>(glfwGetWindowUserPointer(window));
    if (!eng) return;

    eng->camera->ProcessMouseScroll(static_cast<float>(yoffset));
}
