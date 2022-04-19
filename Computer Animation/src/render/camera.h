#ifndef CAMERA_H
#define CAMERA_H

#include "Eigen/Dense"

class Camera {
public:
    Camera()
    {
        position = Eigen::Vector3f(0, 0, 1);
        target = Eigen::Vector3f(0, 0, 0);
        up = Eigen::Vector3f(0, 1, 0);
    }

    Camera(Eigen::Vector3f position, Eigen::Vector3f target, Eigen::Vector3f up = Eigen::Vector3f(0, 1, 0))
    {
        this->position = position;
        this->target = target;
        this->up = up;
    }

    Eigen::Matrix4f lookAt()
    {
        Eigen::Vector3f cameraDirection = (position - target).normalized();
        Eigen::Vector3f cameraRight = up.cross(cameraDirection).normalized();
        Eigen::Vector3f cameraUp = cameraDirection.cross(cameraRight).normalized();

        Eigen::Matrix4f trans1, trans2;
        trans1 << cameraRight.x(), cameraRight.y(), cameraRight.z(), 0,
            cameraUp.x(), cameraUp.y(), cameraUp.z(), 0,
            cameraDirection.x(), cameraDirection.y(), cameraDirection.z(), 0,
            0, 0, 0, 1;
        trans2 << 0, 0, 0, -position.x(),
            0, 0, 0, -position.y(),
            0, 0, 0, -position.z(),
            0, 0, 0, 1;
        Eigen::Matrix4f lookAt = trans1 * trans2;
        return lookAt;
    }

private:
    Eigen::Vector3f position;
    Eigen::Vector3f target;
    Eigen::Vector3f up;
};
#endif // !CAMERA_H
