#pragma once
#include <glm/glm.hpp>
#include <iostream>
#include <math.h>


class Box 
{
	glm::vec3 v1, v2, v3, v4, v5, v6, v7, v8;
	glm::vec3 n1, n2, n3, n4, n5, n6;
	float d1, d2, d3, d4, d5, d6;

public:
	Box(){};
	~Box(){};
	//setters
	void setVertices(const glm::vec3& pos) {
		v1 = pos + glm::vec3(-0.5f, -0.5f, -0.5f);
		v2 = pos + glm::vec3(0.5f, -0.5f, -0.5f);
		v3 = pos + glm::vec3(-0.5f, 0.5f, -0.5f);
		v4 = pos + glm::vec3(0.5f, 0.5f, -0.5f);
		v5 = pos + glm::vec3(-0.5f, -0.5f, 0.5f);
		v6 = pos + glm::vec3(0.5f, -0.5f, 0.5f);
		v7 = pos + glm::vec3(-0.5f, 0.5f, 0.5f);
		v8 = pos + glm::vec3(0.5f, 0.5f, 0.5f);
		computeNormals();
		computeDs();
	};
	void computeNormals() { 
		n1 = glm::normalize(glm::cross(v2 - v1, v3 - v1));
		n2 = glm::normalize(glm::cross(v5 - v1, v2 - v1));
		n3 = glm::normalize(glm::cross(v5 - v1, v3 - v1));
		n4 = glm::normalize(glm::cross(v7 - v3, v4 - v3));
		n5 = glm::normalize(glm::cross(v6 - v2, v4 - v2));
		n6 = glm::normalize(glm::cross(v6 - v5, v7 - v5));
	};
	void computeDs() { 
		d1 = glm::dot(-n1, v1);
		d2 = glm::dot(-n2, v1);
		d3 = glm::dot(-n3, v1);
		d4 = glm::dot(-n4, v3);
		d5 = glm::dot(-n5, v2);
		d6 = glm::dot(-n6, v5);
	};
	//getters
	glm::vec3 getNormal(const int& i) {
		if (i == 1) return n1;
		else if (i == 2) return n2;
		else if (i == 3) return n3;
		else if (i == 4) return n4;
		else if (i == 5) return n5;
		else if (i == 6) return n6;
		else return glm::vec3(0.0f, 0.0f, 0.0f);
	};
	float getD(const int& i) {
		if (i == 1) return d1;
		else if (i == 2) return d2;
		else if (i == 3) return d3;
		else if (i == 4) return d4;
		else if (i == 5) return d5;
		else if (i == 6) return d6;
		else return 0;
	};
	//functions
	bool pointToPlane(glm::vec3 p, glm::vec3 q, glm::vec3 n, float d) { return (glm::dot(n, q) + d)*(glm::dot(n, p) + d) <= 0; };
	glm::vec3 computeIntersection(glm::vec3 p, glm::vec3 q, glm::vec3 n, float d){ return q + (-d - glm::dot(n, q)) / glm::dot(n, q - p)*(q - p); };
	bool isPointColliding(glm::vec3 p, glm::vec3 q, int &id) {
		glm::vec3 intersection;
		if (pointToPlane(p,q,n1,d1)) {
			id = 1;
			intersection = computeIntersection(p, q, n1, d1);
			return (intersection.x >= v1.x && intersection.y >= v1.y && intersection.z <= v1.z &&
				intersection.x <= v2.x && intersection.y >= v2.y && intersection.z <= v2.z &&
				intersection.x >= v3.x && intersection.y <= v3.y && intersection.z <= v3.z &&
				intersection.x <= v4.x && intersection.y <= v4.y && intersection.z <= v4.z);
		}
		else if (pointToPlane(p, q, n2, d2)) {
			id = 2;
			intersection = computeIntersection(p, q, n1, d1);
			return (intersection.x >= v1.x && intersection.y <= v1.y && intersection.z >= v1.z &&
					intersection.x <= v2.x && intersection.y <= v2.y && intersection.z >= v2.z &&
					intersection.x >= v5.x && intersection.y <= v5.y && intersection.z <= v5.z &&
					intersection.x <= v6.x && intersection.y <= v6.y && intersection.z <= v6.z);
		}
		else if (pointToPlane(p, q, n3, d3)) {
			id = 3;
			intersection = computeIntersection(p, q, n3, d3);
			return (intersection.x <= v1.x && intersection.y >= v1.y && intersection.z >= v1.z &&
					intersection.x <= v3.x && intersection.y <= v3.y && intersection.z >= v3.z &&
					intersection.x <= v5.x && intersection.y >= v5.y && intersection.z <= v5.z &&
					intersection.x <= v7.x && intersection.y <= v7.y && intersection.z <= v7.z);
		}
		else if (pointToPlane(p, q, n4, d4)) {
			id = 4;
			intersection = computeIntersection(p, q, n4, d4);
			return (intersection.x >= v3.x && intersection.y <= v3.y && intersection.z >= v3.z &&
					intersection.x <= v4.x && intersection.y <= v4.y && intersection.z >= v4.z &&
					intersection.x >= v7.x && intersection.y <= v7.y && intersection.z <= v7.z &&
					intersection.x <= v8.x && intersection.y <= v8.y && intersection.z <= v8.z);
		}
		else if (pointToPlane(p, q, n5, d5)) {
			id = 5;
			intersection = computeIntersection(p, q, n5, d5);
			return (intersection.x <= v2.x && intersection.y >= v2.y && intersection.z >= v2.z &&
					intersection.x <= v4.x && intersection.y <= v4.y && intersection.z >= v4.z &&
					intersection.x <= v6.x && intersection.y >= v6.y && intersection.z <= v6.z &&
					intersection.x <= v8.x && intersection.y <= v8.y && intersection.z <= v8.z);
		}
		else if (pointToPlane(p, q, n6, d6)) {
			id = 6;
			intersection = computeIntersection(p, q, n6, d6);
			return (intersection.x >= v5.x && intersection.y >= v5.y && intersection.z >= v5.z &&
					intersection.x <= v6.x && intersection.y >= v6.y && intersection.z >= v6.z &&
					intersection.x >= v7.x && intersection.y <= v7.y && intersection.z >= v7.z &&
					intersection.x <= v8.x && intersection.y <= v8.y && intersection.z >= v8.z);
		}
		else return false;
	};
};

