#ifndef SKELETON_RENDER_H
#define SKELETON_RENDER_H

#include "object_render.h"
#include "../ik/skeleton.h"
#include "shader.h"

class SkeletonRender : public ObjectRender {
public:

    SkeletonRender()
    {
        shader = Shader("default.vert", "default.frag");
    }

    SkeletonRender(ca::Skeleton* skeleton)
    {
        this->skeleton = skeleton;
        shader = Shader("default.vert", "default.frag");
    }

    void render(GLFWwindow* window);

private:
    ca::Skeleton* skeleton;

    Shader shader;

    void getTriangles(std::vector<Eigen::Vector3f>& triangles, std::vector<GLuint>& indices);

    void generateTriangles(ca::SkeletonNode* node, std::vector<Eigen::Vector3f>& triangles, std::vector<GLuint>& indices);

    void computeSkeletonPosition();

    void rotateSkeletonNode(ca::SkeletonNode* node);
};
#endif