#include "WindowHolder.hpp"
#include <iostream>

WindowHolder::WindowHolder()
{
	glfwInit();		//initialize GLFW
	//Configure glfw window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);		// set the major version of opengl to 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);		// set the minor version of opengl to 3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);		// set the profile of development to the Core profile, less abstractions
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	// necessary to run in Mac Os X

	_window = NULL;
}


//FUNC: createWindow
//- create, setup and return a instance of glfw window
void WindowHolder::createWindow(int width, int height, const char* title)
{
	_window = glfwCreateWindow(width, height, title, NULL, NULL);		//Create Window with width, height and name

	//if window was not created properly
	if (_window == NULL)
	{
		std::cout << "ERROR: Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}

	glfwMakeContextCurrent(_window);		//tells GLFW to make the context of our window the main context on the current thread

	//Initialize GLAD before we call any OpenGL function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))	//glfwGetProcAddres defines the correct function that the addres of OpenGL function pointer will be loaded, which is OS specific
	{
		std::cout << "ERROR: Failed to initialize GLAD" << std::endl;
		exit(-2);
	}

	glViewport(0, 0, width, height);		//set up the viewport of the window; coordinate of the left corner of the window and the viewport dimensions  

	glfwSetFramebufferSizeCallback(_window, this->framebufferSizeCallback);		//when window is risezed calls framebuffer_size_callback()
}

WindowHolder::~WindowHolder()
{	
	glfwTerminate();
}


//FUNC: framebuffer_size_callback
//- resizes the viewport dimension when the window is resized;
void WindowHolder::framebufferSizeCallback(GLFWwindow*, int width, int heigth)
{
	glViewport(0, 0, width, heigth);
}

GLFWwindow* WindowHolder::getWindow()
{
	return _window;
}