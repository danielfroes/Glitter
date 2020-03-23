#ifndef __WINDOW_HOLDER__
#define __WINDOW_HOLDER__

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class WindowHolder
{
public:
	void createWindow(int width, int height, const char* title);
	GLFWwindow* getWindow();
	WindowHolder();
	~WindowHolder(); 

private:
	GLFWwindow*	_window;
	static void framebufferSizeCallback(GLFWwindow*, int width, int heigth);
};


#endif