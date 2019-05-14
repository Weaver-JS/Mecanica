#pragma once
#include "GeometryPlane.h"
#define _USE_MATH_DEFINES
#include <math.h>
class Circle : public Geometry
{
private:
	float _radius;
	int _ndiv;
	float _angle;
	glm::vec3 _center;
	glm::vec3* pointsVector;
public:
	Circle();
	Circle(int ndiv, float radius, glm::vec3 center);
	~Circle();
	glm::vec3 & getPointsArray(int i);
	int getNumberOfDivision();
	 void setPosition(const glm::vec3& newPos);
	 bool isInside(const glm::vec3& point);
}; 

