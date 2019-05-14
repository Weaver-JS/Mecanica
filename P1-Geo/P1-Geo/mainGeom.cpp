#pragma once;
#include <iostream>  //per escriure a consola
#include <string>    //per poder passar el nom al cout
#include <fstream>   //per escriure a fitxer
#include <chrono>    //per comptar el temps d'execucio 
#include <vector>    //per crear vectors d'estructures tipus: Point, Lines, etc.

#include "Geometry.h"
#define PRINT_VEC3(_v) std::cout << _v.x << " / " << _v.y << " / " << _v.z << std::endl;
#define FPRINT_VEC3(_v) fileOut << _v.x << " / " << _v.y << " / " << _v.z << "\n";


void main() {
   
	float a = glm::radians(30.0f);
	float b = glm::radians(45.0f);
	glm::vec3 u(std::cos(a)* std::cos(b), std::sin(b), std::sin(a) * std::cos(b));

	
	//creem un vector de dimenció 10 dividit amb segments de 0.01.
	glm::vec3 v = glm::normalize(u);
	v *=0.01f;
	glm::vec3  u2 = v;
	// Gestionar un vector d'estructures
	const float nPunts = 10.0f / 0.01f;
	std::vector<Point> punts(nPunts);
	punts[0] = glm::vec3(0.0f);
	for (int i = 1; i < nPunts; i++) {
		punts[i].position += punts[i - 1].position+u2;
	}

	glm::vec3 pt(0.0f);


	//SPHERE
	std::cout << "\tSPHERE:" << std::endl;
	Sphere sphere(glm::vec3(1.0f, 2.0f, 1.0f), 3.0f);
	bool exit = false;
	int index = 1;
	while (!exit)
	{
		if (sphere.intersecSegment(punts[index-1].position, punts[index].position, pt)) exit = true;
		else if (index >= nPunts-1) {
			exit = true;
			index = -1;
		}
		else index++;
	}

	if (index < 0) std::cout << "Sphere not found" << std::endl;
	else {
		std::cout << "Segment " << index << " intersec with sphere:" << std::endl;
		PRINT_VEC3(pt); std::cout << std::endl;
	} 
	

	//PLANE
	std::cout << "\n\tPLANE:"<< std::endl;
	Plane plane(glm::vec3(1.0f, 2.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 1.0f));

	float dist = 0;
	exit = false;
	index = 1;
	dist = plane.distPoint2Plane(punts[index].position);
	while (!exit) {
		if (dist > 0.0f) exit = true;
		else if (index >= nPunts-1) {
			exit = true;
			index = -1;
		}
		else {
			index++;
			dist = plane.distPoint2Plane(punts[index].position);
		}
	}

	if (index < 0) std::cout << "Plane not found" << std::endl;
	else {
		std::cout << "Segment " << index << " intersect with plane." << std::endl;
		plane.intersecSegment(punts[index-1].position, punts[index].position, pt);
		PRINT_VEC3(pt); std::cout << std::endl;
	}
   
	
	//TRIANGE
	std::cout << "\n\tTRIANGE:" << std::endl;
	Triangle triangle(glm::vec3(1.0f, 2.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 2.0f), glm::vec3(0.0f, -1.0f, 0.0f));

	dist = 0;
	exit = false;
	index = 1;
	dist = triangle.distPoint2Plane(punts[index].position);
	while (!exit) {
		if (dist > 0.0f) exit = true;
		else if (index >= nPunts - 1) {
			exit = true;
			index = -1;
		}
		else {
			index++;
			dist = triangle.distPoint2Plane(punts[index].position);
		}
	}

	if (index < 0) std::cout << "Triangle not found" << std::endl;
	else {
		if (triangle.intersecSegmentTriange(punts[index - 1].position, punts[index].position, pt)) {
			std::cout << "Segment " << index << " intersect with triangle." << std::endl;
			PRINT_VEC3(pt); std::cout << std::endl;
		}
		else std::cout << "Triangle not found but segment " << index << " intersect with plane." << std::endl;

	}  


	std::cout << "\n\n-*-*-*-*-*-EXTRA WORK-*-*-*-*-*:" << std::endl;
	//CYLINDER TEST
	std::cout << "\n\tCYLINDER:" << std::endl;
	Cylinder cylinder(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f, -3.0f, 0.0f), 1.0f);
	glm::vec3 p1(0.0f, 0.0f, 0.0f);
	std::cout << "Is the point:"; 
	PRINT_VEC3(p1);
	std::cout<<"Inside?:"<<cylinder.isInside(p1)<<std::endl;
	
	 p1 = glm::vec3(1.1f, 8.7f, 4.2f);
	std::cout << "Is the point:";
	PRINT_VEC3(p1);
	std::cout << "Inside?:" << cylinder.isInside(p1) << std::endl;
	
	//CAPSULE TEST
	std::cout << "\n\tCAPSULE:" << std::endl;
	Capsule capsule(glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f, -3.0f, 0.0f), 1.0f);
	p1 = glm::vec3(0.5f, 3.2f, 0.2f);
	std::cout << "Is the point:";
	PRINT_VEC3(p1);
	std::cout << "Inside?:" << capsule.isInside(p1) << std::endl;

	p1 = glm::vec3(1.1f, 8.7f, 4.2f);
	std::cout << "Is the point:";
	PRINT_VEC3(p1);
	std::cout << "Inside?:" << capsule.isInside(p1) << std::endl;

		std::cout << "\n\tCUBE:" << std::endl;
		//CUBE TEST
		glm::vec3 cP1 = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cP2 = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 cP3 = glm::vec3(1.0f, 0.0f, 1.0f);
		Cube proof(cP1, cP2, cP3);

		
	
		p1 = glm::vec3(0.5f, 0.5f, 0.5f);
		std::cout << "Is the point:";
		PRINT_VEC3(p1);
		std::cout << "Inside?:" << cylinder.isInside(p1) << std::endl;

		p1 = glm::vec3(1.1f, 8.7f, 4.2f);
		std::cout << "Is the point:";
		PRINT_VEC3(p1);
		std::cout << "Inside?:" << cylinder.isInside(p1) << "\n"<<std::endl;



		std::cout << "Do the segment created by Points:" << std::endl;
	
		p1 = glm::vec3(2, -1, 0);
		glm::vec3 p2(-1, 2, -1);
		glm::vec3 pTall(0.0f);
		PRINT_VEC3(p1);
		PRINT_VEC3(p2);

		std::cout <<"Do they intersect?"<< proof.intersecSegment(p1, p2, pTall) << std::endl;
		std::cout << "The point of intersection is:" << std::endl;
		PRINT_VEC3(pTall)

	system("pause");

}