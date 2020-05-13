#include "WindowHolder.hpp"
#include "Model.hpp"
#include "Input.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Váriáveis globais
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); //origin
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); //front vec
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //up vec

float lastX = 400, lastY = 300;


float deltaTime = 0.0f;

float lastFrame = 0.0f;

float  yaw = -90.0f, pitch = 0.0f;

float fov = 45.0f;



int RenderLoop(WindowHolder windowHolder, Model models[], int numModels);
void processInput();



void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top

	lastX = xpos;

	lastY = ypos;

	const float sensitivity = 0.05f;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);

}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){

	
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;

	if (fov <= 1.0f)
		fov = 1.0f;
	else if (fov >= 45.0f)
		fov = 45.0f;

	std::cout << "fov: " << fov << std::endl;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Shaders scripts
void Shader_BlinkingScript(unsigned int ID)
{
	float timeValue = glfwGetTime();
	float value = (sin(timeValue) / 2.0f) + 0.5f; // goes 1 to 0 back and forth

	unsigned int uniformLocation = glGetUniformLocation(ID, "modifier");
	glUniform1f(uniformLocation, value);
}


//** esse callback é so necessario chamar 1 vez (antes do loop)
void Shader_TextureScript(unsigned int ID)
{
	unsigned int uniformLocation = glGetUniformLocation(ID, "ourTexture");
	glUniform1i(uniformLocation, 0);
}



void Shader_TransformScript(unsigned int ID)
{
	

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	unsigned int uniformLocation = glGetUniformLocation(ID, "model");
	//args: uniform location in shader program, how many matrices sending, transpose flag, matrix datas
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(model));




	glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	uniformLocation = glGetUniformLocation(ID, "view");
	//args: uniform location in shader program, how many matrices sending, transpose flag, matrix datas
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(view));


	glm::mat4 projection;
	//args: FOV angle, aspect ratio, near plane, far plane 
	projection = glm::perspective(glm::radians(fov), 1000.0f / 600.0f, 0.1f, 100.0f);
	uniformLocation = glGetUniformLocation(ID, "projection");
	//args: uniform location in shader program, how many matrices sending, transpose flag, matrix datas
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(projection));
}


void Shader_TransformScript2(unsigned int ID)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(80.0f), glm::vec3(0.5f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-1.0f, 0.5f, 1.0f));
	unsigned int uniformLocation = glGetUniformLocation(ID, "model");
	//args: uniform location in shader program, how many matrices sending, transpose flag, matrix datas
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(model));


	glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	uniformLocation = glGetUniformLocation(ID, "view");
	//args: uniform location in shader program, how many matrices sending, transpose flag, matrix datas
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(view));


	glm::mat4 projection;
	//args: FOV angle, aspect ratio, near plane, far plane 
	projection = glm::perspective(glm::radians(fov), 1000.0f / 600.0f, 0.1f, 100.0f);
	uniformLocation = glGetUniformLocation(ID, "projection");
	//args: uniform location in shader program, how many matrices sending, transpose flag, matrix datas
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(projection));
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main()
{
	//need to be at the top because it configure some GLFW sttuf
	WindowHolder windowHolder;
	windowHolder.createWindow(1000, 600, "Pagina do Daniel");


	glEnable(GL_DEPTH_TEST);

	//############################################################################################################
	//float bgPanelVert[] = {
	//	-1.0f, -1.0f, 1.0f, 0.8f, 0.6f, 0.6f, 0.0f, 0.0f,// left - bot
	//	1.0f, -1.0f, 1.0f,  0.7f, 0.6f, 0.6f, 5.0f, 0.0f,//right - bot
	//	-1.0f, 1.0f, 1.0f, 0.4f, 0.6f, 0.6f, 0.0f, 5.0f,// left - top
	//	1.0f, 1.0f, 1.0f, 0.1f, 0.6f, 0.6f, 5.0f, 5.0f//right - top
	//};
	//unsigned int bgPanelIndex[] = {
	//	0, 1, 2,
	//	1, 2, 3
	//};

	//**Decidir oq fazer para se tem cor ou não
	
	float cubeVert[] = {
		// Front face
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,// bottom left
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,// bottom right
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,// top right
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,// top left 

		// Back face
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,// bottom left
		-0.5f, 0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f,// bottom right
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,// top right
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,// top left 

		// Top face
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,// bottom left
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,// bottom right
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,// top right
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,// top left 

		// Bottom face
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,// bottom left
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,// bottom right
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,// top right
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,// top left 

		// Right face
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,// bottom left
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,// bottom right
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,// top right
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,// top left 

		// Left face
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,// bottom left
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,// bottom right
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,  1.0f, 1.0f,// top right
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f,  0.0f, 1.0f,// top left 
	};

	unsigned int cubeIndex[] = {
		0, 1, 2, 0, 2, 3,    // front
		4, 5, 6, 4, 6, 7,    // back
		8, 9, 10, 8, 10, 11,   // top
		12, 13, 14, 12, 14, 15,   // bottom
		16, 17, 18, 16, 18, 19,   // right
		20, 21, 22, 20, 22, 23,   // left
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
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

	Shader transformShader{
						"C:/Users/danie/Documents/OpenGL/Glitter/Shaders/Transform/transform.vert",
						"C:/Users/danie/Documents/OpenGL/Glitter/Shaders/Transform/transform.frag",
						dogTex,
						Shader_TransformScript
	};

	Shader transformShader2{
						"C:/Users/danie/Documents/OpenGL/Glitter/Shaders/Transform/transform.vert",
						"C:/Users/danie/Documents/OpenGL/Glitter/Shaders/Transform/transform.frag",
						dogTex,
						Shader_TransformScript2
	};


	Model modelsArr[] = {
		//Model{bgPanelVert, sizeof(bgPanelVert), bgPanelIndex, sizeof(bgPanelIndex)},
		Model{cubeVert, sizeof(cubeVert), cubeIndex, sizeof(cubeIndex), transformShader},
		Model{cubeVert, sizeof(cubeVert), cubeIndex, sizeof(cubeIndex), transformShader2}
	};

	int numModels = sizeof(modelsArr) / sizeof(Model);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	

	RenderLoop(windowHolder, modelsArr, numModels);

}




int RenderLoop(WindowHolder windowHolder, Model models[] , int numModels)
{

	glfwSetInputMode(windowHolder.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(windowHolder.getWindow(), mouse_callback);
	glfwSetScrollCallback(windowHolder.getWindow(), scroll_callback);



	Input::SetWindow(&windowHolder);

	while (!glfwWindowShouldClose(windowHolder.getWindow()))
	{



		if (Input::GetKey(GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(windowHolder.getWindow(), true);
		}
		//input
		processInput();



		//rendering commands//**talvez colocor no windowHolder
		glClearColor(0.8f, 0.8f, 1.0f, 1.0f); //set the color that will clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer
	 	
		//

		for(int i = 0; i < numModels; i ++)
		{	
			
			models[i].setupToRender(); //Bind VAO of the object
			
			//arg: primitive type to draw, how many indices to draw, type of the indices, offset in EBO (or pass in a index array); 
			glDrawElements(GL_TRIANGLES, models[i].getNumIndices(), GL_UNSIGNED_INT, 0);
		}
		//check and calls events and swap buffers
		glfwSwapBuffers(windowHolder.getWindow());	//swaps the color buffer (buffer that contains the colors values for each pixel in GLFW's window) 
		glfwPollEvents();	//checks if any input event are triggered, and calls the corresponding functions		


		//Calculate the time of the last frame
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}


	//glfwTerminate();	//clean all the GLFW's resources that were allocated
	return 0;
}






void processInput()
{

	//** implementar toogle
	if (Input::GetKey(GLFW_KEY_Z))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (Input::GetKey(GLFW_KEY_X))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}



	const float cameraSpeed =  2.5f * deltaTime;
	if (Input::GetKey(GLFW_KEY_W))
	{
		cameraPos += cameraSpeed * cameraFront;
		std::cout << "W detectado" << std::endl;
	}
	if (Input::GetKey(GLFW_KEY_S))
		cameraPos -= cameraSpeed * cameraFront;
	if (Input::GetKey(GLFW_KEY_D))
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (Input::GetKey(GLFW_KEY_A))
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

}


