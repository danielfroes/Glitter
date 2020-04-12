#include "Texture.hpp"


Texture::Texture(const char* texturePath)
{
	//flip the y axis of all the images that will be loaded to match with the opengl coord system
	stbi_set_flip_vertically_on_load(true);

	//generating a texture object
	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glGenTextures(1, &_texObj); // args: how many textures we want to generate and then stores it in a array;
	glBindTexture(GL_TEXTURE_2D, _texObj); //bind a texture object to be configured with the target of 2D texture

	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrColorChannels;
	unsigned char* imgData = stbi_load(texturePath, &width, &height, &nrColorChannels, 0);

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
}


void Texture::ActivateTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texObj);
	
	/*glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);*/
}