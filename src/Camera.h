#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/gtc/type_ptr.hpp>
#include "Player.h"

class Camera {

public:
	glm::vec3 pos = glm::vec3(0, 0, 0);
	glm::vec3 front = glm::vec3(0, 0, -1);
	glm::vec3 up = glm::vec3(0, 1, 0);
	float pitch = glm::radians(20.0);
	float yaw = 0;
	float rotAngle = 0.0;
	float angle = 0.0;
	float radius = 5;
	int w, a, s, d = 0;
	float getDistanceFromPlayer() const { return distanceFromPlayer; };
	void setDistanceFromPlayer(float dist) { distanceFromPlayer = dist; };
	float getAngleAroundPlayer() const { return angleAroundPlayer; };
	void setAngleAroundPlayer(float angle) { angleAroundPlayer = angle; };
	std::shared_ptr<Player> getPlayer() const { return player; };
	void setPlayer(std::shared_ptr<Player> player_ptr) { player = player_ptr; };

	Camera();
	//Camera(std::shared_ptr<Player> player);
	glm::mat4 process(double ftime);
	//void calculateZoom();

private:
	std::shared_ptr<Player> player;
	float distanceFromPlayer = 10;
	float angleAroundPlayer = 0;
	float calculateHorizontalDistance();
	float calculateVerticalDistance();
	void calculateCameraPosition(float horizDistance, float verticDistance);
	
};

#endif