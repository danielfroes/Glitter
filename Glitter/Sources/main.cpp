#include "WindowHolder.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include <cstdlib>

int RenderLoop(WindowHolder windowHolder, unsigned int shaderProgram, unsigned int _VAO);
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




int main()
{
	float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	//need to be at the top because it configure some GLFW sttuf
	WindowHolder windowHolder;
	windowHolder.createWindow(1000, 600, "Pagina do Daniel");

	unsigned int shaderProgram = HandleShaderStuff();

	//setar buffers
	unsigned int VBO;
	glGenBuffers(1, &VBO);		//Generates a VBO with a buffer id
	unsigned int VAO;	//Vertex array object: store the configuration state of the binded vertex buffer
	glGenVertexArrays(1, &VAO);	//
	unsigned int EBO;
	glGenBuffers(1, &EBO);  //Generate a Element Buffer Object: stores the vertex indices of each primittive

	glBindVertexArray(VAO);		//Bind Vertex array to the following Vertex Buffer Array to store its configuration;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);		//Bind the vertex buffer to the GL_ARRAY_BUFFER type, so that it can be its target in GPU

	//arg: type target, size of data in bytes, data we want to send, how the GPU will manage the given data;
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		//Copies the vertex data into the buffer's memory (in the GPU)	

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//arg:  specifies which vertex attribute to configure (vertexShader => location = 0),
	//		how many values in the attribute, the type of each value, if we want the date to be normalized (0 to 1),
	//		the stride (space between consecutive vertex attribute), the offset where the data begins in the buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //tells how the input from VBO goes to each vertex attribute 
	glEnableVertexAttribArray(0); //enables the vertex attribute, giving the vertex attribute location.
	
	

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	RenderLoop(windowHolder, shaderProgram, VAO);
	
}

int RenderLoop(WindowHolder windowHolder, unsigned int shaderProgram, unsigned int VAO)
{
	// Render Loop
	while (!glfwWindowShouldClose( windowHolder.getWindow() ))
	{
		//input
		processInput(windowHolder.getWindow());

		//rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //set the color that will clear the screen
		glClear(GL_COLOR_BUFFER_BIT); //clear the color buffer


		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);

	
		//arg: primitive type to draw, how many indices to draw, type of the indices, offset in EBO (or pass in a index array); 
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//check and calls events and swap buffers
		glfwSwapBuffers(windowHolder.getWindow());	//swaps the color buffer (buffer that contains the colors values for each pixel in GLFW's window) 
		glfwPollEvents();	//checks if any input event are triggered, and calls the corresponding functions		
	}


	//glfwTerminate();	//clean all the GLFW's resources that were allocated
	return 0;
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
