#include "Camera.h"

Camera::Camera(int screenWidth, int screenHeight):
	_near(0.1f),
	_far(300.0f),
	_projactionWidth(5.0f),
	_projectionHeight(5.0f),
	_FOV(90.0f),
	_cameraPos(0.0f, -1.49f, -0.1f),
	_cameraFront(0.0f, -1.50f, 300.0f),
	_cameraUp(0.0f, 0.0f, 30.0f),
	_aspectRation(screenWidth / screenHeight)
{
	setPrespectiveProjection();
	updateCamera();
}


Camera::~Camera()
{
}

void Camera::setOrthogtaphicProjection()
{
	_projectionMatrix = glm::ortho(-_projactionWidth / 2, _projactionWidth / 2, -_projectionHeight / 2, _projectionHeight / 2, _near, _far);
}

void Camera::setPrespectiveProjection()
{
	_projectionMatrix = glm::perspective(_FOV, _aspectRation, _near, _far);
}

void Camera::updateCamera(glm::vec3 newPos, glm::vec3 newFront)
{
	_cameraPos = newPos;
	_cameraFront = newFront;
	glm::vec3 cameraDirection = glm::normalize(_cameraPos - _cameraFront);
	glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	_cameraUp = glm::cross(cameraDirection, cameraRight);
	_viewMatrix = glm::lookAt(_cameraPos, _cameraFront, _cameraUp);
}

void Camera::updateCamera()
{
	glm::vec3 cameraDirection = glm::normalize(_cameraPos - _cameraFront);
	glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	_cameraUp = glm::cross(cameraDirection, cameraRight);
	_viewMatrix = glm::lookAt(_cameraPos, _cameraFront, _cameraUp);

//	std::cout << _cameraPos.x << " // " << _cameraPos.y << " // " << _cameraPos.z << std::endl;
}

void Camera::moveCamera(glm::vec3 newPos, glm::vec3 newFront)
{
	_cameraPos += newPos;
	_cameraFront += newFront;
}
					 