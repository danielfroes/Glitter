#include "WindowHolder.hpp"
#include "Model.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include <cstdlib>


void processInput(GLFWwindow* window);
unsigned int HandleShaderStuff();

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";


const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


int RenderLoop(WindowHolder windowHolder, unsigned int shaderProgram, Model models[] , int numModels)
{
	// Render Loop
	while (!glfwWindowShouldClose(windowHolder.getWindow()))
	{
		//input
		processInput(windowHolder.getWindow());

		//rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //set the color that will clear the screen
		glClear(GL_COLOR_BUFFER_BIT); //clear the color buffer


		glUseProgram(shaderProgram);
		
		for(int i = 0; i < numModels; i ++)
		{
			models[i] .setupToRender();
			//arg: primitive type to draw, how many indices to draw, type of the indices, offset in EBO (or pass in a index array); 
			glDrawElements(GL_TRIANGLES, models[i].getNumIndices(), GL_UNSIGNED_INT, 0);
		}
		//check and calls events and swap buffers
		glfwSwapBuffers(windowHolder.getWindow());	//swaps the color buffer (buffer that contains the colors values for each pixel in GLFW's window) 
		glfwPollEvents();	//checks if any input event are triggered, and calls the corresponding functions		
	}


	//glfwTerminate();	//clean all the GLFW's resources that were allocated
	return 0;
}


int main()
{
	//need to be at the top because it configure some GLFW sttuf
	WindowHolder windowHolder;
	windowHolder.createWindow(1000, 600, "Pagina do Daniel");


	float vertices1[] = {
	-0.25f,  0.25f, 0.0f,  // top right
	-0.25f, -0.25f, 0.0f,  // bottom right
	-0.75f, -0.25f, 0.0f,  // bottom left
	-0.75f,  0.25f, 0.0f   // top left 
	};
	unsigned int indices1[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	
	float vertices2[] = {
	 0.25f,  0.25f, 0.0f,  // top right
	 0.25f, -0.25f, 0.0f,  // bottom right
	 0.75f, -0.25f, 0.0f,  // bottom left
	 0.56f, -0.9, 0.0,
	 0.4f, 0.3f, 0.0f,
	};
	unsigned int indices2[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		0, 3, 5,
		0, 4, 1
	};


	unsigned int shaderProgram = HandleShaderStuff();



	Model modelsArr[] = { 
		Model(vertices1, sizeof(vertices1), indices1, sizeof(indices1)),
		Model(vertices2, sizeof(vertices2), indices2, sizeof(indices2))
	};
	
	int numModels = sizeof(modelsArr) / sizeof(Model);

	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	RenderLoop(windowHolder, shaderProgram, modelsArr, numModels);
	
}





unsigned int HandleShaderStuff()
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//shader handling
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);	 //create vertex shader object with a/ ID;
	//arg: shader object to compile, how many string are we passing as source code, source code str, NULL; 
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);		//links the shader source to the shader object
	glCompileShader(vertexShader);		//Compile shader

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);	 //create fragment shader object with a ID;
	//arg: shader object to compile, how many string are we passing as source code, source code str, NULL; 
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);		//links the shader source to the shader object
	glCompileShader(fragmentShader);	//Compile shader
	// checks if the compilation was succesful

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int shaderProgam;
	shaderProgam = glCreateProgram(); //create program object; 
	glAttachShader(shaderProgam, vertexShader); //attach the compiled shader to the shader program object
	glAttachShader(shaderProgam, fragmentShader);
	glLinkProgram(shaderProgam);  // links the output and input of each shader

	glGetProgramiv(shaderProgam, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glUseProgram(shaderProgam); //activate the shader program; the active shader program will be used when we issued render calls
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	return shaderProgam;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}



	//** implementar toogle
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
