#pragma once
#include "Springs.h"
#include "Geometry.h"

#define EULERSEMI 0
#define VERLET 1
#define DISTANCE 15.0f

class Rope
{
	std::vector<Particle> particles;
	Springs springs;
	int maxParticles;
	int drawnParticles;
	glm::vec3 initPos;
public:
	Rope() {};
	~Rope() {};


	void initParticles(glm::vec3 pos, int _maxParticles, int _drawnParticles, float ke, float l, float kd) {
		maxParticles = _maxParticles;
		drawnParticles = _drawnParticles;
		particles.reserve(maxParticles);
		initPos = pos;
		for (int i = 0; i < maxParticles; i++) {
			Particle p;
			(i == 0) ? p.setFixed(true) : p.setFixed(false);
			p.setPosition(initPos.x + i / DISTANCE, initPos.y, initPos.z);
			p.setBouncing(0.1f);
			if (i != 0) p.setForce(0.0f, 0.0f, -9.8f);
			particles.push_back(p);
		}
		springs.setConstants(ke, l, kd);
		springs.setConstraints(maxParticles);
	};

	void updateParticles(const int &i, GameObject &object, const bool &fixed, const float &dt, const int &updMeth) {
		particles[i].setFixed((fixed) ? true : false);
		if (!particles[i].isFixed()) 
		{
			particles[i].updateParticle(dt, (updMeth == EULERSEMI) ? Particle::UpdateMethod::EulerSemi : Particle::UpdateMethod::Verlet);
			object._translate = particles[i].getCurrentPosition();
			particles[0].setPosition(initPos);
			particles[0].setVelocity(0.0f, 0.0f, 0.0f);
		}
	};

	void updateSprings() {springs.update(particles, drawnParticles);};
	Particle &getParticle(int i) {return particles[i];};
	inline void addParticle() { drawnParticles++; };
	inline void removeParticle() { drawnParticles--; };
	inline int getDrawnParticles() const {return drawnParticles;};
	inline int getNumParticles() const {return drawnParticles;};
};