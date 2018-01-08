#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>
using namespace std;


class GameDisplay
{
public:
	GameDisplay();
	~GameDisplay();
	void initialiseDisplay();
	void changeBuffer();
	void wipeDisplay(float r, float g, float b, float a);

	float gainWidth();
	float gainHeight();

private:

	void returnError(std::string errorString);
	
	SDL_GLContext glContext; //global variable to hold the context
	SDL_Window* sdlWindow; //holds pointer to out window
	float screenWidth;
	float screenHeight;
};

