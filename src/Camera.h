#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/gtc/type_ptr.hpp>
#include "Model.h"

class Camera {

public:
	glm::vec3 pos = glm::vec3(0, 0, 0);
	glm::vec3 front = glm::vec3(0, 0, -1);
	glm::vec3 up = glm::vec3(0, 1, 0);
	float pitch = 0;
	float yaw = 0;
	float rotAngle = 0.0;
	int w, a, s, d = 0;

	//Camera(Model player);
	Camera();
	glm::mat4 process(double ftime);

private:
	//Model player;
};

#endif