#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__
#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class Texture
{
public:
	Texture(const char* texturePath);

	~Texture();
	
	void ActivateTexture();
	

private:
	
	unsigned int _texObj;
};

#endif