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
        curNode->rotations[node] = Eigen::Quaternionf(0, 0, 0, 1);
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
            Eigen::Vector3f direction = curEndPos - curNode->position;
            Eigen::Vector3f targetDirection = target - curNode->position;

            Eigen::Vector3f rotAxis = direction.cross(targetDirection).normalized();
            float dirLength = std::sqrt(direction.dot(direction)), targetDirLength = std::sqrt(targetDirection.dot(targetDirection));
            float cosAngle = direction.dot(targetDirection) / (dirLength * targetDirLength);
            float rotAngle = std::acosf(cosAngle);
            float sinHalfAngle = std::sinf(rotAngle / 2), cosHalfAngle = std::cosf(rotAngle / 2);
            Eigen::Quaternionf localRot(cosHalfAngle, sinHalfAngle * rotAxis.x(), sinHalfAngle * rotAxis.y(), sinHalfAngle * rotAxis.z());
            curNode->rotations[child] = localRot * curNode->rotations[child];

            curEndPos = curNode->position + targetDirection * dirLength / targetDirLength;
            printf("%f %f %f\n", curEndPos.x(), curEndPos.y(), curEndPos.z());

            child = curNode;
            curNode = child->parent;

            //printf("%d\n", cyclicTimes);
        }
        cyclicTimes++;

        Eigen::Vector3f errorVector = target - curEndPos;
        float error = std::sqrt(errorVector.dot(errorVector));
        if (error < CCD_ERROR)
            return true;
    }

    return false;
}
