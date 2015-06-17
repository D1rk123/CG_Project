#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>

class Camera
{
    glm::vec3 position;
    float fieldOfView;
    float nearPlane;
    float farPlane;
    float viewportAspectRatio;

    public:
    Camera(glm::vec3 position) {
        this->position = position;
        fieldOfView = 50.0f;
        nearPlane = 0.01f;
        farPlane = 100.0f;
        viewportAspectRatio = 4.0f/3.0f;
    }
    Camera() {}

    void setPosition( glm::vec3 newPos) {
        position = newPos;
    }

    void offsetPosition( glm::vec3 offset) {
        position += offset;
    }

    float getFieldOfView() {
        return fieldOfView;
    }

    float getHeightOfView() {
        return -farPlane*glm::tan(0.5*fieldOfView);
    }

    void setFieldOfView(float newFieldOfView) {
        assert(newFieldOfView > 0.0f && newFieldOfView < 180.0f);
        fieldOfView = newFieldOfView;
    }

    void setViewportAspectRatio(float newViewportAspectRatio) {
        assert(newViewportAspectRatio > 0.0);
        viewportAspectRatio = newViewportAspectRatio;
    }

    glm::mat4 getOrientation() {
        return projection() * glm::translate(glm::mat4(), -position);
    }

    glm::mat4 projection() {
        return glm::perspective(glm::radians(fieldOfView), viewportAspectRatio, nearPlane, farPlane);
    }

private:

};

#endif // CAMERA_H
