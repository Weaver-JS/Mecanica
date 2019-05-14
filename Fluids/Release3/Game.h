#pragma once	   

//Third-party libraries
#include <GL/glew.h>			//The OpenGL Extension Wrangler
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>			//OpenGL Mathematics 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <time.h>
#include "Window.h"
#include "GLSLProgram.h"
#include "FPSLimiter.h"
#include "OpenGLBuffers.h"
#include "Vertex.h"
#include "Geometry.h"
#include "InputManager.h"
#include "Camera.h"
#include "Cloth.h"

#define	GAME_SPEED 0.1f
#define	PLAYER_SPEED 0.05f


//Game has four possible states: INIT (Preparing environment), PLAY (Playing), EXIT (Exit from the game) or MENU (Game menu)
enum class GameState{INIT, PLAY, EXIT, MENU};
enum class CameraAvtive { FIRTS_PERSON, THIRT_PERSON, ORTHOGRAFIC};

struct Player {
	glm::vec3 position;
	float height = 0;
	int score;
	int lives;
	float radius;
	float distCenter=5.0f;
	float rotation = 1.0f;
	float angle = 0.0f;
	void rotate(float ag) {
		angle += ag;
		update();
	}
	void move(float m) {
		distCenter += m;
		update();
	}
	void up(float u) {
		height += u;
		update();
	}
	void update() {
		this->position.x = cos(angle)*distCenter;
		this->position.y = sin(angle)*distCenter;
		this->position.z = height;
	}
};

//This class manages the game execution
class Game {
	public:						
		Game(std::string windowTitle, int screenWidth, int screenHeight, bool enableLimiterFPS, int maxFPS, bool printFPS);	//Constructor
		~Game() {};					//Destructor
		void run();					//Game execution
													   
	private:
		float _physicsTime;
		float _refresh_Time;
		Particle _particleVector[NUM_ROPES][NUM_PARTICLES];

			//Attributes	
		std::string _windowTitle;		//Window Title
		int _screenWidth;				//Screen width in pixels				
		int _screenHeight;				//Screen height in pixels				
		GameState _gameState;			//It describes the game state				
		Window _window;					//Manage the OpenGL context
		GLSLProgram _colorProgram;		//Manage the shader programs
		FPSLimiter _fpsLimiter;			//Manage the synchronization between frame rate and refresh rate
		OpenGLBuffers _openGLBuffers;	//Manage the openGL buffers
		Geometry _gameElements;			//Manage the game elements
		InputManager _inputManager;		//Manage the input devices
		GLuint _modelMatrixUniform;
		GLuint _viewMatrixUniform;
		GLuint _projectionMatrixUniform;

		Sphere _sphere;
		Box _box;

		Camera _camera;
		CameraAvtive _cameraActive;
		Player _player;
		bool _filled;
		float _speedIncrement;	
		int pSystemSolver = VERLET;
		bool fixParticles = false;
		bool collissions = false;

			//Internal methods
		void initSystems();
		void resetParticles();
		void initShaders();		
		void initPlayer();
		void gameLoop();
		void calculateSpringsForce();
		void processInput();
		void doPhysics();
		void executePlayerCommands();
		void renderGame();	
		void calculateHeightFieldsForce();
};

