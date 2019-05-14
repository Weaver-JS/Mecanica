#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <math.h>

class Sphere 
{
	glm::vec3 center;
	float radius;
public:
	Sphere(){};
	Sphere(glm::vec3 _center, float _radius){
		center = _center;
		radius = _radius;
	};
	~Sphere(){};
	//setters
	void setCenter(glm::vec3 c) { center = c; };
	void setRadius(float r) { radius = r; };
	//getters
	glm::vec3 getCenter() { return center; };
	float getRadius() { return radius; };
	//functions
	bool isPointColliding(const glm::vec3 &p) {
		float v = glm::length(center - p);
		return (glm::dot(v, v) <= glm::dot(radius, radius));
	}
};

