#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Model.h"
#include <glad/glad.h>

class Player : public Model {
public:
	int w, a, s, d = 0;
	Player(std::string path);
	void move(double ftime);
private:
	const float RUN_SPEED = 6;
	const float TURN_SPEED = 2.5;
	float currentSpeed = 0;
	float currentTurnSpeed = 0;
	void checkInputs();
};

#endif