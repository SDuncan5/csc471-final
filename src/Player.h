#pragma once
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Model.h"

class Player : public Model {
public:
	Player(std::string path);
	//void move();
private:
	const int RUN_SPEED = 20;
	const int TURN_SPEED = 160;
};

#endif