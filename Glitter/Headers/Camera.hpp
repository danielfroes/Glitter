#pragma once


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "WindowHolder.hpp"

/*
-Criar a camera e movimentar a camera como se fosse um objeto diferente da cena independente dos modelos
*/
class Camera
{
public:
	Camera(){};
	Camera(WindowHolder* windowHolder);
	~Camera(){};
	static glm::mat4 GetView();
	static glm::mat4 GetPerspective();
	static void Camera::ProcessInput();


private:
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	
	
	// Váriáveis globais
	static glm::vec3 cameraPos;
	static glm::vec3 cameraFront;
	static glm::vec3 cameraUp;

	static float lastX , lastY;

	static float deltaTime;

	static float lastFrame;

	static float  yaw, pitch;

	static float fov;

	
};

