#pragma once

#include <glm\glm.hpp>
#include <AL\al.h>
#include <AL\alc.h>
#include <vector>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <SDL\SDL.h>
#include <glm/gtc/type_ptr.hpp>

class Sound
{
	struct data {
		int sourceIdentity, bufferIdentity;
		char* soundBuffer;
		std::string name;
		data(unsigned int sI, unsigned int bI, char* b, const char* n)
		{
			this->sourceIdentity = sI;
			this->bufferIdentity = bI;
			this->soundBuffer = b;
			this->name = n;
		}

	};

	struct Vector3
	{
		float x, y, z;
	};
	std::vector<data> datas;
	ALCcontext* context;
	ALCdevice *device;
	bool determineEndian();
	int changeToInt(char* soundBuffer, int length);
	char* LoadSoundFile(const char* fn, int& chan, int& samplerate, int& bps, int& size);

public:
	Sound();
	~Sound();
	unsigned int loadAudio(const char* filename);
	void removeAudio(unsigned int id);
	void playAudio(unsigned int id);
	void playAudio(unsigned int id, glm::vec3& pos);
	void stopAudio(unsigned int id);
	void setListener(glm::vec3& pos, glm::vec3& camLookAt);

private:
	
};

