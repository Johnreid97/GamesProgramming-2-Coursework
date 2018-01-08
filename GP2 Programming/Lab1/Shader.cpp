#include "GameObjectShader.h"
#include <iostream>
#include <fstream>

GameObjectShader::GameObjectShader(const std::string& filename)
{
	program = glCreateProgram(); // create shader program
	shaders[0] = CreateGameObjectShader(LoadGameObjectShader("..\\res\\shader.vert"), GL_VERTEX_SHADER); // create vertex shader
	shaders[1] = CreateGameObjectShader(LoadGameObjectShader("..\\res\\shader.frag"), GL_FRAGMENT_SHADER); // create fragment shader

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		//add all shaders to shader program
		glAttachShader(program, shaders[i]); 
	}


	glBindAttribLocation(program, 0, "position");  
	glBindAttribLocation(program, 1, "texCoord"); 

	//creates program that GPU will run
	glLinkProgram(program);

	CheckShaderError(program, GL_LINK_STATUS, true, "Error: Shader program linking failed"); // cheack for error

	glValidateProgram(program); //check the entire program is valid
	CheckShaderError(program, GL_VALIDATE_STATUS, true, "Error: Shader program not valid");

	uniforms[TRANSFORM_U] = glGetUniformLocation(program, "transform");	
}


GameObjectShader::~GameObjectShader()
{
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		//detach shader 
		glDetachShader(program, shaders[i]); 
		//delete shaders
		glDeleteShader(shaders[i]);
	}
	// delete program
	glDeleteProgram(program); 
}

void GameObjectShader::Attach()
{
	glUseProgram(program);
}

void GameObjectShader::Update(const Transform& transform, const CameraCreator& camera)
{
	glm::mat4 mvp = camera.GetCameraProjection() * transform.GetModelMatrix();
	glUniformMatrix4fv(uniforms[TRANSFORM_U], 1, GLU_FALSE, &mvp[0][0]);
}


GLuint GameObjectShader::CreateGameObjectShader(const std::string& text, unsigned int type)
{
	GLuint shader = glCreateShader(type); //create shader based on specified type

	//checks if shader has actually been created
	if (shader == 0) 
		std::cerr << "Error type creation failed " << type << std::endl;

	const GLchar* stringSource[1]; //convert strings into list of c-strings
	stringSource[0] = text.c_str();
	GLint lengths[1];
	lengths[0] = text.length();

	glShaderSource(shader, 1, stringSource, lengths); 
	glCompileShader(shader); 

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!"); //check for compile error

	return shader;
}

std::string GameObjectShader::LoadGameObjectShader(const std::string& fileName)
{
	//loads shader using a file
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void GameObjectShader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	//checks if any errors
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}

