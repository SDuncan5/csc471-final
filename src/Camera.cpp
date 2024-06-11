#include "Camera.h"

using namespace glm;

//amera::Camera(Model player)
Camera::Camera()
{
	/*w = a = s = d = 0;
	rotAngle = 0.0;
	pitch = 0;
	yaw = 0;
	pos = glm::vec3(0, 0, 0);
	front = glm::vec3(0, 0, -1);
	up = glm::vec3(0, 1, 0);*/
	//this->player = player;
}

glm::mat4 Camera::process(double ftime)
{
	float speed = 0;
	if (w == 1)
	{
		speed = 10 * ftime;
		pos = pos + speed * front;
	}
	else if (s == 1)
	{
		speed = -10 * ftime;
		pos = pos + speed * front;
	}
	//float yangle=0;
	if (a == 1) {
		speed = -10 * ftime;
		pos = pos + speed * normalize(cross(front, up));
		//yangle = -3 * ftime;
	}
	else if (d == 1) {
		//yangle = 3 * ftime;
		speed = 10 * ftime;
		pos = pos + speed * normalize(cross(front, up));
	}

	//float front_y = sin(radians(pitch));
	//if (front_y < 80 && front_y > -80) {
	//	front.y = sin(radians(pitch));
	//}
	front.x = cos(radians(yaw)) * cos(radians(pitch));
	front.y = -1 * sin(radians(pitch));
	front.z = sin(radians(yaw)) * cos(radians(pitch));


	/*rotAngle += yangle;
	glm::mat4 R = glm::rotate(glm::mat4(1), rotAngle, glm::vec3(0, 1, 0));
	glm::vec4 dir = glm::vec4(0, 0, speed,1);
	dir = dir*R;
	pos += glm::vec3(dir.x, dir.y, dir.z);
	glm::mat4 T = glm::translate(glm::mat4(1), pos); */
	//return R * T;


	// front is pos + front (front = (0, 0, -1)
	//return glm::lookAt(pos, vec3(dir.x, dir.y, dir.z), vec3(0, 1, 0));
	return glm::lookAt(pos, pos + front, up);

}
