#include "WindowHolder.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include <cstdlib>

int RenderLoop(WindowHolder windowHolder);
void processInput(GLFWwindow* window);


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

int main()
{
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
	};

	//setar buffers
	unsigned int VBO;
	glGenBuffers(1, &VBO);		//Generates a VBO with a buffer id
	glBindBuffer(GL_ARRAY_BUFFER, VBO);		//Bind the vertex buffer to the GL_ARRAY_BUFFER type, so that it can be its target in GPU
		//arg: type target, size of data in bytes, data we want to send, how the GPU will manage the given data;
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		//Copies the vertex data into the buffer's memory (in the GPU)	

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);	 //create vertex shader object with a ID;
	//arg: shader object to compile, how many string are we passing as source code, source code str, NULL; 
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);		//links the shader source to the shader object
	glCompileShader(vertexShader);		//Compile shader

	// checks if the compilation was succesful
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	WindowHolder windowHolder;
	windowHolder.createWindow(1000, 600, "Pagina do Daniel");
	RenderLoop(windowHolder);

}

int RenderLoop(WindowHolder windowHolder)
{
	// Render Loop
	while (!glfwWindowShouldClose( windowHolder.getWindow() ))
	{
		//input
		processInput(windowHolder.getWindow());

		//rendering commands
		glClearColor(0.2, 0.3f, 0.3f, 1.0f); //set the color that will clear the screen
		glClear(GL_COLOR_BUFFER_BIT); //clear the color buffer

		//check and calls events and swap buffers
		glfwSwapBuffers(windowHolder.getWindow());	//swaps the color buffer (buffer that contains the colors values for each pixel in GLFW's window) 
		glfwPollEvents();	//checks if any input event are triggered, and calls the corresponding functions		
	}


	//glfwTerminate();	//clean all the GLFW's resources that were allocated
	return 0;
}	


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
