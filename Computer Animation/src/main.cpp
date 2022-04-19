#include <iostream>
#include "render/window.h"
#include "ik/skeleton.h"
#include "render/skeleton_render.h"

using namespace std;

int main()
{
    /*Window window;
    window.render();*/

    ca::SkeletonNode node1(Eigen::Vector3f(0, 0, 0), 0),
        node2(Eigen::Vector3f(1, 0, 0), 1),
        node3(Eigen::Vector3f(2, 0, 0), 2);
    ca::Skeleton sk(&node1);
    SkeletonRender render(&sk);
    sk.insertNode(&node1, &node2, false);
    sk.insertNode(&node2, &node3, true);

    sk.computeWithCCD(Eigen::Vector3f(1.5, 1, 0));

    render.render();
    /*Eigen::Quaternionf q(0, 0, 0, 1);
    auto curNode = sk.root;
    printf("tag %d: %f, %f, %f\n", curNode->tag, curNode->position.x(), curNode->position.y(), curNode->position.z());

    auto child1 = curNode->children[0];
    q = curNode->rotations[child1] * q;
    q.normalize();
    auto child1Pos = q.toRotationMatrix() * child1->position;
    printf("tag %d: %f, %f, %f\n", child1->tag, child1Pos.x(), child1Pos.y(), child1Pos.z());

    auto child2 = child1->children[0];
    q = child1->rotations[child2] * q;
    q.normalize();
    auto child2Pos = q.toRotationMatrix() * child2->position;
    printf("tag %d: %f, %f, %f\n", child2->tag, child2Pos.x(), child2Pos.y(), child2Pos.z());*/

    return 0;
}