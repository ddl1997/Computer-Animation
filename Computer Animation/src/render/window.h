#ifndef WINDOW_H
#define WINDOw_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window();

    void render();

    void close();

private:
    GLFWwindow* window;

    void processInput(GLFWwindow* window);
};
#endif