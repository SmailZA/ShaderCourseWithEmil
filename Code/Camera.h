#pragma once
#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/quaternion.hpp>
#include "Transform.h"

class Camera
{

public:


	glm::vec3 Position = glm::vec3(0.f);
	float Yaw = 0.f;
	float Pitch = 0.f;
	glm::vec3 GetForwardVector();

	glm::vec3 GetRightVector();
	void AddYaw(float YawAngle);
	void AddPitch(float PitchAngle);

	glm::mat4 GetViewMatrix();

	Transform GetTransform() const;

};