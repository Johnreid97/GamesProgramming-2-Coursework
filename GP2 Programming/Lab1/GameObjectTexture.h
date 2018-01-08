#pragma once
#include <string>
#include <GL\glew.h>

class GameObjectTexture
{
public:
	GameObjectTexture(const std::string& fileName);

	void Bind(unsigned int unit); // bind upto 32 textures

	~GameObjectTexture();

protected:
private:

	GLuint textureHandler;
};

