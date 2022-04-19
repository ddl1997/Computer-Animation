#include "skeleton_render.h"

void SkeletonRender::render()
{
    computeSkeletonPosition();
}

void SkeletonRender::computeSkeletonPosition()
{
    ca::SkeletonNode* curNode = skeleton->root;
    rotateSkeletonNode(curNode);
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
            Eigen::Matrix3f rot = node->rotations[(*nodeIter)].normalized().toRotationMatrix();
            Eigen::Matrix4f rot4d;
            rot4d << rot.row(0).x(), rot.row(0).y(), rot.row(0).z(), 0,
                rot.row(1).x(), rot.row(1).y(), rot.row(1).z(), 0,
                rot.row(2).x(), rot.row(2).y(), rot.row(2).z(), 0,
                0, 0, 0, 1;
            childPos = inTrans * rot4d * trans * childPos;
            (*nodeIter)->position = Eigen::Vector3f(childPos.x(), childPos.y(), childPos.z());
            rotateSkeletonNode(*nodeIter);
        }
    }
}
