#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
using namespace Render;

Camera::Camera(float vfov, float aspect)
    : aspectRatio(aspect),
      fieldOfView(glm::radians(vfov)),
      nearz(0.1f), farz(1000.f)
{}

void Camera::spawn()
{}

void Camera::think()
{
    auto pEnt = dynamic_cast<const TransformEntity*>(this->getParent());
    if (pEnt)
    {
        transform.pos = pEnt->transform.pos;
        transform.rot = pEnt->transform.rot;
    }

    Mat4 view;
    view = glm::mat4_cast(transform.rot);
    view = glm::translate(view, -transform.pos);
    Mat4 projection = glm::perspective(fieldOfView, aspectRatio, nearz, farz);
    viewProjectionMatrix = projection * view;
}

void Camera::setClip(float near, float far)
{
    nearz = near;
    farz = far;
}

const Mat4& Camera::getMatrix() const
{
    return viewProjectionMatrix;
}