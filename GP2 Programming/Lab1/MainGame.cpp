#include "MainGame.h"
#include "CameraCreator.h"
#include <iostream>
#include <string>

//All transforms
Transform asteroidTransform;
Transform asteroidTransform2;
Transform asteroidTransform3;
Transform asteroidTransform4;
Transform asteroidTransform5;
Transform playerTransform;
Transform bulletTransform;
Transform skyboxTransform;

//variables needed
bool playerFired = false;
int playerScore = 0;


MainGame::MainGame()
{
	gameState = GameState::PLAY;
	GameDisplay* gameDisplay = new GameDisplay(); 
    GameObject* asteroid();
	GameObject* asteroid2();
	GameObject* asteroid3();
	GameObject* asteroid4();
	GameObject* asteroid5();
	GameObject* player();
	GameObject* bullet();
	Sound* audioDevice();
	
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	CreateEverything(); 
	GameUpdate();
}

void MainGame::CreateEverything()
{
	//creates initial setup for the game to be created
	gameDisplay.initialiseDisplay(); 
	
	bangSound = audioDevice.loadAudio("..\\res\\bang.wav");
	background = audioDevice.loadAudio("..\\res\\background.wav");
	
	asteroid.createModel("..\\res\\A2.obj");
	asteroid2.createModel("..\\res\\A2.obj");
	asteroid3.createModel("..\\res\\A2.obj");
	asteroid4.createModel("..\\res\\A2.obj");
	asteroid5.createModel("..\\res\\A2.obj");
	player.createModel("..\\res\\SpaceShip.obj");
	bullet.createModel("..\\res\\spheres.obj");
	
	camera.initialiseCamera(glm::vec3(0, 0, -10), 70.0f, (float)gameDisplay.gainWidth()/gameDisplay.gainHeight(), 0.01f, 1000.0f);
	playerTransform.SetPos(glm::vec3(0, -3, 0));
	
	counter = 1.0f;

}

void MainGame::GameUpdate()
{
	//Update method
	while (gameState != GameState::EXIT)
	{
		PlayerInputs();
		drawTheGame();
		AsteroidHitsPlayer();
		BulletHitsAsteroid();
		EndGame();
		playAudio(background, glm::vec3(0.0f, 0.0f, 0.0f));
	}
}

//checks all players inputs
void MainGame::PlayerInputs() 
{
	SDL_Event event;

	while(SDL_PollEvent(&event)) 
	{
		switch (event.type)
		{
			case SDL_QUIT:
				gameState = GameState::EXIT;
				break;
			case SDL_KEYDOWN:
				SDL_Keycode keyPressed = event.key.keysym.sym;
				//checks if player moves or fires and acts accordingly 
					switch (keyPressed)
					{
					case SDLK_LEFT:
						
						playerTransform.SetPos(glm::vec3(playerTransform.GetPos()->x + .25, playerTransform.GetPos()->y, playerTransform.GetPos()->z));
						
						break;
					case SDLK_RIGHT:
						playerTransform.SetPos(glm::vec3(playerTransform.GetPos()->x - .25, playerTransform.GetPos()->y, playerTransform.GetPos()->z));
						break;
					case SDLK_UP:
						playerTransform.SetPos(glm::vec3(playerTransform.GetPos()->x, playerTransform.GetPos()->y + 0.25, playerTransform.GetPos()->z));
							break;
					case SDLK_DOWN:
						playerTransform.SetPos(glm::vec3(playerTransform.GetPos()->x, playerTransform.GetPos()->y - 0.25, playerTransform.GetPos()->z));
						break;
					case SDLK_SPACE:	
						playerFired = true;
						bulletTransform.SetPos(glm::vec3(playerTransform.GetPos()->x, playerTransform.GetPos()->y + .05, playerTransform.GetPos()->z));
							break;
					
					}									
				break;
			
		}
	}
	
}

//checks if any of the gamobjects "Collides" with each other
bool MainGame::collisionDetection(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		audioDevice.setListener(camera.getPosition(), m1Pos); //add bool to mesh
		
		return true;
	}
	else
	{
		return false;
	}
}

void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
{
	//plays audio
	ALint state; 
	alGetSourcei(Source, AL_SOURCE_STATE, &state);
	
	if (AL_PLAYING != state)
	{
		audioDevice.playAudio(Source, pos);
	}
}

void MainGame::drawTheGame()
{
	
	gameDisplay.wipeDisplay(0.0f, 0.0f, 0.0f, 1.0f);
	DrawSkybox(20);
	GameObjectShader gameobjectShader("..\\res\\shader"); //new gameobjectShader
	GameObjectTexture stoneTexture("..\\res\\bricks.jpg"); //load stoneTexture
	GameObjectTexture waterTexture("..\\res\\water.jpg"); //load stoneTexture
	GameObjectTexture asteroidTexture("..\\res\\AsteroidTexture.jpg"); //loads asteroid texture
	
	//creates all gameobjects, textures them and attaches colliders
	asteroidTransform.SetPos(glm::vec3(2, 3-counter, 0.0));
	asteroidTransform.SetRot(glm::vec3(0.0, 0.0, counter * 5));
	asteroidTransform.SetScale(glm::vec3(0.5, 0.5, 0.5));

	gameobjectShader.Attach();
	gameobjectShader.Update(asteroidTransform, camera);
	asteroidTexture.Bind(0);
	asteroid.draw();
	asteroid.AttachSphereCollider(*asteroidTransform.GetPos(), 0.2f);
	
	asteroidTransform2.SetPos(glm::vec3(.5,  3-counter, 0.0));
	asteroidTransform2.SetRot(glm::vec3(counter * 5, counter * 5, counter * 5));
	asteroidTransform2.SetScale(glm::vec3(0.5, 0.5, 0.5));

	gameobjectShader.Attach();
	gameobjectShader.Update(asteroidTransform2, camera);
	asteroidTexture.Bind(0);
	asteroid2.draw();
	asteroid2.AttachSphereCollider(*asteroidTransform2.GetPos(), 0.62f);

	asteroidTransform3.SetPos(glm::vec3(1, 2-counter, 0.0));
	asteroidTransform3.SetRot(glm::vec3(counter * 5, counter * 5, counter * 5));
	asteroidTransform3.SetScale(glm::vec3(0.5, 0.5, 0.5));

	gameobjectShader.Attach();
	gameobjectShader.Update(asteroidTransform3, camera);
	asteroidTexture.Bind(0);
	asteroid3.draw();
	asteroid3.AttachSphereCollider(*asteroidTransform3.GetPos(), 0.2f);

	asteroidTransform4.SetPos(glm::vec3(-1, 3 -counter, 0.0));
	asteroidTransform4.SetRot(glm::vec3(counter * 5, counter * 5, counter * 5));
	asteroidTransform4.SetScale(glm::vec3(0.5, 0.5, 0.5));

	gameobjectShader.Attach();
	gameobjectShader.Update(asteroidTransform4, camera);
	asteroidTexture.Bind(0);
	asteroid4.draw();
	asteroid4.AttachSphereCollider(*asteroidTransform4.GetPos(), 0.2f);
	

	asteroidTransform5.SetPos(glm::vec3(2, 3 -counter, 0.0));
	asteroidTransform5.SetRot(glm::vec3(counter * 5, counter * 5, counter * 5));
	asteroidTransform5.SetScale(glm::vec3(0.5, 0.5, 0.5));

	gameobjectShader.Attach();
	gameobjectShader.Update(asteroidTransform5, camera);
	asteroidTexture.Bind(0);
	asteroid5.draw();
	asteroid5.AttachSphereCollider(*asteroidTransform5.GetPos(), 0.2f);

	
	playerTransform.SetRot(glm::vec3(-90, 0, 0));
	playerTransform.SetScale(glm::vec3(0.2, 0.2, 0.2));

	gameobjectShader.Attach();
	gameobjectShader.Update(playerTransform, camera);
	asteroidTexture.Bind(0);
	player.draw();
	player.AttachSphereCollider(*playerTransform.GetPos(), 0.2f);

	
	if (playerFired == true)
	{
		//if player has fired create bullet 
		bulletTransform.SetPos(glm::vec3(bulletTransform.GetPos()->x, bulletTransform.GetPos()->y +0.1, bulletTransform.GetPos()->z));
		bulletTransform.SetRot(glm::vec3(counter * 5, counter * 5, counter * 5));
		bulletTransform.SetScale(glm::vec3(0.5, 0.5, 0.5));

		gameobjectShader.Attach();
		gameobjectShader.Update(bulletTransform, camera);
		stoneTexture.Bind(0);
		bullet.draw();
		bullet.AttachSphereCollider(*bulletTransform.GetPos(), 0.2f);
		
	}
	
	counter = counter + 0.05f;

				
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	gameDisplay.changeBuffer();
} 

//checks if the player collides with any of the asteriods 
void MainGame::AsteroidHitsPlayer()
{
	
	if (collisionDetection(asteroid.gainSphereColliderPos(), asteroid.gainSphereColliderRadius(), player.gainSphereColliderPos(), player.gainSphereColliderRadius()) == true)
	{
		player.~GameObject();
		gameState = GameState::EXIT;
		
	}
	if (collisionDetection(asteroid2.gainSphereColliderPos(), asteroid2.gainSphereColliderRadius(), player.gainSphereColliderPos(), player.gainSphereColliderRadius()) == true)
	{
		player.~GameObject();
		gameState = GameState::EXIT;
	}
	if (collisionDetection(asteroid3.gainSphereColliderPos(), asteroid3.gainSphereColliderRadius(), player.gainSphereColliderPos(), player.gainSphereColliderRadius()) == true)
	{
		player.~GameObject();
		gameState = GameState::EXIT;
	}
	
	if (collisionDetection(asteroid4.gainSphereColliderPos(), asteroid4.gainSphereColliderRadius(), player.gainSphereColliderPos(), player.gainSphereColliderRadius()) == true)
	{
		player.~GameObject();
		gameState = GameState::EXIT;
	}
	if (collisionDetection(asteroid5.gainSphereColliderPos(), asteroid5.gainSphereColliderRadius(), player.gainSphereColliderPos(), player.gainSphereColliderRadius()) == true)
	{
		player.~GameObject();
		gameState = GameState::EXIT;
	}	
}
//checks if the bullet collides with the asteriods 
void MainGame::BulletHitsAsteroid()
{

	if (collisionDetection(asteroid.gainSphereColliderPos(), asteroid.gainSphereColliderRadius(), bullet.gainSphereColliderPos(), bullet.gainSphereColliderRadius()) == true)
	{
		//if collided deconstruct the asteroid, play audio and set boolean to false to stop rendering the bullet. 
		asteroid.~GameObject();
		playAudio(bangSound,asteroid.gainSphereColliderPos());		
		playerFired = false;
		playerScore++;
	}
	if (collisionDetection(asteroid2.gainSphereColliderPos(), asteroid2.gainSphereColliderRadius(), bullet.gainSphereColliderPos(), bullet.gainSphereColliderRadius()) == true)
	{
		//if collided deconstruct the asteroid, play audio and set boolean to false to stop rendering the bullet. 
		asteroid2.~GameObject();
		playAudio(bangSound, asteroid2.gainSphereColliderPos());
		playerFired = false;
		playerScore++;
	}
	if (collisionDetection(asteroid3.gainSphereColliderPos(), asteroid3.gainSphereColliderRadius(), bullet.gainSphereColliderPos(), bullet.gainSphereColliderRadius()) == true)
	{
		asteroid3.~GameObject();
		playAudio(bangSound, asteroid3.gainSphereColliderPos());		
		playerFired = false;
		playerScore++;
	}
	if (collisionDetection(asteroid4.gainSphereColliderPos(), asteroid4.gainSphereColliderRadius(), bullet.gainSphereColliderPos(), bullet.gainSphereColliderRadius()) == true)
	{	
		asteroid4.~GameObject();
		
		playerFired = false;
		playerScore++;
		playAudio(bangSound, asteroid4.gainSphereColliderPos());
	}
	if (collisionDetection(asteroid5.gainSphereColliderPos(), asteroid5.gainSphereColliderRadius(), bullet.gainSphereColliderPos(), bullet.gainSphereColliderRadius()) == true)
	{
		asteroid5.~GameObject();
		playerFired = false;
		playAudio(bangSound, asteroid5.gainSphereColliderPos());
		playerScore++;
	}

	

}

void MainGame::EndGame()
{
	if (playerScore == 5)
	{
		gameState = GameState::EXIT;
	}
}


void MainGame::DrawSkybox(float skyboxArea)
{
	GameObjectShader skyboxShader("..\\res\\shader");
	GameObjectTexture SkyBoxTexture("..\\res\\water.jpg") ;

	skyboxTransform.SetPos(glm::vec3(0, 0, 0));
	skyboxShader.Attach();
	SkyBoxTexture.Bind(0);
	skyboxShader.Update(skyboxTransform, camera);
	
	
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(skyboxArea / 2, -skyboxArea / 2, skyboxArea / 2);
	glTexCoord2f(1, 0);
	glVertex3f(-skyboxArea / 2, -skyboxArea / 2, skyboxArea / 2);
	glTexCoord2f(1, 1);
	glVertex3f(-skyboxArea / 2, skyboxArea / 2, skyboxArea / 2);
	glTexCoord2f(0, 1);
	glVertex3f(skyboxArea / 2, skyboxArea / 2, skyboxArea / 2);
	glEnd();

	skyboxShader.Attach();
	SkyBoxTexture.Bind(0);
	skyboxShader.Update(skyboxTransform, camera);
	

	
	glBegin(GL_QUADS);
	//left face
	glTexCoord2f(0, 0);
	glVertex3f(-skyboxArea / 2, -skyboxArea / 2, skyboxArea / 2);
	glTexCoord2f(1, 0);
	glVertex3f(-skyboxArea / 2, -skyboxArea / 2, -skyboxArea / 2);
	glTexCoord2f(1, 1);
	glVertex3f(-skyboxArea / 2, skyboxArea / 2, -skyboxArea / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-skyboxArea / 2, skyboxArea / 2, skyboxArea / 2);
	glEnd();

	skyboxShader.Attach();
	SkyBoxTexture.Bind(0);
	skyboxShader.Update(skyboxTransform, camera);
	
	
	
	glBegin(GL_QUADS);
	//front face
	glTexCoord2f(1, 0);
	glVertex3f(skyboxArea / 2, skyboxArea / 2, -skyboxArea / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-skyboxArea / 2, skyboxArea / 2, -skyboxArea / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-skyboxArea / 2, -skyboxArea / 2, -skyboxArea / 2);
	glTexCoord2f(1, 1);
	glVertex3f(skyboxArea / 2, -skyboxArea / 2, -skyboxArea / 2);
	glEnd();

	skyboxShader.Attach();
	SkyBoxTexture.Bind(0);
	skyboxShader.Update(skyboxTransform, camera);
	
	
	glBegin(GL_QUADS);
	//right face
	glTexCoord2f(0, 0);
	glVertex3f(skyboxArea / 2, -skyboxArea / 2, -skyboxArea / 2);
	glTexCoord2f(1, 0);
	glVertex3f(skyboxArea / 2, -skyboxArea / 2, skyboxArea / 2);
	glTexCoord2f(1, 1);
	glVertex3f(skyboxArea / 2, skyboxArea / 2, skyboxArea / 2);
	glTexCoord2f(0, 1);
	glVertex3f(skyboxArea / 2, skyboxArea / 2, -skyboxArea / 2);
	glEnd();

	skyboxShader.Attach();
	SkyBoxTexture.Bind(0);
	skyboxShader.Update(camera.getPosition(), camera);
	
	
	glBegin(GL_QUADS);                    
	glTexCoord2f(1, 0);
	glVertex3f(skyboxArea / 2, skyboxArea / 2, -skyboxArea / 2);//
	glTexCoord2f(0, 0);
	glVertex3f(-skyboxArea / 2, skyboxArea / 2, -skyboxArea / 2);//
	glTexCoord2f(0, 1);
	glVertex3f(-skyboxArea / 2, skyboxArea / 2, skyboxArea / 2);
	glTexCoord2f(1, 1);
	glVertex3f(skyboxArea / 2, skyboxArea / 2, skyboxArea / 2);
	glEnd();

	skyboxShader.Attach();
	SkyBoxTexture.Bind(0);
	skyboxShader.Update(skyboxTransform, camera);
	
	
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1);
	glVertex3f(skyboxArea / 2, -skyboxArea / 2, -skyboxArea / 2);
	glTexCoord2f(0, 1);
	glVertex3f(-skyboxArea / 2, -skyboxArea / 2, -skyboxArea / 2);
	glTexCoord2f(0, 0);
	glVertex3f(-skyboxArea / 2, -skyboxArea / 2, skyboxArea / 2);
	glTexCoord2f(1, 0);
	glVertex3f(skyboxArea / 2, -skyboxArea / 2, skyboxArea / 2);
	glEnd();
}


