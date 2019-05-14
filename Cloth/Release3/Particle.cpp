#include "Particle.h"


//setters
void Particle::setPosition(const float& x, const float& y, const float& z)
{
	glm::vec3 pos;
	pos.x = x; pos.y = y; pos.z = z;
	m_currentPosition = pos;
}

void Particle::setPreviousPosition(const float& x, const float& y, const float& z)
{
	glm::vec3 pos;
	pos.x = x; pos.y = y; pos.z = z;
	m_previousPosition = pos;
}

void Particle::setForce(const float& x, const float& y, const float& z)
{
	glm::vec3 force;
	force.x = x; force.y = y; force.z = z;
	m_force = force;
}

void Particle::addForce(const float& x, const float& y, const float& z)
{
	glm::vec3 force;
	force.x += x; force.y += y; force.z += z;
	m_force += force;
}

void Particle::setVelocity(const float& x, const float& y, const float& z)
{
	glm::vec3 vel;
	vel.x = x; vel.y = y; vel.z = z;
	m_velocity = vel;
}

//Functions
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
		correctCollision(spNormal, spD, 0.6f);
	}
}
