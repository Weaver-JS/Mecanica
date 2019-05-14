#include "Particle.h"


Particle::Particle()
{
	m_lifetime = -1;
	id = 0;
}

Particle::Particle(const float& x, const float& y, const float& z) :
m_previousPosition(0, 0, 0), m_velocity(0, 0, 0), m_force(0, 0, 0), m_bouncing(1), m_lifetime(50), m_fixed(false)
{
	m_currentPosition.x = x;
	m_currentPosition.y = y;
	m_currentPosition.z = z;
	id = 0;
}

Particle::Particle(glm::vec3 pos):
m_previousPosition(0, 0, 0), m_velocity(0, 0, 0), m_force(0, 0, 0), m_bouncing(1), m_lifetime(50), m_fixed(false)
{
	m_currentPosition = pos;
	id = 0;
}

Particle::~Particle()
{
}

//setters
void Particle::setPosition(const float& x, const float& y, const float& z)
{
	glm::vec3 pos;
	pos.x = x; pos.y = y; pos.z = z;
	m_currentPosition = pos;
}
void Particle::setPosition(glm::vec3 pos)
{
	m_currentPosition = pos;
}

void Particle::setPreviousPosition(const float& x, const float& y, const float& z)
{
	glm::vec3 pos;
	pos.x = x; pos.y = y; pos.z = z;
	m_previousPosition = pos;
}

void Particle::setPreviousPosition(glm::vec3 pos)
{
	m_previousPosition = pos;
}

void Particle::setForce(const float& x, const float& y, const float& z)
{
	glm::vec3 force;
	force.x = x; force.y = y; force.z = z;
	m_force = force;
}

void Particle::setForce(glm::vec3 force)
{
	m_force = force;
}

void Particle::addForce(const float& x, const float& y, const float& z)
{
	glm::vec3 force;
	force.x += x; force.y += y; force.z += z;
	m_force += force;
}

void Particle::addForce(glm::vec3 force)
{
	m_force += force;
}

void Particle::setVelocity(const float& x, const float& y, const float& z)
{
	glm::vec3 vel;
	vel.x = x; vel.y = y; vel.z = z;
	m_velocity = vel;
}

void Particle::setVelocity(glm::vec3 vel)
{
	m_velocity = vel;
}

void Particle::setBouncing(float bouncing)
{
	m_bouncing = bouncing;
}

void Particle::setLifetime(float lifetime)
{
	m_lifetime = lifetime;
}

void Particle::setFixed(bool fixed)
{
	m_fixed = fixed;
}

//getters
glm::vec3 Particle::getCurrentPosition()
{
	return m_currentPosition;
}

glm::vec3 Particle::getPreviousPosition()
{
	return m_previousPosition;
}

glm::vec3 Particle::getForce()
{
	return m_force;
}

glm::vec3 Particle::getVelocity()
{
	return m_velocity;
}

float Particle::getBouncing()
{
	return m_bouncing;
}

float Particle::getLifetime()
{
	return m_lifetime;
}

bool Particle::isFixed()
{
	return m_fixed;
}

void Particle::updateParticle(const float& dt, UpdateMethod method)
{
	if (!m_fixed)
	{
		m_previousPosition = m_currentPosition;
		switch (method)
		{
		case UpdateMethod::EulerOrig:
		{
			m_currentPosition += m_velocity*dt;
			m_velocity += m_force*dt;
		}
			break;
		case UpdateMethod::EulerSemi:
		{
			m_velocity += m_force*dt;
			m_currentPosition += m_velocity*dt;
		}
			break;
		case UpdateMethod::Verlet:
		{
			m_velocity += m_force*dt;
			m_currentPosition += m_velocity*dt + 0.5f*m_force*(dt*dt);
			//m_velocity = (m_currentPosition - m_previousPosition)/dt;
		}
			break;
		}
	}
	return;
}

void Particle::correctCollision(glm::vec3 normal, float d, float friction) {
	glm::vec3 normVel = glm::dot(normal, m_velocity)*normal;
	glm::vec3 tangVel = m_velocity - normal;
	glm::vec3 newPos = m_previousPosition - glm::dot(1 + m_bouncing, glm::dot(normal, m_currentPosition) + d)*normal;
	glm::vec3 newVel = m_velocity - glm::dot(1 + m_bouncing, glm::dot(normal, m_velocity))*normal;
	newVel += -friction*tangVel;
	setPosition(newPos);
	setVelocity(newVel);
}

void Particle::collideWithSphere(Sphere sphere) {
	if (sphere.isPointColliding(m_currentPosition)) {
		glm::vec3 spNormal = glm::normalize(m_currentPosition - sphere.getCenter());
		float spD = glm::dot(-spNormal, m_currentPosition);
		correctCollision(spNormal, spD, 0.1f);
	}
}

void Particle::collideWithPlane(Plane plane) {
	if (plane.isPointColliding(m_previousPosition, m_currentPosition)) correctCollision(plane.getNormal(), plane.getD(), 0.001f);
}

void Particle::collideWithTriangle(Triangle triangle) {
	if (triangle.isPointColliding(m_previousPosition, m_currentPosition)) correctCollision(triangle.getNormal(), triangle.getD(), 0.001f);
}

void Particle::collideWithBox(Box box) {
	if (box.isPointColliding(m_previousPosition, m_currentPosition, id)) correctCollision(box.getNormal(id), box.getD(id), 0.001f);
}
