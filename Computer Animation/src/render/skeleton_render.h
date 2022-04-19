#ifndef SKELETON_RENDER_H
#define SKELETON_RENDER_H

#include "object_render.h"
#include "../ik/skeleton.h"

class SkeletonRender : public ObjectRender {
public:

    SkeletonRender(ca::Skeleton* skeleton)
    {
        this->skeleton = skeleton;
    }

    void render();

private:
    ca::Skeleton* skeleton;

    void computeSkeletonPosition();

    void rotateSkeletonNode(ca::SkeletonNode* node);
};
#endif