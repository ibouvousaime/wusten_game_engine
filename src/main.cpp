#include "render.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}




int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    int height = 1024;
    int width = 1024;
    GLFWwindow* window = glfwCreateWindow(width, height, "Wusten Engine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    } 

    glViewport(0, 0, height, width);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    auto scene = initTestScene(width, height);
    glfwSetWindowUserPointer(window, &scene->cameras[0]);
    glfwSetCursorPosCallback(window, mouseCallback);

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }
    const int fontSize = 32;
    FontRenderingData frd = setupDebugTextRendering(ft, fontSize);

    double lastTime = glfwGetTime();
    int frameCount = 0;
    RenderDebugData renderData = RenderDebugData();

    while (!glfwWindowShouldClose(window))
    {
        frameCount++;
        glfwGetWindowSize(window, &width, &height);
        scene->cameras[0].height = height;
        scene->cameras[0].width = width;
        scene->cameras[0].calculateMatrix();
        processInput(window, &scene->cameras[0]);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderScene(*scene, scene->cameras[0]);
        renderDebugText(&frd, width, height, fontSize, renderData);

        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        if (deltaTime >= 1.0) {
            renderData.fpsCounter = frameCount / deltaTime;
            frameCount = 0;
            lastTime = currentTime;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
