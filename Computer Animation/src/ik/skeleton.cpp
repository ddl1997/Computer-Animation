#include "skeleton.h"
#include "global.h"
//#include "stdio.h"

bool ca::Skeleton::insertNode(SkeletonNode* parent, SkeletonNode* node, bool isEndEffector, SkeletonNode* rootNode)
{
    SkeletonNode* curNode = (rootNode == nullptr ? root : rootNode);
    if (parent == curNode)
    {
        int index = curNode->children.size();
        curNode->children.push_back(node);
        curNode->rotations[node] = 0;
        node->parent = curNode;
        if (isEndEffector)
        {
            endEffector = node;
        }
        return true;
    }
    else
    {
        for (auto nodeIter = curNode->children.begin(); nodeIter != curNode->children.end(); nodeIter++)
        {
            if (insertNode(parent, node, isEndEffector, *nodeIter))
                return true;
        }
    }
    return false;
}

bool ca::Skeleton::computeWithCCD(Eigen::Vector3f target)
{
    if (endEffector == root)
    {
        return endEffector->position == target;
    }

    int cyclicTimes = 0;
    Eigen::Vector3f curEndPos = endEffector->position;
    while (cyclicTimes < MAX_CCD_CYCLIC_TIMES && target != curEndPos)
    {
        SkeletonNode *curNode = endEffector->parent,
            *child = endEffector;
        
        while (curNode != nullptr)
        {
            curEndPos = endEffector->position;
            Eigen::Vector3f direction = curEndPos - curNode->position;
            Eigen::Vector3f targetDirection = target - curNode->position;

            Eigen::Vector3f rotAxis = direction.cross(targetDirection);
            float dirLength = std::sqrt(direction.dot(direction)), targetDirLength = std::sqrt(targetDirection.dot(targetDirection));
            float cosAngle = direction.dot(targetDirection) / (dirLength * targetDirLength);
            float rotAngle = std::acosf(cosAngle);
            /*float sinHalfAngle = std::sinf(rotAngle / 2), cosHalfAngle = std::cosf(rotAngle / 2);
            Eigen::Quaternionf localRot(cosHalfAngle, sinHalfAngle * rotAxis.x(), sinHalfAngle * rotAxis.y(), sinHalfAngle * rotAxis.z());*/
            if (rotAxis.z() < 0) rotAngle *= -1;
            curNode->rotations[child] += rotAngle;

            updatePosition(curNode, child, rotAngle);

            child = curNode;
            curNode = child->parent;

            //printf("%d\n", cyclicTimes);
        }
        printf("%f %f %f\n", endEffector->position.x(), endEffector->position.y(), endEffector->position.z());
        cyclicTimes++;

        Eigen::Vector3f errorVector = target - endEffector->position;
        float error = std::sqrt(errorVector.dot(errorVector));
        if (error < CCD_ERROR)
            return true;
    }

    return false;
}

void ca::Skeleton::updatePosition(SkeletonNode* parent, SkeletonNode* child, float angle)
{
    Eigen::Matrix4f trans, inTrans;
    trans << 1, 0, 0, -parent->position.x(),
        0, 1, 0, -parent->position.y(),
        0, 0, 1, -parent->position.z(),
        0, 0, 0, 1;
    inTrans << 1, 0, 0, parent->position.x(),
        0, 1, 0, parent->position.y(),
        0, 0, 1, parent->position.z(),
        0, 0, 0, 1;
    Eigen::Vector4f childPos = Eigen::Vector4f(child->position.x(), child->position.y(), child->position.z(), 1);
    //Eigen::Matrix3f rot = node->rotations[(*nodeIter)].normalized().toRotationMatrix();
    float sinRotAngle = std::sinf(angle), cosRotAngle = std::cosf(angle);
    Eigen::Matrix4f rot4d;
    rot4d << cosRotAngle, -sinRotAngle, 0, 0,
        sinRotAngle, cosRotAngle, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 1;
    childPos = inTrans * rot4d * trans * childPos;
    child->position = Eigen::Vector3f(childPos.x(), childPos.y(), childPos.z());
    for (auto nodeIter = child->children.begin(); nodeIter != child->children.end(); nodeIter++)
    {
        updatePosition(parent, *nodeIter, angle);
    }
}
