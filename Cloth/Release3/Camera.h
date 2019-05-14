#pragma once
#include "ErrorManagement.h"
#include <GL/glew.h>			//The OpenGL Extension Wrangler
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>

using namespace std;

class Camera
{
private:
	float _aspectRation;
	glm::mat4 _projectionMatrix;
	glm::mat4 _viewMatrix;
	float _FOV;
	float _far;
	float _near;
	float _projactionWidth;
	float _projectionHeight;
	glm::vec3 _cameraPos;
	glm::vec3 _cameraFront;
	glm::vec3 _cameraUp;

public:
	Camera(int screenWidth, int screenHeight);
	~Camera();

	//Getters
	inline glm::mat4 GetViewMatrix() const{ return _viewMatrix; };
	inline glm::mat4 GetProjectionMatrix() const{ return _projectionMatrix; };
	inline glm::vec3 GetCameraPos() const { return _cameraPos; };
	inline glm::vec3 GetCameraFront() const { return _cameraFront; };
	inline glm::vec3 GetCameraUp() const { return _cameraUp; };

	//Setters
	inline void SetCameraPos(glm::vec3 newPos) { _cameraPos = newPos; };
	inline void SetCameraFront(glm::vec3 newFront) { _cameraFront = newFront; };
	inline void SetCameraUp(glm::vec3 newUp) { _cameraUp = newUp; };
	inline void SetProjectionWith(float pWith) { _projactionWidth = pWith; };
	inline void SetProjectionHeight(float pHeight) { _projectionHeight = pHeight; };

	void setOrthogtaphicProjection();
	void setPrespectiveProjection();
	void updateCamera(glm::vec3 newPos, glm::vec3 newFront);
	void updateCamera();
	void moveCamera(glm::vec3 newPos, glm::vec3 newFront);

};

