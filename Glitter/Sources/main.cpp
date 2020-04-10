#include "WindowHolder.hpp"
#include "Model.h"
#include "Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


int RenderLoop(WindowHolder windowHolder, unsigned int texture1, unsigned int texture2, Model models[], int numModels);
void processInput(GLFWwindow* window);


void Shader_BlinkingScript(unsigned int ID)
{
	float timeValue = glfwGetTime();
	float value = (sin(timeValue) / 2.0f) + 0.5f;

	unsigned int uniformLocation = glGetUniformLocation(ID, "modifier");
	glUniform1f(uniformLocation, value);
}

//** esse callback é so necessario chamar 1 vez (antes do loop)
void Shader_TextureScript(unsigned int ID)
{
	unsigned int uniformLocation = glGetUniformLocation(ID, "texture1");
	glUniform1i(uniformLocation, 0);
	uniformLocation = glGetUniformLocation(ID, "texture2");
	glUniform1i(uniformLocation, 1);
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

	//**Decidir oq fazer para se tem cor ou não
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
	 0.25f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f // top
	};

	unsigned int indices2[] = {  // note that we start from 0!
		0, 1, 2,   // first triangle
	};
	//############################################################################################################
	
	//flip the y axis of all the images that will be loaded to match with the opengl coord system
	stbi_set_flip_vertically_on_load(true);

		//generating a texture object
	unsigned int texture1;
	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glGenTextures(1, &texture1); // args: how many textures we want to generate and then stores it in a array;
	glBindTexture(GL_TEXTURE_2D, texture1); //bind a texture object to be configured with the target of 2D texture


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrColorChannels;
	unsigned char* imgData = stbi_load("C:/Users/danie/Documents/OpenGL/Glitter/Textures/container.jpg", &width, &height, &nrColorChannels, 0);

	if (imgData)
	{
		//Arg: Texture target, mipmap level (if needed to set manually), format we want to store the texture (our img has only RGB values), ...
		//... Width of tex, Height of tex, ALWAYS ZERO (some legacy stuff), format of the loaded img, data type of the loaded img (chars = bytes), img data itself
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData); //put the texture data in the currently bound texture object
		glGenerateMipmap(GL_TEXTURE_2D); //generate all the mipmaps for the currently bound texture;
	}
	else
	{
		std::cout << "ERROR::FAILED_TO_LOAD_TEXTURE" << std::endl;
	}
	stbi_image_free(imgData);

	unsigned int texture2;
	glActiveTexture(GL_TEXTURE1); // activate the texture unit first before binding texture
	glGenTextures(1, &texture2); // args: how many textures we want to generate and then stores it in a array;
	glBindTexture(GL_TEXTURE_2D, texture2); //bind a texture object to be configured with the target of 2D texture


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	width, height, nrColorChannels;
	imgData = stbi_load("C:/Users/danie/Documents/OpenGL/Glitter/Textures/awesomeface.png", &width, &height, &nrColorChannels, 0);

	if (imgData)
	{
		///                                                      GL_RGBA: specifies the alpha channel in the img
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData); //put the texture data in the currently bound texture object
		glGenerateMipmap(GL_TEXTURE_2D); //generate all the mipmaps for the currently bound texture;
	}
	else
	{
		std::cout << "ERROR::FAILED_TO_LOAD_TEXTURE" << std::endl;
	}
	stbi_image_free(imgData);

	//###############################################################################################################


	Shader blinkingShader{
						"C:/Users/danie/Documents/OpenGL/Glitter/Shaders/Blinking/blinkingVertex.glsl",
						"C:/Users/danie/Documents/OpenGL/Glitter/Shaders/Blinking/blinkingFragment.glsl",
						Shader_BlinkingScript
	};

	//** default parameter of callback not working
	Shader textureShader{
						"C:/Users/danie/Documents/OpenGL/Glitter/Shaders/DefaultTexture/dTextureVertex.glsl",
						"C:/Users/danie/Documents/OpenGL/Glitter/Shaders/DefaultTexture/dTextureFragment.glsl",
						Shader_TextureScript
	};

	Model modelsArr[] = {
		Model{bgPanelVert, sizeof(bgPanelVert), bgPanelIndex, sizeof(bgPanelIndex), textureShader},
		Model{vertices1, sizeof(vertices1), indices1, sizeof(indices1), textureShader},//retangulo
		Model{vertices2, sizeof(vertices2), indices2, sizeof(indices2), blinkingShader }//triangulo
	};

	int numModels = sizeof(modelsArr) / sizeof(Model);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	RenderLoop(windowHolder, texture1, texture2, modelsArr, numModels);

}




int RenderLoop(WindowHolder windowHolder, unsigned int texture1, unsigned int texture2, Model models[] , int numModels)
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
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture2);


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
