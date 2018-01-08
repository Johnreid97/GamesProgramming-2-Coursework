#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "GameDisplay.h" 
#include "GameObjectShader.h"
#include "GameObject.h"
#include "GameObjectTexture.h"
#include "transform.h"
#include "Sound.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void CreateEverything();
	void PlayerInputs();
	void GameUpdate();
	void drawTheGame();
	bool collisionDetection(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);
	void playAudio(unsigned int Source, glm::vec3 pos);
	void AsteroidHitsPlayer();
	void BulletHitsAsteroid();
	void EndGame();
	void DrawSkybox(float size);

	GameDisplay gameDisplay;
	GameState gameState;

	//all meshes/gameobjets being used in the scene
	GameObject player;
	GameObject bullet;
	GameObject asteroid;
	GameObject asteroid2;
	GameObject asteroid3;
	GameObject asteroid4;
	GameObject asteroid5;
	GameObject asteroid6;

	CameraCreator camera;
	Sound audioDevice;

	float counter;
	unsigned int bangSound;
	unsigned int background;
	unsigned int fireSound;

};

