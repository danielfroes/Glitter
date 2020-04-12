#ifndef __SHADER__
#define __SHADER__


#pragma once

#include <glad/glad.h>
 
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Texture.hpp"


class Shader
{
	public:
		unsigned int ID;
		
		//Shader(const char* vertexPath , const char* fragmentPath);
		Shader(const char* vertexPath, const char* fragmentPath, void (*uniformCalback)(unsigned int ID));

		Shader(const char* vertexPath, const char* fragmentPath, Texture texture, void (*uniformCalback)(unsigned int ID));
		Shader();
		void use();
		
		void setBoolUniform(const std::string &name, bool value) const;
		void setFloatUniform(const std::string &name, float value) const;
		void setIntUniform(const std::string& name, int value) const;


		

	private:
		void setupShader(const char* vertexPath, const char* fragmentPath);

		Texture* _texture = NULL;
		//**Generalizar 
		void (*_uniformCallback)(unsigned int ID) {};

		const char* _defaultVertexPath = "C:/Users/danie/Documents/OpenGL/Glitter/Shaders/Default/defaultVertex.glsl";
		const char* _defaultFragmentPath = "C:/Users/danie/Documents/OpenGL/Glitter/Shaders/Default/defaultFragment.glsl";
	/*	const char* _defaultVertexPath = "Default/defaultVertex.glsl";
		const char* _defaultFragmentPath = "Default/defaultFragment.glsl";*/
};


#endif
