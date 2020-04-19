#ifndef __MODEL__
#define __MODEL__
#pragma once


#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <Shader.hpp>

class Model
{
public:
	Model(float vertices[], int nVertices, unsigned int indices[], int nIndices);
	Model(float vertices[], int nVertices, unsigned int indices[], int nIndices, Shader shader);
	~Model();

	void setupToRender();
	int getNumIndices();
	


private:
	int _sizeVertices;
	int _sizeIndices;
	unsigned int _VBO;
	unsigned int _EBO;
	unsigned int _VAO;

	Shader _shader;

	void _setupBuffers(float vertices[], int sizeVertices, unsigned int indices[], int sizeIndices);
};



#endif // !__MODEL__


