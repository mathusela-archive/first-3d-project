#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "CUSTOM/objects/camera.hpp"


// -----------------------------------------------------------------------------------------



float deltaTime;
float lastFrame;

float yaw = 90.0f;
float pitch = 0.0f;

Camera *varCamera;

bool captureCursorToggle = true;
int prevKey = 0;

void camera_controller_init(GLFWwindow* window) {

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}

void camera_controller(Camera *camera, GLFWwindow *window) {

    varCamera = camera;

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float baseSpeed = 4.0;
    float sprintSpeed = 8.0;
    float cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) cameraSpeed = sprintSpeed * deltaTime; else cameraSpeed = baseSpeed * deltaTime;

    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera -> location += cameraSpeed * glm::normalize(glm::vec3(camera -> front.x, 0.0, camera -> front.z));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera -> location -= cameraSpeed * glm::normalize(glm::vec3(camera -> front.x, 0.0, camera -> front.z));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera -> location -= glm::normalize(glm::cross(camera -> front, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera -> location += glm::normalize(glm::cross(camera -> front, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera -> location += cameraSpeed * cameraUp;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera -> location -= cameraSpeed * cameraUp;

    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    
    #ifndef FULLSCREEN
        if (!prevKey) {
            if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)  {
                if (captureCursorToggle) {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                    captureCursorToggle = false;
                } else {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    captureCursorToggle = true;
                }
            }
        }
    #else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    #endif
    prevKey = glfwGetKey(window, GLFW_KEY_TAB);
    
    glfwSetCursorPosCallback(window, mouse_callback);

    camera -> position =    glm::lookAt(camera -> location,
                                    camera -> location + camera -> front,
                                    cameraUp);

}

float lastx = 1100/2;
float lasty = 700/2;

const float sensitivity = 0.1f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    float xoffset = xpos - lastx;
    float yoffset = lasty - ypos; 
    lastx = xpos;
    lasty = ypos; 

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    varCamera -> front = glm::normalize(direction);

}



// -----------------------------------------------------------------------------------------



#endif