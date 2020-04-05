#ifndef __SHADER__
#define __SHADER__


#pragma once

#include <glad/glad.h>
 
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
	public:
		unsigned int ID;
		
		Shader(const char* vertexPath , const char* fragmentPath);
		Shader();
		void use();
		
		void setBoolUniform(const std::string &name, bool value) const;
		void setFloatUniform(const std::string &name, float value) const;
		void setIntUniform(const std::string& name, int value) const;

	private:
		void setupShader(const char* vertexPath, const char* fragmentPath);
		const char* _defaultVertexPath = "../Glitter/Shaders/Default/defaultVertex.glsl";
		const char* _defaultFragmentPath = "../Glitter/Shaders/Default/defaultFragment.glsl";
};

#endif
