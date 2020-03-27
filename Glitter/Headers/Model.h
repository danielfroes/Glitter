#ifndef __MODEL__
#define __MODEL__
#pragma once


#include <GLFW/glfw3.h>
#include <glad/glad.h>

class Model
{
public:
	Model(float vertices[], int nVertices, unsigned int indices[], int nIndices);
	~Model();

	void setupToRender();
	int getNumIndices();

private:
	int _sizeVertices;
	int _sizeIndices;
	unsigned int _VBO;
	unsigned int _EBO;
	unsigned int _VAO;
};



#endif // !__MODEL__


