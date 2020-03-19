
#ifndef __WINDOW__
#define __WINDOW__
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
	public:
		
		Window();
		~Window();

		GLFWwindow* createWindow(int width, int height, const char* title);


	private:
		//maybe create an instance of glfwWindow and only access it with getWindowInstance
		//maybe abstract every function that requires the window to this class 

		static void framebuffer_size_callback(GLFWwindow* window, int width, int heigth);
};
#endif
