#include "GameDisplay.h"

//sets the screen dimensions
GameDisplay::GameDisplay()
{
	sdlWindow = nullptr; 
	screenWidth = 1024.0f;
	screenHeight = 968.0f; 
}

GameDisplay::~GameDisplay()
{
	//deletes the window context, then window
	SDL_GL_DeleteContext(glContext); 
	SDL_DestroyWindow(sdlWindow); 
	SDL_Quit();
}

//gets screen width and height
float GameDisplay::gainWidth() { return screenWidth; } 
float GameDisplay::gainHeight() { return screenHeight; }

//error checking
void GameDisplay::returnError(std::string errorString)
{
	std::cout << errorString << std::endl;
	std::cout << "press any  key to quit...";
	int in;
	std::cin >> in;
	SDL_Quit();
}

void GameDisplay::changeBuffer()
{
	//swaps buffers 
	SDL_GL_SwapWindow(sdlWindow); 
}

void GameDisplay::wipeDisplay(float r, float g, float b, float a)
{
	//clears the window display
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void GameDisplay::initialiseDisplay()
{
	//initialises the full display
	SDL_Init(SDL_INIT_EVERYTHING); 

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); 
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	sdlWindow = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)screenWidth, (int)screenHeight, SDL_WINDOW_OPENGL); // create window

	//error debugging 
	if (sdlWindow == nullptr)
	{
		returnError("window failed to create");
	}

	glContext = SDL_GL_CreateContext(sdlWindow);

	if (glContext == nullptr)
	{
		returnError("SDL_GL context failed to create");
	}

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		returnError("GLEW failed to initialise");
	}

	glEnable(GL_DEPTH_TEST); //enable z-buffering 
	glEnable(GL_CULL_FACE); //dont draw faces that are not pointing at the camera

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
}