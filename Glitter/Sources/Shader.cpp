#include "Shader.h"


Shader::Shader()
{
	setupShader(_defaultVertexPath, _defaultFragmentPath);
	
}

//Shader::Shader(const char* vertexPath, const char* fragmentPath)
//{
//
//	setupShader(vertexPath, fragmentPath);
//
//}

Shader::Shader(const char* vertexPath, const char* fragmentPath, void (*customCalback)(unsigned int ID) = NULL)
{
	uniformCallback = customCalback;
	setupShader(vertexPath, fragmentPath);

}


void Shader::setupShader(const char* vertexPath , const char* fragmentPath )
{
	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	//ensure ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into stream
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{

		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl << e.code() << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();



	unsigned int vertex;
	vertex = glCreateShader(GL_VERTEX_SHADER);	 //create vertex shader object with a/ ID;
	//arg: shader object to compile, how many string are we passing as source code, source code str, NULL; 
	glShaderSource(vertex, 1, &vShaderCode, NULL);		//links the shader source to the shader object
	glCompileShader(vertex);		//Compile shader

	int  success;
	char infoLog[512];
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragment;
	fragment = glCreateShader(GL_FRAGMENT_SHADER);	 //create fragment shader object with a ID;
	//arg: shader object to compile, how many string are we passing as source code, source code str, NULL; 
	glShaderSource(fragment, 1, &fShaderCode, NULL);		//links the shader source to the shader object
	glCompileShader(fragment);	//Compile shader
	// checks if the compilation was succesful

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	ID = glCreateProgram(); //create program object; 
	glAttachShader(ID, vertex); //attach the compiled shader to the shader program object
	glAttachShader(ID, fragment);
	glLinkProgram(ID);  // links the output and input of each shader

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glUseProgram(ID); //activate the shader program; the active shader program will be used when we issued render calls
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}





void Shader::use()
{
	glUseProgram(ID);

	//** Generilzar Chama a função do uniform
	if (uniformCallback != NULL)
	{
		std::cout << "callback called "<< std::endl;
		uniformCallback(ID);
	}
	else
		std::cout << "no callback" << std::endl; 
}




void Shader::setBoolUniform(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setFloatUniform(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setIntUniform(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
