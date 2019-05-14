#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Box.h"

class Particle
{
public:
	enum class UpdateMethod : std::int8_t { EulerOrig, EulerSemi, Verlet };

	Particle() {
		m_lifetime = -1;
		id = 0;
	};
	Particle(const float& x, const float& y, const float& z) :
		m_previousPosition(0, 0, 0), m_velocity(0, 0, 0), m_force(0, 0, 0), m_bouncing(1), m_lifetime(50), m_fixed(false)
	{
		m_currentPosition.x = x;
		m_currentPosition.y = y;
		m_currentPosition.z = z;
		id = 0;
	};
	Particle(glm::vec3 pos):m_previousPosition(0, 0, 0), m_velocity(0, 0, 0), m_force(0, 0, 0), m_bouncing(1), m_lifetime(50), m_fixed(false)
	{
		m_currentPosition = pos;
		id = 0;
	};
	~Particle() {};

	//setters
	void setPosition(const float& x, const float& y, const float& z);
	void setPosition(glm::vec3 pos) { m_currentPosition = pos; };
	void setPreviousPosition(const float& x, const float& y, const float& z);
	void setPreviousPosition(glm::vec3 pos) { m_previousPosition = pos; };
	void setVelocity(const float& x, const float& y, const float& z);
	void setVelocity(glm::vec3 vel) { m_velocity = vel; };
	void setForce(const float& x, const float& y, const float& z);
	void setForce(glm::vec3 force) { m_force = force; };
	void setBouncing(float bouncing) { m_bouncing = bouncing; };
	void setLifetime(float lifetime) { m_lifetime = lifetime; };
	void setFixed(bool fixed) { m_fixed = fixed; };

	//getters
	inline glm::vec3 getCurrentPosition() { return m_currentPosition; };
	inline glm::vec3 getPreviousPosition() { return m_previousPosition; };
	inline glm::vec3 getForce() { return m_force; };
	inline glm::vec3 getPreviousVelocity();
	inline glm::vec3 getVelocity() { return m_velocity; };
	inline float getBouncing() { return m_bouncing; };
	inline float getLifetime() { return m_lifetime; };
	inline bool isFixed() { return m_fixed; };

	//functions
	void addForce(glm::vec3 force) { m_force += force; };
	void addForce(const float& x, const float& y, const float& z);
	void updateParticle(const float& dt, UpdateMethod method = UpdateMethod::EulerOrig);
	void correctCollision(glm::vec3 normal, float d, float friction);
	void collideWithSphere(Sphere sphere);
	void collideWithPlane(Plane plane) {
		if (plane.isPointColliding(m_previousPosition, m_currentPosition)) correctCollision(plane.getNormal(), plane.getD(), 0.001f);
	};
	void collideWithTriangle(Triangle triangle) {
		if (triangle.isPointColliding(m_previousPosition, m_currentPosition)) correctCollision(triangle.getNormal(), triangle.getD(), 0.001f);
	};
	void collideWithBox(Box box) {
		if (box.isPointColliding(m_previousPosition, m_currentPosition, id)) correctCollision(box.getNormal(id), box.getD(id), 0.8f);
	};

private:
	glm::vec3 m_currentPosition;
	glm::vec3 m_previousPosition;
	glm::vec3 m_force;
	glm::vec3 m_velocity;

	float m_bouncing;
	float m_lifetime;
	bool  m_fixed;
	int id;
};

