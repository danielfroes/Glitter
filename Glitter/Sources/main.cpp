#include "WindowHolder.hpp"
#include "Model.hpp"


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



int RenderLoop(WindowHolder windowHolder, Model models[], int numModels);
void processInput(GLFWwindow* window);


void Shader_BlinkingScript(unsigned int ID)
{
	float timeValue = glfwGetTime();
	float value = (sin(timeValue) / 2.0f) + 0.5f; // goes 1 to 0 back and forth

	unsigned int uniformLocation = glGetUniformLocation(ID, "modifier");
	glUniform1f(uniformLocation, value);
}


//** esse callback � so necessario chamar 1 vez (antes do loop)
void Shader_TextureScript(unsigned int ID)
{
	unsigned int uniformLocation = glGetUniformLocation(ID, "ourTexture");
	glUniform1i(uniformLocation, 0);
	//uniformLocation = glGetUniformLocation(ID, "ourTexture2");
	//glUniform1i(uniformLocation, 1);

	//float timeValue = glfwGetTime();
	//float value = (sin(timeValue) / 2.0f) + 0.5f; // goes 1 to 0 back and forth
	//
	//uniformLocation = glGetUniformLocation(ID, "interpolationValue");
	//glUniform1f(uniformLocation, value);
}



void Shader_RotateScript(unsigned int ID)
{
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	
	unsigned int uniformLocation = glGetUniformLocation(ID, "transform");
	//args: uniform location in shader program, how many matrices sending, transpose flag, matrix datas
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(trans));
}

int main()
{

	//need to be at the top because it configure some GLFW sttuf
	WindowHolder windowHolder;
	windowHolder.createWindow(1000, 600, "Pagina do Daniel");

	//############################################################################################################
	float bgPanelVert[] = {
		-1.0f, -1.0f, 0.0f, 0.8f, 0.6f, 0.6f, 0.0f, 0.0f,// left - bot
		1.0f, -1.0f, 0.0f,  0.7f, 0.6f, 0.6f, 5.0f, 0.0f,//right - bot
		-1.0f, 1.0f, 0.0f, 0.4f, 0.6f, 0.6f, 0.0f, 5.0f,// left - top
		1.0f, 1.0f, 0.0f, 0.1f, 0.6f, 0.6f, 5.0f, 5.0f//right - top
	};
	unsigned int bgPanelIndex[] = {
		0, 1, 2,
		1, 2, 3
	};

	//**Decidir oq fazer para se tem cor ou n�o
	float vertices1[] = { // Retangulo (Pos-3, Color-3, Texture-2)
	-0.25f,  0.25f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // top right
	-0.25f, -0.25f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,// bottom right
	-0.75f, -0.25f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,// bottom left
	-0.75f,  0.25f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f// top left 
	};
	unsigned int indices1[] = {  // note that we start from 0!
		1, 0, 2,   // first triangle
		2, 3, 0    // second triangle
	};


	float vertices2[] = { //Triangulo
	 -0.25f,  -0.5f, 0.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0, // bottom left
	 0.75f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,// bottom right
	 0.25f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.5f, 1.0f // top
	};

	unsigned int indices2[] = {  // note that we start from 0!
		0, 1, 2,   // first triangle
	};

	//############################################################################################################
	Texture dogTex{ "C:/Users/danie/Documents/OpenGL/Glitter/Textures/dog.jpg"};

	Shader blinkingShader{
						"C:/Users/danie/Documents/OpenGL/Glitter/Shaders/Blinking/blinking.vert",
						"C:/Users/danie/Documents/OpenGL/Glitter/Shaders/Blinking/blinking.frag",
						Shader_BlinkingScript
	};


	////** default parameter of callback not working
	Shader dogShader{
						"C:/Users/danie/Documents/OpenGL/Glitter/Shaders/DefaultTexture/dTexture.vert",
						"C:/Users/danie/Documents/OpenGL/Glitter/Shaders/DefaultTexture/dTexture.frag",
						dogTex,
						NULL
	};

	Shader rotateShader{
						"C:/Users/danie/Documents/OpenGL/Glitter/Shaders/Transform/transform.vert",
						"C:/Users/danie/Documents/OpenGL/Glitter/Shaders/Transform/transform.frag",
						dogTex,
						Shader_RotateScript
	};

	Model modelsArr[] = {
		Model{bgPanelVert, sizeof(bgPanelVert), bgPanelIndex, sizeof(bgPanelIndex)},
		Model{vertices1, sizeof(vertices1), indices1, sizeof(indices1), dogShader},//retangulO
		Model{vertices2, sizeof(vertices2), indices2, sizeof(indices2), blinkingShader},//triangulo
	};

	int numModels = sizeof(modelsArr) / sizeof(Model);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	RenderLoop(windowHolder, modelsArr, numModels);

}




int RenderLoop(WindowHolder windowHolder, Model models[] , int numModels)
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
			/*glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture2);*/

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
