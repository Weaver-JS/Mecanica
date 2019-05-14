#pragma once
//Third-party libraries
#include <GL/glew.h>			//The OpenGL Extension Wrangler
#include <glm/glm.hpp>			//OpenGL Mathematics 

struct Position {
	GLfloat  x;
	GLfloat  y;
	GLfloat  z;
};

struct ColorRGBA8 {
	ColorRGBA8() :r(0), g(0), b(0), a(0) {	}
	ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) : r(R), g(G), b(B), a(A)  {}
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

struct Vertex {
	//Position position;
	glm::vec3 position = glm::vec3(0);
	ColorRGBA8 color;
	void setPosition(float  x, float  y, float  z) {
		position.x = x;
		position.y = y;	 
		position.z = z;
	}

	void setPosition(glm::vec3 pos) {
		position.x = pos.x;
		position.y = pos.y;
		position.z = pos.z;
		//position = pos;
	}

	/*void setPosition(GLfloat  x, GLfloat  y, GLfloat  z) {
		position.x = x;
		position.y = y;
		position.z = z;
	}

	void setPosition(glm::vec3 pos) {
		position.x = (GLfloat) pos.x;
		position.y = (GLfloat) pos.y;
		position.z = (GLfloat) pos.z;
	}  */
	
	void setColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
		color.r = r;
		color.g = g;
		color.b = b;
		color.a = a;
	}
};