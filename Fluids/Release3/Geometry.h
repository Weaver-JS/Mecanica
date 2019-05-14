#pragma once
#include "Vertex.h"
#include <vector>
#include "GameObject.h"	 
#include "ObjectLoader.h"
#include "parserAse.h"
//#include <time.h> 

#define CUBE 0
#define POINT 1
#define SPHERE 2
#define QUAD 4
#define LINE 5
#define NUM_OBJECTS 2
#define NUM_PARTICLES 50
#define NUM_ROPES 50
#define MAX 0.5f
#define MIN -0.5f
#define CUBE_EDGES 36
#define QUAD_EDGES 4
#define SPHERE_RADIUS 0.5
#define SPHERE_RESOLUTION 15


class Geometry
{
	std::vector <Vertex*> _verticesData;
	std::vector <int> _numVertices;
	std::vector <GameObject> _listOfObjects;
	ObjectLoader _objectLoader;

	int numObjects;
	int numParticles;
	int numRopes;


public:
	int first_particle, last_particle;
	Geometry();
	~Geometry() {};

	glm::vec3 spherePosition = glm::vec3(0.0f, 2.0f, 0.0f);

	//Getters
	inline int getNumVertices(int objectID) { return _numVertices[objectID]; };
	int getNumGameElements() {return _listOfObjects.size();};
	int getStartParticles() { return numObjects; };
	inline Vertex & getData(int objectID) { return *_verticesData[objectID]; };
	inline GameObject & getGameElement(int objectID) { return _listOfObjects[objectID]; };
	inline Vertex &getVertex(const int &i, const int &j) { return _verticesData[i][j]; };
	int getNumParticles() { return numParticles; };
	inline int getNumRopes() const { return numRopes; };


	//Functions
	void loadGameElementFromASE(char fileName[100]) { _objectLoader.loadAse(fileName, _numVertices, _verticesData); };
	void loadBasic3DObjects();
	void loadScene(string fileName);
	std::vector<glm::vec3> Geometry::createSphere(float Radius, int Resolution);
};

