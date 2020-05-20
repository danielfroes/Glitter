#include "Camera.hpp"
#include "Input.hpp"


//camera vector field parameters
glm::vec3 Camera::cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); //origin
glm::vec3 Camera::cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); //front vec
glm::vec3 Camera::cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //up vec


//Center of screen
float Camera::lastX = 500;
float Camera::lastY = 300;



//initial rotation
float Camera::yaw = -90.0f;
float Camera::pitch = 0.0f;

//initial fov
float Camera::fov = 45.0f;

float Camera::deltaTime = 0.0f;
float Camera::lastFrame = 0.0f;

Camera::Camera(WindowHolder* windowHolder)
{
	//glfw setup
	glfwSetInputMode(windowHolder->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(windowHolder->getWindow(), this->mouse_callback);
	glfwSetScrollCallback(windowHolder->getWindow(), this->scroll_callback);
}



glm::mat4 Camera::GetView() 
{

	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 Camera::GetPerspective()
{
	return glm::perspective(glm::radians(fov), 1000.0f / 600.0f, 0.1f, 100.0f);
}


void Camera::ProcessInput()
{

	const float cameraSpeed = 2.5f * deltaTime;
	if (Input::GetKey(GLFW_KEY_W))
	{
		cameraPos += cameraSpeed * cameraFront;
	}
	if (Input::GetKey(GLFW_KEY_S))
		cameraPos -= cameraSpeed * cameraFront;
	if (Input::GetKey(GLFW_KEY_D))
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (Input::GetKey(GLFW_KEY_A))
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;


	//Calculate the time of the last frame
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

}



void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top

	lastX = xpos;

	lastY = ypos;

	const float sensitivity = 0.05f;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

}


void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {


	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;

	if (fov <= 1.0f)
		fov = 1.0f;
	else if (fov >= 45.0f)
		fov = 45.0f;

}


