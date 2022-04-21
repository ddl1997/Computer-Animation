#include "skeleton_render.h"

void SkeletonRender::render(GLFWwindow* window)
{
    glfwMakeContextCurrent(window);
    std::vector<Eigen::Vector3f> triangles;
    std::vector<GLuint> indices;
    getTriangles(triangles, indices);

    /*std::vector<float> triangles(9);
    triangles[0] = 0.5;
    triangles[1] = 0.5;
    triangles[2] = 0;
    triangles[3] = 0.5;
    triangles[4] = -0.5;
    triangles[5] = 0;
    triangles[6] = -0.5;
    triangles[7] = -0.5;
    triangles[8] = 0;

    std::vector<unsigned int> indices(3);
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;*/

    //float triangles[] = {
    //0.5f, 0.5f, 0.0f,   // 右上角
    //0.5f, -0.5f, 0.0f,  // 右下角
    //-0.5f, -0.5f, 0.0f, // 左下角
    //};

    //unsigned int indices[] = { // 注意索引从0开始! 
    //    0, 1, 3, // 第一个三角形
    //};

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, triangles.size() * sizeof(float), &triangles[0], GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    shader.use();
    /*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void SkeletonRender::getTriangles(std::vector<Eigen::Vector3f>& triangles, std::vector<GLuint>& indices)
{
    generateTriangles(skeleton->root, triangles, indices);
}

void SkeletonRender::generateTriangles(ca::SkeletonNode* node, std::vector<Eigen::Vector3f>& triangles, std::vector<GLuint>& indices)
{
    for (auto child = node->children.begin(); child != node->children.end(); child++)
    {
        Eigen::Vector3f dir = (*child)->position - node->position;
        Eigen::Vector3f norm(-dir.y(), dir.x(), 0);
        int triSize = triangles.size();
        triangles.push_back(((*child)->position) / 2);
        triangles.push_back((node->position + norm.normalized()) / 2);
        triangles.push_back((node->position - norm.normalized()) / 2);
        indices.push_back(triSize);
        indices.push_back(triSize + 1);
        indices.push_back(triSize + 2);
        generateTriangles((*child), triangles, indices);
    }
}

void SkeletonRender::computeSkeletonPosition()
{
    ca::SkeletonNode* curNode = skeleton->root;

    //rotateSkeletonNode(curNode);
    printf("tag %d: %f, %f, %f\n", curNode->tag, curNode->position.x(), curNode->position.y(), curNode->position.z());
    curNode = curNode->children[0];
    printf("tag %d: %f, %f, %f\n", curNode->tag, curNode->position.x(), curNode->position.y(), curNode->position.z());
    curNode = curNode->children[0];
    printf("tag %d: %f, %f, %f\n", curNode->tag, curNode->position.x(), curNode->position.y(), curNode->position.z());
}

void SkeletonRender::rotateSkeletonNode(ca::SkeletonNode* node)
{
    if (node != nullptr)
    {
        Eigen::Matrix4f trans, inTrans;
        trans << 1, 0, 0, -node->position.x(),
            0, 1, 0, -node->position.y(),
            0, 0, 1, -node->position.z(),
            0, 0, 0, 1;
        inTrans << 1, 0, 0, node->position.x(),
            0, 1, 0, node->position.y(),
            0, 0, 1, node->position.z(),
            0, 0, 0, 1;
        for (auto nodeIter = node->children.begin(); nodeIter != node->children.end(); nodeIter++)
        {
            Eigen::Vector4f childPos = Eigen::Vector4f((*nodeIter)->position.x(), (*nodeIter)->position.y(), (*nodeIter)->position.z(), 1);
            //Eigen::Matrix3f rot = node->rotations[(*nodeIter)].normalized().toRotationMatrix();
            float sinRotAngle = std::sinf(node->rotations[(*nodeIter)]), cosRotAngle = std::cosf(node->rotations[(*nodeIter)]);
            Eigen::Matrix4f rot4d;
            rot4d << cosRotAngle, sinRotAngle, 0, 0,
                -sinRotAngle, cosRotAngle, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 1;
            childPos = inTrans * rot4d * trans * childPos;
            (*nodeIter)->position = Eigen::Vector3f(childPos.x(), childPos.y(), childPos.z());
            rotateSkeletonNode(*nodeIter);
        }
    }
}
