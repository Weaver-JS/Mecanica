#pragma once
#include "Rope.h"

class Cloth
{
	std::vector<Rope> ropes;
	Springs springs;
	int _numRopes;
	glm::vec3 initPos;

public:
	Cloth() {};
	~Cloth() {};

	//Getters
	inline int getNumRopes() const { return _numRopes; };
	inline Rope& getRope(int i) { return ropes[i]; };

	//Setters

	//Funcions
	void initCloth(glm::vec3 pos, int numParticlesxRope, int numRopes, float ke, float l, float kd) {
		initPos = pos;
		_numRopes = numRopes;
		for (int i = 0; i < _numRopes; i++) {
			Rope r;
			r.initParticles(glm::vec3(0.0f, 0.0f + (i*0.2f), 1.0f), numParticlesxRope, numParticlesxRope - 1, 500.0f, 0.01f, 5.0f);
			ropes.push_back(r);
		}
	};
};

