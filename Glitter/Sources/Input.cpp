#include "Input.hpp"
#include <iostream>

GLFWwindow* Input::_window = NULL;

Input::Input() {};


void Input::SetWindow(WindowHolder* windowHolder)
{

	//std::cout << "teste" << std::endl;
	_window = windowHolder->getWindow();
}

bool Input::GetKey(int keyName) 
{

	if (_window != NULL)
	{
		if (glfwGetKey(_window, keyName))
			return true;
		else
			return false;
	}
	else
	{
		std::cout << "ERROR::INPUT::WINDOW::NOT::SET" << std::endl;	
		return false;
	}

}


Input::~Input(){};


