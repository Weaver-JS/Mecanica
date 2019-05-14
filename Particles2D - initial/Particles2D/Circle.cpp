#include "Circle.h"



Circle::Circle()
{
}

Circle::Circle(int ndiv, float radius, glm::vec3 center)
{
	_ndiv = ndiv;
	_radius = radius;
	_angle = 2 * M_PI / ndiv;
	pointsVector = new glm::vec3[ndiv];
	for (int i = 0; i < ndiv;i++)
	{
		pointsVector[i] = glm::vec3(_radius * cos(_angle), _radius * sin(_angle), 0.0f);
		_angle += _angle;
	}
}


Circle::~Circle()
{
}

glm::vec3 & Circle::getPointsArray(int i)
{
	return pointsVector[i];
}

int Circle::getNumberOfDivision()
{
	return _ndiv;
}

void Circle::setPosition(const glm::vec3 & newPos)
{
	_center = newPos;
}

bool Circle::isInside(const glm::vec3 & point)
{
	return false;
}
