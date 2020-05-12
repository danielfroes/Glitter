#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "WindowHolder.hpp"


class Input
{
public:
	Input();

	static void SetWindow(WindowHolder* window);

	static bool  GetKey(int keyName);

	~Input();
private:
	static GLFWwindow*  _window;


};

