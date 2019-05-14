#pragma once
#include "Geometry.h"

//****************************************************
// Point
//****************************************************

Point::Point(){
};

Point::Point(const float& x, const float& y, const float& z){
	position.x = x;
	position.y = y;
	position.z = z;
};

Point::Point(const glm::vec3& newPos){
	position = newPos;
};

void Point::setPosition(const glm::vec3& newPos){
	position = newPos;

};

void Point::setPosition(const float& x, const float& y, const float& z){
	position.x = x;
	position.y = y;
	position.z = z;
};

float Point::distPoint2Point(const Point& punt){
	float dist;
	return dist = glm::length(this->position - punt.position);
}

bool Point::isInside(const glm::vec3& point){ return false; };

Point Point::pointInSegment(const Point& q, const float& alfa){
	Point r = (1 - alfa)*(this->position) + alfa*q.position;
	return r;
};


//****************************************************
// Line
//****************************************************

Line::Line()
{
};

Line::Line(const glm::vec3& origen, const glm::vec3& vector){
	point.position = origen;
	direction = vector;
};

Line::Line(const Point& origen, const Point& extrem){
	point.position = origen.position;
	direction = extrem.position - origen.position;
};

void Line::setPosition(const glm::vec3& newPos){
	point.position = newPos;
};

void Line::setDirection(const glm::vec3& newDir){
	direction = newDir;
};

bool Line::isInside(const Point& point){ //not yet implemented
	return (distLine2Point(point) < APROXIMATION_ERROR_UP && distLine2Point(point) > APROXIMATION_ERROR_DOWN);
};

bool Line::isInside(const glm::vec3& punt){ //not yet implemented
	return (distLine2Point(punt) < APROXIMATION_ERROR_UP && distLine2Point(punt) > APROXIMATION_ERROR_DOWN);
};

float Line::distLine2Point(const Point& point){
	Point closestPoint = closestPointInLine(point); // Calculamos el punto más cercano,con respecto a la recta, al pundo dado.
	glm::vec3 vector; // Al tener el punto más cercano ya calculado, al restar los puntos tendremos la distancia entre recta y punto. 
	vector -= point.position;
	return glm::length(vector); // Modulo ya que querremos la distancia. 
}

glm::vec3 Line::closestPointInLine(const Point& point){
	glm::vec3 newVector = this->point.position - point.position;
	glm::vec3 distToPoint = (glm::dot(direction, newVector) / (std::pow(glm::length(direction), 2)))* direction;
	return distToPoint;																							
};

float Line::distLine2Line(const Line& line){
	glm::vec3 PQ = line.point.position - direction;
	return glm::length(glm::dot(PQ, glm::cross(direction, line.direction))) / glm::length(glm::cross(direction, line.direction));
};


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

void Plane::setPosition(const glm::vec3& newPos){
	dconst = -glm::dot(newPos, normal);
};

bool Plane::isInside(const glm::vec3& point){
	return (distPoint2Plane(point) < APROXIMATION_ERROR_UP && distPoint2Plane(point) > APROXIMATION_ERROR_DOWN);
};

float Plane::distPoint2Plane(const glm::vec3& point) {
	return glm::dot(normal.x, point.x) + glm::dot(normal.y, point.y) + glm::dot(normal.z, point.z) + dconst / glm::length(normal);
	//return (glm::dot(normal.x, point.x) + glm::dot(normal.y, point.y) + glm::dot(normal.z, point.z) + dconst) / glm::length(normal);
}
					   
glm::vec3 Plane::closestPointInPlane(const glm::vec3& point) {
	float alfa = (-dconst - glm::dot(normal, point)) / glm::dot(normal, normal);
	glm::vec3 closestPoint = point + alfa * normal;
	return closestPoint;
};

bool Plane::intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall) {
	glm::vec3 directorRecta = point2 - point1;
	/*Line l(point1, directorRecta);
	if (intersecLinePlane(l, pTall)) {
		return !(glm::length(pTall - point1) + glm::length(pTall - point2) > glm::length(directorRecta));
	}
	else return false;	*/
	
	float alpha = (normal.x * point1.x + normal.y * point1.y + normal.z * point1.z + dconst) / (-normal.x * directorRecta.x - normal.y*directorRecta.y - normal.z * directorRecta.z);
	pTall = point1 + alpha *directorRecta;
	return (glm::length(pTall - point1) + glm::length(pTall - point2) == glm::length(directorRecta)); 
};

bool Plane::intersecLinePlane(const Line& line, glm::vec3& pTall) {

	float i = glm::length(glm::dot(normal, line.direction));
	float x = (-dconst - glm::dot(normal, line.point.position));


	float alpha = (-dconst - glm::dot(normal, line.point.position)) / glm::length(glm::dot(normal, line.direction));
	pTall = line.point.position + (alpha * normal);
	return (glm::length(pTall - line.point.position) + glm::length(pTall - (line.point.position+line.direction)) == glm::length(line.direction)); 
};

bool Plane::whereIsThatPoint(const glm::vec3 & point)
{
	if (distPoint2Plane(point) > APROXIMATION_ERROR_UP)
	{
		return true;
	}
	return false;
}



//****************************************************
// Triangle
//****************************************************

Triangle::Triangle(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2){
	vertex1 = point0;
	vertex2 = point1;
	vertex3 = point2;
	normal = glm::normalize(glm::cross(vertex2 - vertex1, vertex3 - vertex2));
	dconst = -glm::dot(point0, normal);
};

void Triangle::setPosition(const glm::vec3& newPos){
	glm::vec3 baryc;
	baryc = (vertex1 + vertex2 + vertex3) / 3.0f;     //displacement of the barycenter 
	glm::vec3 disp = newPos - baryc; //is only considered
	vertex1 = vertex1 + disp;
	vertex2 = vertex2 + disp;
	vertex3 = vertex3 + disp;
};

bool Triangle::isInside(const glm::vec3& point){
	//Area our triangle
	glm::vec3 u(vertex1.x - vertex2.x, vertex1.y - vertex2.y, vertex1.z - vertex2.z);
	glm::vec3 v(vertex1.x - vertex3.x, vertex1.y - vertex3.y, vertex1.z - vertex3.z);
	glm::vec3 dot = glm::cross(u, v);
	float area = 1.0f/ 2.0f * glm::length(dot);

	//Area new 3 triangles from point
	//---1---
	v = point - vertex1;
	u = point - vertex2;
	 dot = glm::cross(u, v);
	float area1 = 1.0f / 2.0f * glm::length(dot);
	//---2---
	v = point - vertex2;
	u = point - vertex3;
	dot = glm::cross(u, v);
	float area2 = 1.0f / 2.0f * glm::length(dot);
	//---3---
	v = point - vertex3;
	u = point - vertex1;
	dot = glm::cross(u, v);
	float area3 = 1.0f / 2.0f * glm::length(dot);

	return area >= (area1 + area2 + area3);
};

bool Triangle::intersecSegmentTriange(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall){
	//Comprovar que intersecte amb el pla.
	bool intersectWithPlane = this->intersecSegment(point1, point2, pTall);
	//Si intersecte amb el pla mirar si el punt de tall està a dins.
	if (intersectWithPlane) return false;
	else return isInside(pTall);
};


//****************************************************
// Sphere
//****************************************************

Sphere::Sphere(const glm::vec3& point, const float& radious){
	center = point;
	radi = radious;
};

void Sphere::setPosition(const glm::vec3& newPos){
	center = newPos;
};

bool Sphere::isInside(const glm::vec3& point){
	float dist = glm::length(point - center);
	if (dist > radi) return false;
	return true;
};

bool Sphere::intersecSegment(const glm::vec3& point1, const glm::vec3& point2, glm::vec3& pTall){
	//VEctor director
	glm::vec3 v = point2 - point1;
	//Calculo apha's
	float a = glm::dot(v, v);
	float b = 2 * glm::dot(v, (point1 - center));
	float c = glm::dot(center, center) + glm::dot(point1, point1) - 2 * glm::dot(point1, center) - glm::dot(radi, radi);

	float sqrtCont = b*b - 4 * a * c;
	if(sqrtCont<0.0f){
		return false;
	}

	float alpha = (-b + sqrt(sqrtCont)) / (2 * a);
	if(alpha<=1 && alpha>=-1){
		pTall = point1 + (alpha *v);
		return true;
	}
	return false;
};


//****************************************************
// Cylinder
//****************************************************
Cylinder::Cylinder(const glm::vec3 & point1, const glm::vec3 & point2, const float & radious)
{
	this->point1 = point1;
	this->point2 = point2;
	this->radi = radious;
}

void Cylinder::setPosition(const glm::vec3 & newPos)
{
	point1 += newPos;
	point2 += newPos;
}

bool Cylinder::isInside(const glm::vec3 & point)
{
	Line line(point1, point2 - point1);
	float dist =line.distLine2Point(point);

	if (dist <= radi) {
		glm::vec3 clPoint = line.closestPointInLine(point);
		float dist = glm::length( point2 - point1);
		float dist1 = glm::length(clPoint - point1);
		float dist2 = glm::length(clPoint - point2);
		if (dist <= dist1 + dist2) return true;
	}	
	return false;
}

bool Cylinder::intersecSegment(const glm::vec3 & p1, const glm::vec3 & p2, glm::vec3 & pTall)
{
	glm::vec3 segment(p2 - p1);
	Line cylinderLine(this->point1, this->point2 - this->point1);
		
	glm::vec3 halfSegmentVector(segment*0.5f);
	Point centerPoint(p1 + halfSegmentVector);
	float distToCenterCylinder = cylinderLine.distLine2Point(centerPoint.position);

	if (distToCenterCylinder <= 0.001f) {
		pTall = centerPoint.position;
		return true;
	}
	else if (this->intersecSegment(p1, centerPoint.position, pTall))
		return true;
	else if (this->intersecSegment(centerPoint.position, p2, pTall))
		return true;
	
	else return false;
}

//****************************************************
// Capsule
//****************************************************

Capsule::Capsule(const glm::vec3 & point1, const glm::vec3 & point2, const float & radious): Cylinder(point1,point2, radious)
{
}

void Capsule::setPosition(const glm::vec3 & newPos)
{
	point1 += newPos;
	point2 += newPos;
}

bool Capsule::isInside(const glm::vec3 & point)
{
	Line line(point1, point2 - point1);
	float dist = line.distLine2Point(point);
	Sphere shpereUp(this->point1, this->radi);
	Sphere shpereDow(this->point2, this->radi);
	Cylinder cylinder(this->point1, this->point2, this->radi);

	if (cylinder.isInside(point)) return true;
	else if (shpereUp.isInside(point))return true;
	else if (shpereDow.isInside(point))return true;
	return false;
}


//****************************************************
// Cube
//****************************************************
Cube::Cube(const glm::vec3 & point1, const glm::vec3 & point2, const glm::vec3 & point3)
{
	points[0] = point1;
	points[1] = point2;
	points[2] = point3;
	float distance = glm::length(point2 - point1);
	points[3] = point1;
	points[3].z += distance;
	points[4] = point1;
	points[4].y += distance;
	points[5] = points[4];
	points[5].x += distance;
	points[6] = points[5];
	points[6].z += distance;
	points[7] = points[6];
	points[7].x -= distance;
	_bottom = Plane(points[0], points[1], points[2]);
	_front = Plane(points[0], points[1], points[4]);
	_left = Plane(points[0], points[3], points[4]);
	_right = Plane(points[1], point3, points[5]);
	_back = Plane(points[2], points[3], points[7]);
	_top = Plane(points[5], points[4], points[7]);

}
void Cube::setPosition(const glm::vec3 & newPos)
{
	for (int i = 0; i < 8; i++)
	{
		points[i] = newPos;
	}
}
bool Cube::isInside(const glm::vec3 & point)
{
	if (!_bottom.isInside(point) && !_top.isInside(point) && !_left.isInside(point) && !_right.isInside(point) && !_front.isInside(point) && !_back.isInside(point))
		return true;
	return false;
}
bool Cube::intersecSegment(const glm::vec3 & point1, const glm::vec3 & point2, glm::vec3 & pTall)
{
	if (_top.whereIsThatPoint(point1) != _top.whereIsThatPoint(point2))
	{
		if (_top.intersecSegment(point1, point2, pTall) && glm::length(pTall - point1) <glm::length(pTall - point2))
			return true;
	}

	if (_bottom.whereIsThatPoint(point1) != _bottom.whereIsThatPoint(point2))
	{
		if (_bottom.intersecSegment(point1, point2, pTall) && glm::length(pTall - point1) <glm::length(pTall - point2))
			return true;
	}
	if (_right.whereIsThatPoint(point1) != _right.whereIsThatPoint(point2))
	{
		if (_right.intersecSegment(point1, point2, pTall) && glm::length(pTall - point1) <glm::length(pTall - point2))
			return true;
	}

	if (_left.whereIsThatPoint(point1) != _left.whereIsThatPoint(point2))
	{
		if (_left.intersecSegment(point1, point2, pTall) && glm::length(pTall - point1) <glm::length(pTall - point2))
			return true;
	}
	if (_front.whereIsThatPoint(point1) != _front.whereIsThatPoint(point2))
	{
		if (_front.intersecSegment(point1, point2, pTall) && glm::length(pTall - point1) <glm::length(pTall - point2))
			return true;
	}

	if (_back.whereIsThatPoint(point1) != _back.whereIsThatPoint(point2))
	{
		if (_back.intersecSegment(point1, point2, pTall) && glm::length(pTall - point1) <glm::length(pTall - point2))
			return true;
	}
	return false;
}
