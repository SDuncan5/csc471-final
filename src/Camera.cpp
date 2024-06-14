#include "Camera.h"

using namespace glm;
using namespace std;


//amera::Camera(Model player)

Camera::Camera() {

}

/* Camera::Camera(shared_ptr<Player> player)
{
	this->player = player;
	//	w = a = s = d = 0;
	//	rotAngle = 0.0;
	//	pitch = 0;
	//	yaw = 0;
	//	pos = glm::vec3(0, 0, 0);
	//	front = glm::vec3(0, 0, -1);
	//	up = glm::vec3(0, 1, 0);
} */

glm::mat4 Camera::process(double ftime)
{
	//float speed = 0;
	//if (w == 1)
	//{
	//	speed = 10 * ftime;
	//	pos = pos + speed * front;
	//}
	//else if (s == 1)
	//{
	//	speed = -10 * ftime;
	//	pos = pos + speed * front;
	//}
	////float yangle=0;
	//if (a == 1) {
	//	speed = -10 * ftime;
	//	pos = pos + speed * normalize(cross(front, up));
	//	//yangle = -3 * ftime;
	//}
	//else if (d == 1) {
	//	//yangle = 3 * ftime;
	//	speed = 10 * ftime;
	//	pos = pos + speed * normalize(cross(front, up));
	//}

	//float horizontalDistance = calculateHorizontalDistance();
	//float verticalDistance = calculateVerticalDistance();
	//calculateCameraPosition(horizontalDistance, verticalDistance);

	//yaw = 180 - (this->player->getRotationAngle() + angleAroundPlayer);
	//yaw = fmod(yaw, 360);

	// cout << "Angle = " << angle << endl;
	pos.x = player->getLocation().x + radius * cos(radians(angle));
	pos.y = player->getLocation().y + 2.5;
	pos.z = player->getLocation().z + radius * sin(radians(angle));


	//cout << "Pitch: " << pitch << " Yaw: " << yaw << endl;

	//float front_y = sin(radians(pitch));
	//if (front_y < 80 && front_y > -80) {
	//	front.y = sin(radians(pitch));
	//}
	/*front.x = cos(radians(yaw)) * cos(radians(pitch));
	front.y = -1 * sin(radians(pitch));
	front.z = sin(radians(yaw)) * cos(radians(pitch));*/


	/*rotAngle += yangle;
	glm::mat4 R = glm::rotate(glm::mat4(1), rotAngle, glm::vec3(0, 1, 0));
	glm::vec4 dir = glm::vec4(0, 0, speed,1);
	dir = dir*R;
	pos += glm::vec3(dir.x, dir.y, dir.z);
	glm::mat4 T = glm::translate(glm::mat4(1), pos); */
	//return R * T;


	// front is pos + front (front = (0, 0, -1)
	//return glm::lookAt(pos, vec3(dir.x, dir.y, dir.z), vec3(0, 1, 0));
	//return glm::lookAt(pos, pos + front, up);
	return glm::lookAt(pos, player->getLocation(), up);
	//return glm::lookAt(pos, player->getLocation(), up);
	//return glm::lookAt(pos, pos + player->getLocation(), up);
}

float Camera::calculateHorizontalDistance() {
	return (float)(distanceFromPlayer * cos(radians(pitch)));
}

float Camera::calculateVerticalDistance() {
	return (float)(distanceFromPlayer * sin(radians(pitch)));
}

void Camera::calculateCameraPosition(float horizDistance, float verticDistance) {
	float theta = this->getPlayer()->getRotationAngle() + radians(angleAroundPlayer); // in radians
	cout << "Theta: " << theta << endl;
	float offsetX = (float) horizDistance * sin(theta);
	float offsetZ = (float) horizDistance * cos(theta);
	pos.x = this->getPlayer()->getLocation().x - offsetX;
	pos.x = this->getPlayer()->getLocation().z - offsetZ;
	pos.y = this->getPlayer()->getLocation().y + verticDistance;
}

// CREATE PLAYER SETUP CLASS
// GET PLAYER POS AND MAKE A DEFAULT FRONT AND POSITION BEHIND PLAYER

//void Camera::calculateZoom() {
//	float zoomLevel = 0;
//}

//void Camera::calculateAngleAroundPlayer() {
//
//}
