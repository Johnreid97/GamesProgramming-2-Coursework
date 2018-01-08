#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct CameraCreator
{
public:
	CameraCreator()
	{
	}

	//creates the camera
	void initialiseCamera(const glm::vec3& pos, float fov, float aspect, float nearClip, float farClip)
	{
		this->pos = pos;
		this->forward = glm::vec3(0.0f, 0.0f, 1.0f);
		this->up = glm::vec3(0.0f, 1.0f, 0.0f);
		this->projection = glm::perspective(fov, aspect, nearClip, farClip);
	}

	glm::vec3 getPosition()
	{
		return this->pos;
	}

	inline glm::mat4 GetCameraProjection() const
	{
		return projection * glm::lookAt(pos, pos + forward, up);
	}


protected:
private:
	glm::mat4 projection;
	glm::vec3 pos;
	glm::vec3 forward;
	glm::vec3 up;
};


