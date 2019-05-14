#pragma once
#include <glm\glm.hpp>
#include <vector>
#include <math.h>
#include <iostream>
#include "Particle.h"

struct Geometry{
	virtual void setPosition(const glm::vec3& newPos) = 0;
	virtual bool isInside(const glm::vec3& point) = 0;
};

struct Plane : public Geometry {
	glm::vec3 normal;
	float dconst;
	Plane(){};
	~Plane() {};
	Plane(const glm::vec3& point, const glm::vec3& normalVect);
	Plane(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2);

	void setPosition(const glm::vec3& newPos);
	void setPointNormal(const glm::vec3& point, const glm::vec3& normal);
	bool isInside(const glm::vec3& point);
	float distPoint2Plane(const glm::vec3& point);
	glm::vec3 closestPointInPlane(const glm::vec3& point);
	bool intersecSegment(const glm::vec3& punt1, const glm::vec3& punt2, glm::vec3& pTall);
};	

struct Circle : public Geometry {
	glm::vec3 center;
	float radius;

	Circle() {};
	~Circle() {};
	Circle(const glm::vec3& c, const float r) : center(c), radius(r) {};

	//Getters
	inline glm::vec3 getCenter() const {return center; };
	inline float getRadius() const {return radius; };

	//Setters
	inline void setCenter(const glm::vec3& c) { center = c; };
	inline void setRadius(const float r) { radius = r; };

	//Functions
	void setPosition(const glm::vec3& newPos) {};
	bool isInside(const glm::vec3& point) { return true; };
};

struct Polygon : public Geometry {
	std::vector<glm::vec3> _vertex;
	int _numOfVertex;

	Polygon() : _numOfVertex(0){};
	~Polygon() {};
	
	//Getters
	std::vector<glm::vec3> & getVector();
	//Functions
	void setPosition(const glm::vec3& newPos) {};
	bool isInside(const glm::vec3& point) { return true; };
	bool isInside(Particle &particle);
	void addVertex(glm::vec3 & newVertex);
	void detectPlaneCollision(Particle & particle);
	void eulerSolver(Particle &particle);
	bool intersecSegment(const glm::vec3& punt1, const glm::vec3& punt2, glm::vec3& pTall);
};


