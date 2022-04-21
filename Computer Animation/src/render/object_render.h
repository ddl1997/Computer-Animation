#ifndef OBJECT_RENDER_H
#define OBJECT_RENDER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class ObjectRender {
public:

    virtual void render(GLFWwindow* window) = 0;
};
#endif // !OBJECT_RENDER_H
