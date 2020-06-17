#pragma once
#include <gl/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Default camera values
const float YAW = -45.0f;
const float PITCH = 45.0f;
const float SENSITIVITY = 0.01f;
const float ZOOM = 25.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class OrbitCamera
{
public:
    // camera Attributes
    float x, y, z;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor
    OrbitCamera(float yaw = YAW, float pitch = PITCH, float Sensitivity = 0.01f, float Zoom = 25.0f) : MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix() const
    {
        return glm::lookAt(glm::vec3(x, y, z), glm::vec3(18, 0, 0), glm::vec3(0, 1, 0));
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;
        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }
        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    void updateCameraVectors()
    {
        x = Zoom * std::sin(Pitch) * cos(Yaw);
        y = Zoom * std::cos(Pitch);
        z = Zoom * std::sin(Pitch) * std::sin(Yaw);
    }
};


