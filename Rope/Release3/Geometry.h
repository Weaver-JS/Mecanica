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
#define NUM_PARTICLES 15
#define NUM_ROPES 2
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
	~Geometry();

	//Getters
	int getNumVertices(int objectID);
	int getNumGameElements() {return _listOfObjects.size();};
	int getStartParticles() { return numObjects; };
	Vertex & getData(int objectID);
	GameObject & getGameElement(int objectID);
	inline Vertex &getVertex(const int &i, const int &j) { return _verticesData[i][j]; };
	int getNumParticles() { return numParticles; };
	inline int getNumRopes() const { return numRopes; };


	//Functions
	void loadGameElementFromASE(char fileName[100]);
	void loadBasic3DObjects();
	void loadScene(string fileName);
	std::vector<glm::vec3> Geometry::createSphere(float Radius, int Resolution);
};

