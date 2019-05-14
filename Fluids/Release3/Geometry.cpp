#include "Geometry.h"
#include "ErrorManagement.h"
#include <iostream>
#include <fstream>

using namespace std;

Geometry::Geometry() {
	_verticesData.resize(NUM_OBJECTS);
	_numVertices.resize(NUM_OBJECTS);
	first_particle = last_particle = 0;
}

void Geometry::loadScene(string fileName)
{
	numObjects = NUM_OBJECTS;
	numParticles = NUM_PARTICLES;
	numRopes = NUM_ROPES;
	//Game Info
	int size = numObjects + (numParticles*numRopes);
	_verticesData.resize(size);
	_numVertices.resize(size);
	//Sphere
	GameObject go;
	go._objectType = SPHERE;
	go._translate = glm::vec3(0.0f, 0.0f, 0.0f);
	go._angle = 0.0f;
	go._rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	go._scale = glm::vec3(1.0f, 1.0f, 1.0f);
	go._mode = 2;
	_listOfObjects.push_back(go);
	//Cube
	go._objectType = CUBE;
	go._translate = glm::vec3(0.0f, 2.3f, 0.0f);
	go._angle = 0.0f;
	go._rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	go._scale = glm::vec3(1.0f, 1.0f, 1.0f);
	go._mode = 2;
	_listOfObjects.push_back(go);

	//Particles
	for (int i = 0; i < numRopes; i++) {
		for (int j = 0; j < numParticles; j++) {
			GameObject newObject;
			newObject._objectType = POINT;
			newObject._translate = glm::vec3(0.0f, 0.0f, 2.0f);
			newObject._angle = 90.0f;
			newObject._rotation = glm::vec3(1.0f, 0.0f, 0.0f);
			newObject._scale = glm::vec3(0.1f);
			newObject._mode = 4;
			_listOfObjects.push_back(newObject);
		}
	}
	//Quads
	/*for (int i = 0; i < numRopes; i++) {
		for (int j = 0; j < numParticles; j++) {
			GameObject newObject;
			newObject._objectType = LINE;
			newObject._translate = glm::vec3(0.0f, 0.0f, 2.0f);
			newObject._angle = 0;
			newObject._rotation = glm::vec3(0.0f, 0.0f, 0.0f);
			newObject._scale = glm::vec3(0.03f, 0.03f, 0.03f);
			newObject._mode = 4;
			_listOfObjects.push_back(newObject);
		}
	}  */
	
	/*
			for (int i = 0; i < maxParticles; i++) {
				GameObject newObject;
				newObject._objectType = 1;
				newObject._translate = glm::vec3(0.0f, 0.0f, 2.0f);
				newObject._angle = 0;
				newObject._rotation = glm::vec3(0.0f, 0.0f, 0.0f);
				newObject._scale = glm::vec3(0.03f, 0.03f, 0.03f);
				newObject._mode = 4;
				_listOfObjects.push_back(newObject);
			}	
			for (int i = 0; i < maxParticles - 1; i++) {
				GameObject newObject;
				newObject._objectType = i + 5;
				newObject._translate = glm::vec3(0.0f, 0.0f, 0.0f);
				newObject._angle = 0;
				newObject._rotation = glm::vec3(0.0f, 0.0f, 0.0f);
				newObject._scale = glm::vec3(1.0f, 1.0f, 1.0f);
				newObject._mode = 3;
				_listOfObjects.push_back(newObject);
			}
		}
	}
*/
}

std::vector<glm::vec3> Geometry::createSphere(float Radius, int Resolution)
{
	const long double PI = 3.141592653589793238462643383279502884L;

	// vertice positions
	std::vector<glm::vec3> v;

	// iniatiate the variable we are going to use
	float X1, Y1, X2, Y2, Z1, Z2;
	float inc1, inc2, inc3, inc4, Radius1, Radius2;

	for (int w = 0; w < Resolution; w++) {
		for (int h = (-Resolution / 2); h < (Resolution / 2); h++) {

			inc1 = (w / (float)Resolution) * 2 * (float)PI;
			inc2 = ((w + 1) / (float)Resolution) * 2 * (float)PI;

			inc3 = (h / (float)Resolution)*(float)PI;
			inc4 = ((h + 1) / (float)Resolution)*(float)PI;

			X1 = sin(inc1);
			Y1 = cos(inc1);
			X2 = sin(inc2);
			Y2 = cos(inc2);

			// store the upper and lower radius, remember everything is going to be drawn as triangles
			Radius1 = Radius*cos(inc3);
			Radius2 = Radius*cos(inc4);

			Z1 = Radius*sin(inc3);
			Z2 = Radius*sin(inc4);

			// insert the triangle coordinates
			v.push_back(glm::vec3(Radius1*X1, Z1, Radius1*Y1));
			v.push_back(glm::vec3(Radius1*X2, Z1, Radius1*Y2));
			v.push_back(glm::vec3(Radius2*X2, Z2, Radius2*Y2));

			v.push_back(glm::vec3(Radius1*X1, Z1, Radius1*Y1));
			v.push_back(glm::vec3(Radius2*X2, Z2, Radius2*Y2));
			v.push_back(glm::vec3(Radius2*X1, Z2, Radius2*Y1));
		}
	}
	return v;
}  

void Geometry::loadBasic3DObjects()
{
	//CUBE
	_numVertices[CUBE] = CUBE_EDGES;
	_verticesData[CUBE] = new Vertex[CUBE_EDGES];

		//Bottom
	_verticesData[CUBE][0].setPosition(MIN, MIN, MIN);
	_verticesData[CUBE][1].setPosition(MAX, MIN, MIN);
	_verticesData[CUBE][2].setPosition(MAX, MAX, MIN);
	_verticesData[CUBE][3].setPosition(MIN, MIN, MIN);
	_verticesData[CUBE][4].setPosition(MAX, MAX, MIN);
	_verticesData[CUBE][5].setPosition(MIN, MAX, MIN);
		//Top
	_verticesData[CUBE][6].setPosition(MIN, MIN, MAX);
	_verticesData[CUBE][7].setPosition(MAX, MIN, MAX);
	_verticesData[CUBE][8].setPosition(MAX, MAX, MAX);
	_verticesData[CUBE][9].setPosition(MIN, MIN, MAX);
	_verticesData[CUBE][10].setPosition(MAX, MAX, MAX);
	_verticesData[CUBE][11].setPosition(MIN, MAX, MAX);
		//Left
	_verticesData[CUBE][12].setPosition(MIN, MIN, MIN);
	_verticesData[CUBE][13].setPosition(MAX, MIN, MIN);
	_verticesData[CUBE][14].setPosition(MAX, MIN, MAX);
	_verticesData[CUBE][15].setPosition(MIN, MIN, MIN);
	_verticesData[CUBE][16].setPosition(MAX, MIN, MAX);
	_verticesData[CUBE][17].setPosition(MIN, MIN, MAX);
		//Ritht
	_verticesData[CUBE][18].setPosition(MIN, MAX, MIN);
	_verticesData[CUBE][19].setPosition(MAX, MAX, MIN);
	_verticesData[CUBE][20].setPosition(MAX, MAX, MAX);
	_verticesData[CUBE][21].setPosition(MIN, MAX, MIN);
	_verticesData[CUBE][22].setPosition(MAX, MAX, MAX);
	_verticesData[CUBE][23].setPosition(MIN, MAX, MAX);
		//Front
	_verticesData[CUBE][24].setPosition(MAX, MIN, MIN);
	_verticesData[CUBE][25].setPosition(MAX, MAX, MIN);
	_verticesData[CUBE][26].setPosition(MAX, MAX, MAX);
	_verticesData[CUBE][27].setPosition(MAX, MIN, MIN);
	_verticesData[CUBE][28].setPosition(MAX, MAX, MAX);
	_verticesData[CUBE][29].setPosition(MAX, MIN, MAX);
		//Back
	_verticesData[CUBE][30].setPosition(MIN, MIN, MIN);
	_verticesData[CUBE][31].setPosition(MAX, MIN, MIN);
	_verticesData[CUBE][32].setPosition(MAX, MAX, MIN);
	_verticesData[CUBE][33].setPosition(MIN, MIN, MIN);
	_verticesData[CUBE][34].setPosition(MAX, MAX, MIN);
	_verticesData[CUBE][35].setPosition(MIN, MAX, MIN);

	for (int j = 0; j < CUBE_EDGES; j++) _verticesData[CUBE][j].setColor(0, 0, 255, 255);

	//POINT
	_numVertices[POINT] = 1;
	_verticesData[POINT] = new Vertex[_numVertices[1]];
	_verticesData[POINT][0].setPosition(0.0f, 0.0f, 0.0f);
	_verticesData[POINT][0].setColor(0, 255, 100, 255);

	//SHERE
	std::vector<glm::vec3> v = createSphere(SPHERE_RADIUS, SPHERE_RESOLUTION);
	for (int j = SPHERE; j < 4; j++) {
		_numVertices[j] = v.size();
		_verticesData[j] = new Vertex[_numVertices[j]];
		for (int i = 0; i < _numVertices[j]; i++)_verticesData[j][i].setPosition(v[i].x, v[i].y, v[i].z);
	}
	for (int i = 0; i < _numVertices[SPHERE]; i++) _verticesData[SPHERE][i].setColor(200, 0, 0, 255);
	for (int i = 0; i < _numVertices[SPHERE+1]; i++) _verticesData[SPHERE+1][i].setColor(0, 0, 200, 255);
	 

	//QUAD
	float floor = -2.0f;
	_numVertices[QUAD] = QUAD_EDGES;
	_verticesData[QUAD] = new Vertex[QUAD_EDGES];
	_verticesData[QUAD][0].setPosition(1.0f, floor, 1.0f);
	_verticesData[QUAD][1].setPosition(1.0f, floor, 0.0f);
	_verticesData[QUAD][2].setPosition(0.0f, floor, 0.0f);
	_verticesData[QUAD][3].setPosition(0.0f, floor, 1.0f);
	for (int i = 0; i < _numVertices[4]; i++) _verticesData[4][i].setColor(255, 255, 255, 255);

	// LINE
	for (int i = 5; i < NUM_OBJECTS - 6; i++) {
		_numVertices[i] = 2;
		_verticesData[i] = new Vertex[_numVertices[i]];
		for (int j = 0; j < _numVertices[i]; j++) _verticesData[i][j].setPosition(0.0f, 0.0f, 0.0f);
		for (int j = 0; j < _numVertices[i]; j++) _verticesData[i][j].setColor(100, 50, 255, 255);
	}
}

