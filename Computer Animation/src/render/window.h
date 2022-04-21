#ifndef WINDOW_H
#define WINDOw_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "skeleton_render.h"

class Window {
public:

    Window();

    void render();

    void setMouseCallback(GLFWcursorposfun func)
    {
        glfwSetCursorPosCallback(window, func);
    }

    void setSkeletonRender(SkeletonRender* render)
    {
        skRender = render;
    }

    void close();

private:
    GLFWwindow* window;

    SkeletonRender* skRender;

    void processInput(GLFWwindow* window);

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {

    }
};
#endif