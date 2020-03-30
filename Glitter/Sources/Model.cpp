#include "Model.h"



Model::Model(float vertices[], int sizeVertices, unsigned int indices[], int sizeIndices)
{
	

	glGenBuffers(1, &_VBO);		//Generates a Vertex Buffer Array with a buffer id
	glGenVertexArrays(1, &_VAO);	//Vertex array object: store the configuration state of the binded vertex buffer
	glGenBuffers(1, &_EBO);  //Generate a Element Buffer Object: stores the vertex indices of each primittive


	_sizeVertices = sizeVertices;
	_sizeIndices = sizeIndices;

	glBindVertexArray(_VAO);		//Bind Vertex array to the following Vertex Buffer Array to store its configuration;
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);		//Bind the vertex buffer to the GL_ARRAY_BUFFER type, so that it can be its target in GPU

	//arg: type target, size of data in bytes, data we want to send, how the GPU will manage the given data;
	glBufferData(GL_ARRAY_BUFFER, _sizeVertices, vertices, GL_STATIC_DRAW);		//Copies the vertex data into the buffer's memory (in the GPU)	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _sizeIndices, indices, GL_STATIC_DRAW);

	//** talvez de problema na hora de modularizar o shaders e no stride
	//arg:  specifies which vertex attribute to configure (vertexShader => location = 0), 
	//		how many values in the attribute, the type of each value, if we want the data to be normalized (0 to 1),
	//		the stride (space between consecutive vertex attribute), the offset where the data begins in the buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); //tells how the input from VBO goes to each vertex attribute 
	glEnableVertexAttribArray(0); //enables the vertex attribute, giving the vertex attribute location.

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}
	
	

Model::~Model()
{


}


void Model::setupToRender()
{
	glBindVertexArray(_VAO);
}

int Model::getNumIndices()
{
	return _sizeIndices / sizeof(unsigned int);
}