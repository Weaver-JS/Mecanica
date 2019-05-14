#pragma once
#include "Particle.h"

class Springs
{
	std::vector<glm::vec3> constraints;
	float ke;
	float l;
	float kd;
public:
	Springs(){};
	~Springs() {};
	Springs(int maxParticles, float ke, float l, float kd) {
		this->ke = ke;
		this->l = l;
		this->kd = kd;
		constraints.resize(maxParticles);
	}
	void setConstraints(int maxParticles) {
		constraints.resize(maxParticles);
	}
	void setConstants(const float &ke, const float &l, const float &kd) {
		this->ke = ke;
		this->l = l;
		this->kd = kd;
	}
	void update(std::vector<Particle> &particles, const int &drawnParticles) {
		for (int i = 0; i < drawnParticles - 1; i++) {
			glm::vec3 vPos = particles[i + 1].getCurrentPosition() - particles[i].getCurrentPosition();
			glm::vec3 vVel = particles[i + 1].getVelocity() - particles[i].getVelocity();
			float vPosLength = glm::length(vPos);
			constraints[i] = (ke*(vPosLength - l) + kd * glm::dot(vVel, glm::normalize(vPos)))*glm::normalize(vPos);
		}
		(!particles[0].isFixed()) ? particles[0].setForce(constraints[0]) : particles[0].setForce(glm::vec3(0.0f));
		for (int i = 1; i < drawnParticles; i++) {
			if (!particles[i].isFixed()) {
				particles[i].setForce(-constraints[i - 1] + constraints[i]);
				particles[i].addForce(0.0f, 0.0f, -9.8f);
			}
		}
	}
};

