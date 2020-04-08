#include "WindowHolder.hpp"
#include "Model.h"
#include "Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include <cstdlib>

#include <fstream>
#include <sstream>

int RenderLoop(WindowHolder windowHolder, /*Shader ourShader,*/ Model models[], int numModels);
void processInput(GLFWwindow* window);


void Shader_BlinkingScript(unsigned int ID)
{
	float timeValue = glfwGetTime();
	float value = (sin(timeValue) / 2.0f) + 0.5f;

	unsigned int uniformLocation = glGetUniformLocation(ID, "modifier");
	glUniform1f(uniformLocation, value);
}

int main()
{ 

	//need to be at the top because it configure some GLFW sttuf
	WindowHolder windowHolder;
	windowHolder.createWindow(1000, 600, "Pagina do Daniel");


	float bgPanelVert[] = {
		-1.0f, -1.0f, 0.0f, 0.8f, 0.6f, 0.6f, // right - bot
		1.0f, -1.0f, 0.0f,  0.7f, 0.6f, 0.6f, //left - bot
		-1.0f, 1.0f, 0.0f, 0.4f, 0.6f, 0.6f, // right - top
		1.0f, 1.0f, 0.0f, 0.1f, 0.6f, 0.6f //left top
	};
	unsigned int bgPanelIndex[] = {
		0, 1, 2,
		1, 2, 3
	};

	//**Decidir oq fazer para se tem cor ou n�o
	float vertices1[] = { // Retangulo
	-0.25f,  0.25f, 0.0f, 0.8f, 0.6f, 0.6f, // top right
	-0.25f, -0.25f, 0.0f, 0.7f, 0.6f, 0.6f,  // bottom right
	-0.75f, -0.25f, 0.0f, 0.4f, 0.6f, 0.6f,  // bottom left
	-0.75f,  0.25f, 0.0f, 0.1f, 0.6f, 0.6f // top left 
	};
	unsigned int indices1[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};


	float vertices2[] = { //Triangulo
	 -0.25f,  -0.5f, 0.0f, 0.8f, 0.2f, 0.2f, // top
	 0.75f, -0.5f, 0.0f, 0.5f, 0.2f, 0.2f, // bottom right
	 0.25f, 0.5f, 0.0f, 0.3f, 0.2f, 0.2f // bottom left
	};

	unsigned int indices2[] = {  // note that we start from 0!
		0, 1, 2,   // first triangle
	};

	Shader ourShader(
					 "C:/Users/danie/Documents/OpenGL/Glitter/Shaders/vertexShader.glsl",
					 "C:/Users/danie/Documents/OpenGL/Glitter/Shaders/fragmentShader.glsl",
					  Shader_BlinkingScript
					);

	Model modelsArr[] = {
		Model{bgPanelVert, sizeof(bgPanelVert), bgPanelIndex, sizeof(bgPanelIndex), ourShader},
		Model{vertices1, sizeof(vertices1), indices1, sizeof(indices1)}, //retangulo
		Model{vertices2, sizeof(vertices2), indices2, sizeof(indices2), ourShader }//triangulo
	};

	int numModels = sizeof(modelsArr) / sizeof(Model);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	RenderLoop(windowHolder, /*ourShader,*/ modelsArr, numModels);

}




int RenderLoop(WindowHolder windowHolder,/* Shader ourShader,*/ Model models[] , int numModels)
{
	while (!glfwWindowShouldClose(windowHolder.getWindow()))
	{
		//input
		processInput(windowHolder.getWindow());

		//rendering commands//**talvez colocor no windowHolder
		glClearColor(0.8f, 0.8f, 1.0f, 1.0f); //set the color that will clear the screen
		glClear(GL_COLOR_BUFFER_BIT); //clear the color buffer

		//

		for(int i = 0; i < numModels; i ++)
		{

			//**PROBLEMA, N�o quero fazer um if pra cada
			models[i].setupToRender(); //Bind VAO of the object

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
