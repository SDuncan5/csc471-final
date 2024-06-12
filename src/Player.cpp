#include "Program.h"
#include "Player.h"
#include "WindowManager.h"


using namespace std;
using namespace glm;

Player::Player(std::string path)
	: Model(path) {

}

//void Player::checkInputs(int key, int action) {
//	if(key == GLFW_KEY_W && action == GLFW_PRESS) {
//		this->currentSpeed = RUN_SPEED;
//	}
//	else if (key == GLFW_KEY_S && action == GLFW_PRESS) {
//		this->currentSpeed = -RUN_SPEED;
//	}
//	else {
//		this->currentSpeed = 0;
//	}
//
//	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
//		this->currentTurnSpeed = TURN_SPEED;
//	}
//	else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
//		this->currentTurnSpeed = -TURN_SPEED;
//	}
//	else {
//		this->currentTurnSpeed = 0;
//	}
//}

void Player::checkInputs() {
	if (w == 1) {
		this->currentSpeed = RUN_SPEED;
	}
	else if (s == 1) {
		this->currentSpeed = -RUN_SPEED;
	}
	else {
		this->currentSpeed = 0;
	}

	if (a == 1) {
		this->currentTurnSpeed = TURN_SPEED;
	}
	else if (d == 1) {
		this->currentTurnSpeed = -TURN_SPEED;
	}
	else {
		this->currentTurnSpeed = 0;
	}
}

void Player::move(double ftime) {
	checkInputs();
	// turn player based on turn speed
	this->rotAxis = vec3(0, 1, 0);
	this->rotAngle = (rotAngle + currentTurnSpeed * ftime);
	// move player based on run speed
	float distance = currentSpeed * ftime;

	/*float dx = distance * sin(radians(this->rotAngle));
	float dz = distance * cos(radians(this->rotAngle));*/
	float dx = distance * sin(this->rotAngle);
	float dz = distance * cos(this->rotAngle);
	(this->location).x = (this->location).x + dx;
	(this->location).z = (this->location).z + dz;

	cout << "Player rotAngle = " << this->rotAngle << endl;

	/*float dx = distance * sin(radians(this->rotAngle));
	float dz = distance * cos(radians(this->rotAngle));
	(this->location).x = (this->location).x + dx;
	(this->location).z = (this->location).z + dz;*/
}

