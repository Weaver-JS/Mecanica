#pragma once
#include "Geometry.h"

//****************************************************
// Plane
//****************************************************

Plane::Plane(const glm::vec3& point, const glm::vec3& normalVect){
	normal = glm::normalize(normalVect);
	dconst = -glm::dot(point, normal);
};

Plane::Plane(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2){
	glm::vec3 v1 = point1 - point0;
	glm::vec3 v2 = point2 - point0;
	normal = glm::normalize(glm::cross(v1, v2));
	dconst = -glm::dot(point0, normal);
};

void Plane::setPosition(const glm::vec3& newPos) {
	dconst = -glm::dot(newPos, normal);
};

void Plane::setPointNormal(const glm::vec3& punt, const glm::vec3& normalVect) {
	normal = glm::normalize(normalVect);
	dconst = -glm::dot(punt, normal);
};

bool Plane::isInside(const glm::vec3& point){
	float dist;
	dist = glm::dot(point, normal) + dconst;
	if (dist > 1.e-7)
		return false;
	else
		return true;
};

float Plane::distPoint2Plane(const glm::vec3& point){
	float dist;
	return dist = glm::dot(point, normal) + dconst;
};

glm::vec3 Plane::closestPointInPlane(const glm::vec3& point){
	glm::vec3 closestP;
	float r = (-dconst - glm::dot(point, normal));
	return closestP = point + r*normal;
};

bool Plane::intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall){
	if (distPoint2Plane(point1)*distPoint2Plane(point2) > 0)	return false;
	float r = (-dconst - glm::dot(point1, normal)) / glm::dot((point2 - point1), normal);
	pTall = (1 - r)*point1 + r*point2;
	return true;
};

std::vector<glm::vec3>& Polygon::getVector()
{
	// TODO: insert return statement here
	return _vertex;
}

bool Polygon::isInside(Particle & particle)
{
	int cont = 0; 


	

	for (int i = 0; i < _vertex.size();i++){
		bool intersect = false;
		bool ignoreTop = false;

		float alpha = -1; 
	
		if (_vertex[i].x >= particle.getCurrentPosition().x){
			if (i == _vertex.size() -1)
			{
				alpha = (particle.getCurrentPosition().y - _vertex[i].y) / (_vertex[0].y - _vertex[i].y);
				if (_vertex[0].y > _vertex[i].y){
					ignoreTop = true;
				}
			}
			else {
				alpha = (particle.getCurrentPosition().y - _vertex[i].y) / (_vertex[i + 1].y - _vertex[i].y);
				if (_vertex[i+1].y >_vertex[i].y){
					ignoreTop = true;
				}
			}
		}
		if (ignoreTop){
			if (alpha >= 0 && alpha < 1)
				cont++;
		}
		else{
			if (alpha > 0 && alpha <= 1)
				cont++;
		}
	}

	return cont % 2 != 0;
}

void Polygon::addVertex(glm::vec3 & newVertex)
{
	_vertex.push_back(newVertex);
	_numOfVertex++;
}

void Polygon::detectPlaneCollision(Particle & particle)
{
	/*
	Detect Collision and Correct particle velocity and position.
	*/
	glm::vec3 particleVector = particle.getCurrentPosition() - particle.getPreviousPosition();
	glm::vec3 normal;
	
	for (int i = 0; i < _vertex.size();i++)
	{
		glm::vec3 uniVector;
		float d;
		if(i == _vertex.size() - 1)
			uniVector = _vertex[0] - _vertex[i];
		else
			uniVector = _vertex[i + 1] - _vertex[i];

		uniVector = glm::normalize(uniVector);
		normal = glm::cross(uniVector, glm::vec3(0, 0, 1)); //glm::vec3(uniVector.y, -uniVector.x, 0)/ glm::length(glm::vec3(uniVector.y,-uniVector.x,0));
		//normal = glm::normalize(normal);
		d = -glm::dot(normal, _vertex[i]);

	float alpha = ((-d) - glm::dot(normal, particle.getPreviousPosition()) ) / (glm::dot(normal, particleVector));

	std::cout <<alpha<< std::endl;
		if (alpha >= 0 && alpha <= 1)
		{
			float disact, disant;

			glm::vec3 newPosition = particle.getPreviousPosition() - (1 + particle.getBouncing()) * (glm::dot(normal ,(particle.getPreviousPosition() + d))* normal);//particle.getPreviousPosition() + d , normal);
			glm::vec3 newVelocity = particle.getVelocity() - (1 + particle.getBouncing() ) * glm::dot(normal ,particle.getVelocity()) * normal;
			
			
			//std::cout << newPosition.x << "\t" << newPosition.y << std::endl;
			particle.setPosition(particle.getPreviousPosition());
			particle.setVelocity(newVelocity);
			particle.updateParticle(0.01f, Particle::UpdateMethod::Verlet);
			break;
		}
	}
	

}
