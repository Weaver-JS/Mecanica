#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <math.h>

class Triangle 
{
	glm::vec3 v1, v2, v3;
	glm::vec3 normal;
	float d;
public:
	Triangle(){};
	~Triangle(){};
	//setters
	void setVertices(const glm::vec3& pos) {
		v1 = pos + glm::vec3(0.0f, 1.0f, 0.0f);
		v2 = pos + glm::vec3(1.0f, -1.0f, 0.0f);
		v3 = pos + glm::vec3(-1.0f, -1.0f, 0.0f);
		computeNormal();
		computeD();
	};
	void computeNormal() { normal = glm::normalize(glm::cross(v2 - v1, v3 - v1)); };
	void computeD() { d = glm::dot(-normal, v1); };
	//getters
	glm::vec3 getNormal() { return normal; };
	float getD() { return d; };
	float getTriangleArea(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3){ return (glm::length(glm::cross(v2 - v1, v3 - v1)) / 2); };
	//functions
	bool isPointColliding(glm::vec3 p, glm::vec3 q) {
		if ((glm::dot(normal, q) + d)*(glm::dot(normal, p) + d) <= 0) {
			glm::vec3 v = q - p;
			float alpha = (-d - glm::dot(normal, q)) / glm::dot(normal, v);
			glm::vec3 intersection; intersection = q + alpha*v;
			return	((getTriangleArea(intersection, v2, v3) + getTriangleArea(v1, intersection, v3) +
					getTriangleArea(v1, v2, intersection)) - getTriangleArea(v1, v2, v3) <= 0);
		}
		else return false;
	};
};