#pragma once
#include <string>
#include <GL\glew.h>
#include "transform.h"
#include "CameraCreator.h"

class GameObjectShader
{
public:
	GameObjectShader(const std::string& filename);

	void Attach(); //Makes GPU use the shader
	void Update(const Transform& transform, const CameraCreator& camera);

	std::string GameObjectShader::LoadGameObjectShader(const std::string& fileName);
	void GameObjectShader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint GameObjectShader::CreateGameObjectShader(const std::string& text, unsigned int type);

    ~GameObjectShader();


protected:
private:
	static const unsigned int NUM_SHADERS = 2; // number of shaders

	enum
	{
		TRANSFORM_U,

		NUM_UNIFORMS
	};

	GLuint program; // Track the shader program
	GLuint shaders[NUM_SHADERS]; //array of shaders
	GLuint uniforms[NUM_UNIFORMS]; //no of uniform variables
};
