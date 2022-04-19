#ifndef COMPUTER_ANIMATION_SKELETON_H
#define COMPUTER_ANIMATION_SKELETON_H

#include <vector>

namespace ca {

    class SkeletonNode {
    public:
        std::vector<SkeletonNode*> children;
        SkeletonNode* parent;
    };

    class Skeleton {
    public:
        SkeletonNode* root;
    };
}
#endif // !COMPUTER_ANIMATION_SKELETON_H
