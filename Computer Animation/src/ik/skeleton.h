#ifndef COMPUTER_ANIMATION_SKELETON_H
#define COMPUTER_ANIMATION_SKELETON_H

#include <vector>
#include <map>
#include <Eigen/Dense>

namespace ca {

    class SkeletonNode {
    public:
        std::vector<SkeletonNode*> children;
        std::map<SkeletonNode*, Eigen::Quaternionf> rotations;
        SkeletonNode* parent;
        Eigen::Vector3f position;

        int tag;

        SkeletonNode()
        {
            parent = nullptr;
            position = Eigen::Vector3f(0, 0, 0);
        }

        SkeletonNode(Eigen::Vector3f position)
        {
            parent = nullptr;
            this->position = position;
        }

        SkeletonNode(Eigen::Vector3f position, int tag)
        {
            parent = nullptr;
            this->position = position;
            this->tag = tag;
        }
    };

    class Skeleton {
    public:
        SkeletonNode* root;
        SkeletonNode* endEffector;

        Skeleton()
        {
            root = new SkeletonNode();
            endEffector = root;
        }

        Skeleton(SkeletonNode* root)
        {
            this->root = root;
            endEffector = root;
        }

        bool insertNode(SkeletonNode* parent, SkeletonNode* node, bool isEndEffector, SkeletonNode* rootNode = nullptr);

        bool computeWithCCD(Eigen::Vector3f target);
    };
}
#endif // !COMPUTER_ANIMATION_SKELETON_H
