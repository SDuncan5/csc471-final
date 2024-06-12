#pragma once
#ifndef _MODEL_H_
#define _MODEL_H_

#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include <tiny_obj_loader/tiny_obj_loader.h>

class Shape;
class Program;

class Model {

public:
	Model(std::string path);
	virtual ~Model();
	void draw(const std::shared_ptr<Program> prog) const;
	std::vector<std::shared_ptr<Shape>> getMesh() const { return mesh_shapes; };
	glm::vec3 getLocation() const { return location; };
	void setLocation(glm::vec3 loc) { location = loc; };
	glm::vec3 getScale() const { return scale; };
	void setScale(glm::vec3 sca) { scale = sca; };
	glm::vec3 getRotationAxis() const { return rotAxis; };
	void setRotationAxis(glm::vec3 rot) { rotAxis = rot; };
	float getRotationAngle() const { return rotAngle; };
	void setRotationAngle(float rotAng) { rotAngle = rotAng; };
protected:
	std::vector<std::shared_ptr<Shape>> mesh_shapes;
	glm::vec3 location = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);
	glm::vec3 rotAxis = glm::vec3(0, 1, 0);
	float rotAngle = 0.0; // IN RADIANS :)
private:
};

#endif