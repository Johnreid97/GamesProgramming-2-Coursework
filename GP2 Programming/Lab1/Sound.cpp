#include "Sound.h"


Sound::Sound()
{
	device = alcOpenDevice(NULL); //open sound card
	if (device == NULL)
	{
		std::cout << "cannot open sound card" << std::endl;
	}

	context = alcCreateContext(device, NULL);

	if (context == NULL)
	{
		std::cout << "cannot open context" << std::endl;
	}

	alcMakeContextCurrent(context);
}

Sound::~Sound()
{
	for (unsigned int i = 0; i < datas.size(); i++)
	{
		alDeleteSources(1, (unsigned int*)&datas[i].sourceIdentity);
		if (datas[i].bufferIdentity != -1)
		{
			alDeleteBuffers(1, (unsigned int*)&datas[i].bufferIdentity);
			delete[] datas[i].soundBuffer;
		}
	}
	alcDestroyContext(context);
	alcCloseDevice(device);
}

bool Sound::determineEndian()
{
	int a = 1;
	return !((char*)&a)[0];
}

int Sound::changeToInt(char* soundBuffer, int length)
{
	int a = 0;
	if (!determineEndian())
		for (int i = 0; i < length; i++)
			((char*)&a)[i] = soundBuffer[i];
	else
		for (int i = 0; i < length; i++)
			((char*)&a)[3 - i] = soundBuffer[i];
	return a;
}

char* Sound::LoadSoundFile(const char* fn, int& chan, int& samplerate, int& bps, int& size)
{
	char soundBuffer[4];
	std::ifstream in(fn, std::ios::binary);
	in.read(soundBuffer, 4);
	in.read(soundBuffer, 4);
	in.read(soundBuffer, 4); 
	in.read(soundBuffer, 4);	
	in.read(soundBuffer, 4); 
	in.read(soundBuffer, 2); 
	in.read(soundBuffer, 2);
	chan = changeToInt(soundBuffer, 2);
	in.read(soundBuffer, 4);
	samplerate = changeToInt(soundBuffer, 4);
	in.read(soundBuffer, 4);
	in.read(soundBuffer, 2);
	in.read(soundBuffer, 2);
	bps = changeToInt(soundBuffer, 2);
	in.read(soundBuffer, 4);
	in.read(soundBuffer, 4);
	size = changeToInt(soundBuffer, 4);

	char* soundData = new char[size];
	in.read(soundData, size);
	return soundData;
}

unsigned int Sound::loadAudio(const char* filename)
{
	bool found = false;
	unsigned int sourceIdentity, bufferIdentity;
	char* soundData = NULL;
	for (unsigned int i = 0; i < datas.size(); i++)
	{
		if (datas[i].name == filename && datas[i].bufferIdentity != -1)
		{
			bufferIdentity = datas[i].bufferIdentity;
			found = true;
			break;
		}
	}
	if(!found)
		{	
			int channel, sampleRate, bps, size;
			soundData = LoadSoundFile(filename, channel, sampleRate, bps, size);
			unsigned int format;
			alGenBuffers(1, &bufferIdentity);

			if (channel == 1)
			{
				if (bps == 8)
				{
					format = AL_FORMAT_MONO8;
				}
				else
				{
					format = AL_FORMAT_MONO16;
				}
			}
			else
			{
				if (bps == 8)
				{
					format = AL_FORMAT_STEREO8;
				}
				else
				{
					format = AL_FORMAT_STEREO16;
				}

			}

			alBufferData(bufferIdentity, format, soundData, size, sampleRate);
		}
	alGenSources(1, &sourceIdentity);
	alSourcei(sourceIdentity, AL_BUFFER, bufferIdentity);
	alSourcef(sourceIdentity, AL_REFERENCE_DISTANCE, 1.0f);
	datas.push_back(data(sourceIdentity, (!found ? bufferIdentity : -1), soundData, filename));
	return sourceIdentity;
}

void Sound::removeAudio(unsigned int id) 
{}

void Sound::playAudio(unsigned int id) 
{
	alSourcePlay(id);
}
void Sound::playAudio(unsigned int id, glm::vec3& pos) 
{
	alSource3f(id, AL_POSITION, pos.x, pos.y, pos.z);
	alSourcePlay(id);
}
void Sound::stopAudio(unsigned int id) 
{
	alSourceStop(id);
}
void Sound::setListener(glm::vec3& pos, glm::vec3& camLookAt) 
{
	alListener3f(AL_POSITION, pos.x, pos.y, -pos.z);
	float orinet[6] = { camLookAt.x,camLookAt.y,camLookAt.z,0,1,0 };
	alListenerfv(AL_ORIENTATION, orinet);
}