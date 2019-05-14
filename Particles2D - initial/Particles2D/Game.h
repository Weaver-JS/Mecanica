#pragma once

#include <GL/glew.h>			
#include <glm/glm.hpp>
#include <time.h>
#include "Window.h"
#include "FpsLimiter.h"
#include "InputManager.h"
#include "GLSLProgram.h"
#include "Particle.h"
#include "Geometry.h"

// standard C++ libraries
#include <iostream>
//#include <math.h>
#define PI 3.14159265359
#define NUM_VERTEX_CIRCLE 40
#define NUM_OBJECT 5
//Game has two possible states: PLAY or EXIT
enum class GameState{INIT, PLAY, EXIT};

const int NumGameObj = 5; //Triangle, SysParticles
const int PLANE_NUMS = 4;
const int PARTICLE_NUM = 20;

//This class manages the game execution
class Game {
	public:			
			//Constructor			
		Game(std::string windowTitle, int screenWidth, int screenHeight, int maxFPS);
		~Game();				//Destructor
		void run();					//Game execution

	private:
			//Attributes	
		std::string _windowTitle;
		int _screenWidth;			//Screen width in pixels				
		int _screenHeight;			//Screen height in pixels				
		int _maxFPS;				//The maximum FPS that we will generate
		GameState _gameState;		//It describes the game state		
		GLuint gVAO[NumGameObj];
		GLuint gVBO[NumGameObj];
		
			//Internal methods
		void initSystems();
		void loadShaders();
		void loadGameObjects(const int& NumGameObj);
//		void loadTextures();
		void gameLoop();
		void processInput();
		void executeActions();
		void drawGame();
		

		Window _window;
		FpsLimiter _fpsLimiter;
		InputManager _inputManager;
		GLSLProgram _glProgram;
		vector<glm::vec3> posSysPart; //Only position is needed for draw
		vector<Particle> sysParticles;
		Polygon _polygonArray[NUM_OBJECT];
		float _dt = 0.01f; //Simulation time step
		//Limits
		Plane _planes[4];
		//Geometry
//		Polygon _polTriang;
};

