#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <math.h>

class Plane 
{
	glm::vec3 point;
	glm::vec3 direc1;
	glm::vec3 direc2;
	glm::vec3 normal;
	float d;
public:
	Plane(){};
	~Plane(){};
	// setters
	void setPoint(const float& x, const float& y, const float& z) { point = glm::vec3(x, y, z); };
	void setNormal(const float& x, const float& y, const float& z) {normal = glm::vec3(x, y, z);};
	void setDirec1(const float& x, const float& y, const float& z) {direc1 = glm::vec3(x, y, z);};
	void setDirec2(const float& x, const float& y, const float& z) {direc2 = glm::vec3(x, y, z);};
	void setFrom3Points(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
		direc1 = glm::vec3(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
		direc2 = glm::vec3(p1.x - p3.x, p1.y - p3.y, p1.z - p3.z);
		computeNormal();
		computeD();
	};
	void computeNormal() { normal = glm::cross(direc1, direc2); };
	void computeD() { d = glm::dot(-normal, point); };
	// getters
	glm::vec3 getNormal(){ return normal; };
	float getD() { return d; };
	// functions
	bool isPointColliding(const glm::vec3 &p, const glm::vec3 &q) { return ((glm::dot(normal, q) + d)*(glm::dot(normal, p) + d) <= 0); };
};

