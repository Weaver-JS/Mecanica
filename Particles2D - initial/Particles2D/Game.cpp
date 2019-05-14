#include "Game.h"

Game::Game(std::string windowTitle, int screenWidth, int screenHeight, int maxFPS) :
	_windowTitle(windowTitle), _screenWidth(screenWidth), _screenHeight(screenHeight),_gameState(GameState::INIT), _maxFPS(maxFPS) {

}

Game::~Game()
{
}

/*
* Game execution
*/
void Game::run() {
		//System initializations
	initSystems();
		//Start the game if all the elements are ready
	gameLoop();
	//system("pause"); //Don't close the command window
}

/*
* Initializes all the game engine components
*/
void Game::initSystems() {
		//Create an Opengl window using SDL
	_window.create(_windowTitle, _screenWidth, _screenHeight, 0);
		//Set the max fps
	_fpsLimiter.setMaxFPS(_maxFPS);
		//Compile and Link shader
	loadShaders();
		//Load all the Game Objects
	loadGameObjects(NumGameObj);
		//Load and prepare the textures
	//loadTextures();	
}

/*
* Compiles, sets the variables between C++ and the Shader program and links the shader program
*/
void Game::loadShaders() {
		//Compile the shaders
	_glProgram.addShader(GL_VERTEX_SHADER, "./vertex-shader.txt");
	_glProgram.addShader(GL_FRAGMENT_SHADER, "./fragment-shader.txt");
	_glProgram.compileShaders();
		//Attributes must be added before linking the code
	_glProgram.addAttribute("vert");
		//Link the compiled shaders
	_glProgram.linkShaders();
}

/*
* Loads all the element that has to be displayed on the screen
*/
void Game::loadGameObjects(const int& NumGameObj) {
	// make and bind the VAO
	glGenVertexArrays(NumGameObj, &gVAO[0]);
	
	//-------------------------------------
	// INIT and LOAD the Particle System
	//-------------------------------------
	glBindVertexArray(gVAO[0]);
	glGenBuffers(1, &gVBO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO[0]);

	// Initialize Particles
	sysParticles.resize(PARTICLE_NUM);
	posSysPart.resize(PARTICLE_NUM);
	srand(time(NULL));
	for (int i = 0; i < PARTICLE_NUM; i++) {
		sysParticles[i].setPosition(0.0f, 0.8f, 0.0f);
		float xForce = rand() % 2 + 0.1f;//((rand() % 50) / 10.0f) - 2.0f;
		float yForce = rand() % 2 + 0.1f;//((rand() % 100) / 10.0f) - 2.5f;
		float liveTime = ((rand() % 100) / 5000.0f);
		sysParticles[i].setVelocity(xForce, yForce, 0.0f);
		sysParticles[i].setLifetime(liveTime);
		sysParticles[i].setBouncing(0.9999f);
		sysParticles[i].setFixed(false);
		posSysPart[i] = sysParticles[i].getCurrentPosition();
	}

	glBufferData(GL_ARRAY_BUFFER, posSysPart.size()*sizeof(glm::vec3), &posSysPart[0], GL_STREAM_DRAW);
	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(_glProgram.getAttribLocation("vert"));
	glVertexAttribPointer(_glProgram.getAttribLocation("vert"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	// unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//----------------------
	// LOAD the Circle
	//----------------------
	glBindVertexArray(gVAO[1]);
	glGenBuffers(1, &gVBO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO[1]);

	float slice = (2.0f * PI) / NUM_VERTEX_CIRCLE;
	float angle2 = (2.0f * PI) / NUM_VERTEX_CIRCLE;
	for (int i = 0; i < NUM_VERTEX_CIRCLE;i++)
	{
		float radius = 0.2f;
		float angle = slice * i;
		_polygonArray[0].addVertex(glm::vec3(radius * cos(angle), radius * sin(angle), 0.0f));		
	}

	glBufferData(GL_ARRAY_BUFFER, _polygonArray[0].getVector().size()*sizeof(glm::vec3), &_polygonArray[0].getVector()[0], GL_STATIC_DRAW);
	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(_glProgram.getAttribLocation("vert"));
	glVertexAttribPointer(_glProgram.getAttribLocation("vert"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	// unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//----------------------
	// LOAD the Triangle
	//----------------------
	glBindVertexArray(gVAO[2]);
	glGenBuffers(1, &gVBO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO[2]);

	float mida = 0.2f;
	float altura = -sqrt(2.0f) * mida;
	_polygonArray[1].addVertex(glm::vec3(-0.1f, -0.4, 0.0f));
	_polygonArray[1].addVertex(glm::vec3(0.1f, -0.8f, 0.0f));
	_polygonArray[1].addVertex(glm::vec3(0.1f, -0.4, 0.0f));

	glBufferData(GL_ARRAY_BUFFER, _polygonArray[1].getVector().size()*sizeof(glm::vec3), &_polygonArray[1].getVector()[0], GL_STATIC_DRAW);

	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(_glProgram.getAttribLocation("vert"));
	glVertexAttribPointer(_glProgram.getAttribLocation("vert"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	// unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//----------------------
	// LOAD the Rectangle 1
	//----------------------
	glBindVertexArray(gVAO[3]);
	glGenBuffers(1, &gVBO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO[3]);

	_polygonArray[2].addVertex(glm::vec3(-0.8f, 0.4f, 0.0f));
	_polygonArray[2].addVertex(glm::vec3(-0.8f, -0.4f, 0.0f));
	_polygonArray[2].addVertex(glm::vec3(-0.6f, -0.4f, 0.0f));
	_polygonArray[2].addVertex(glm::vec3(-0.6f, 0.4f, 0.0f));

	glBufferData(GL_ARRAY_BUFFER, _polygonArray[2].getVector().size()*sizeof(glm::vec3), &_polygonArray[2].getVector()[0], GL_STATIC_DRAW);
	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(_glProgram.getAttribLocation("vert"));
	glVertexAttribPointer(_glProgram.getAttribLocation("vert"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	// unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//----------------------
	// LOAD the Rectangle 2
	//----------------------
	glBindVertexArray(gVAO[4]);
	glGenBuffers(1, &gVBO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO[4]);

	_polygonArray[3].addVertex(glm::vec3(0.8f, 0.4f, 0.0f));
	_polygonArray[3].addVertex(glm::vec3(0.8f, -0.4f, 0.0f));
	_polygonArray[3].addVertex(glm::vec3(0.6f, -0.4f, 0.0f));
	_polygonArray[3].addVertex(glm::vec3(0.6f, 0.4f, 0.0f));

	glBufferData(GL_ARRAY_BUFFER, _polygonArray[3].getVector().size()*sizeof(glm::vec3), &_polygonArray[3].getVector()[0], GL_STATIC_DRAW);
	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(_glProgram.getAttribLocation("vert"));
	glVertexAttribPointer(_glProgram.getAttribLocation("vert"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	// unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//----------------------
	// LOAD the Planes
	//----------------------
	_planes[0] = Plane();
	_planes[0].setPointNormal(glm::vec3(0.0f, -1.0f, 0.0f),glm::vec3(0.0f, -1.0f, 0.0f));
	_planes[1].setPointNormal(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	_planes[2].setPointNormal(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	_planes[3].setPointNormal(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f));

}


/*
* Game execution: Gets input events, processes game logic and draws objects on the screen
*/
void Game::gameLoop() {
	float _fps;
	int frameCounter = 0;
	_gameState = GameState::PLAY;
	float time = 0.0f;
	while (_gameState != GameState::EXIT) {
			//Start the fps limiter for the current frame
		_fpsLimiter.begin();
			//Process the input information (keyboard and mouse)
		processInput();
			//Execute actions based on the input events
		executeActions();
		time = time + _dt;
		//std::cout << "Total time = " << time << std::endl;
			//Draw the objects on the screen
		drawGame();			
			//Delay (or not) the execution for allowing the expected FPS
		_fps = _fpsLimiter.end();
			//Draw the current FPS in the console
		if (frameCounter == 10) {
			//cout << "FPS:" << _fps << endl;
			frameCounter = 0;
		}
		frameCounter++;
	}
}


/*
* Processes input with SDL
*/
void Game::processInput() {
	//Review https://wiki.libsdl.org/SDL_Event to see the different kind of events
	//Moreover, table show the property affected for each event type
	SDL_Event evnt;

		//Update the input event states. Current states are moved to the previous states for being able to detect pressed keys
	_inputManager.update();

	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN:
			_inputManager.pressKey(evnt.key.keysym.sym);
			if (_inputManager.isKeyPressed(SDLK_ESCAPE)) {
				_gameState = GameState::EXIT;
			}
			break;
		case SDL_KEYUP:
			_inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			_inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			_inputManager.releaseKey(evnt.button.button);
			break;
		default:
			break;
		}
	}
}

/**
* Moves the objects based on their logics and the input events
*/
void Game::executeActions() {

	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)){
		glm::ivec2 mouseCoords = _inputManager.getMouseCoords();
		cout << mouseCoords.x << ", " << mouseCoords.y << endl;
	}

	float disact, disant;
	Particle particleCopy;
	bool moveIsUpdate = false;
	
	for (int particleIndex = 0; particleIndex < PARTICLE_NUM; particleIndex++)
	{
		moveIsUpdate = false;
	
		if (sysParticles[particleIndex].getLifetime() > 0) 
		{
			Particle particleCp= sysParticles[particleIndex];

			sysParticles[particleIndex].setForce(0.0f, 0.0f, 0.0f);  //Avoid to accumulate
			//sysParticles[particleIndex].addForce(0.0f, -9.8f, 0.0f); //gravity
			sysParticles[particleIndex].updateParticle(_dt, Particle::UpdateMethod::EulerSemi);

			for (int i= 0;i < NUM_OBJECT;i++)
			{
				if (_polygonArray[i].isInside(sysParticles[particleIndex]))
				{
					_polygonArray[i].detectPlaneCollision(particleCp);
					//if(i == 1)std::cout << "Collision" << std::endl;
				}
			}
			
			sysParticles[particleIndex] = particleCp;
			
			for (int planeIndex = 0; planeIndex < PLANE_NUMS; planeIndex++) 
			{
				particleCopy = sysParticles[particleIndex];

				disant = _planes[planeIndex].distPoint2Plane(sysParticles[particleIndex].getCurrentPosition());
				sysParticles[particleIndex].setForce(0.0f, 0.0f, 0.0f);  //Avoid to accumulate
				//sysParticles[particleIndex].addForce(0.0f, -9.8f, 0.0f); //gravity
				sysParticles[particleIndex].updateParticle(_dt, Particle::UpdateMethod::EulerSemi);

				//Check for floor collisions
				sysParticles[particleIndex].setLifetime(sysParticles[particleIndex].getLifetime() - _dt); //lifetime is decreased
				disact = _planes[planeIndex].distPoint2Plane(sysParticles[particleIndex].getCurrentPosition());

				if (disant*disact < 0.0f) {
					//only valid for the plane y=0 (floor plane)
					glm::vec3 correcPos = -(1 + sysParticles[particleIndex].getBouncing()) * disact *_planes[planeIndex].normal;
					glm::vec3 correcVel = -(1 + sysParticles[particleIndex].getBouncing()) * (sysParticles[particleIndex].getVelocity()*_planes[planeIndex].normal)*_planes[planeIndex].normal;
					sysParticles[particleIndex].setPosition(sysParticles[particleIndex].getCurrentPosition() + correcPos);
					sysParticles[particleIndex].setVelocity(sysParticles[particleIndex].getVelocity() + correcVel);
					//sysParticles[particleIndex].updateParticle(_dt, Particle::UpdateMethod::EulerSemi);
					moveIsUpdate = true;
				}
				else {
					sysParticles[particleIndex] = particleCopy;
				}	
			}
			if (!moveIsUpdate) {
				disant = _planes[particleIndex].distPoint2Plane(sysParticles[particleIndex].getCurrentPosition());
				sysParticles[particleIndex].setForce(0.0f, 0.0f, 0.0f);  //Avoid to accumulate
				//sysParticles[particleIndex].addForce(0.0f, -9.8f, 0.0f); //gravity
				sysParticles[particleIndex].updateParticle(_dt, Particle::UpdateMethod::EulerSemi);
				moveIsUpdate = true;
			} 
		}
		else 
		{
			//Reiniciar Partícula
			sysParticles[particleIndex].setPosition(0.0f, 0.0f, 0.0f);
			float xForce = ((rand() % 100) / 20.0f) - 2.5f;
			float yForce = ((rand() % 100) / 10.0f)- 2.5f;
			float liveTime = ((rand() % 100) / 5000.0f);
			sysParticles[particleIndex].setVelocity(xForce, yForce, 0.0f);
			sysParticles[particleIndex].setLifetime(liveTime);
			sysParticles[particleIndex].setBouncing(0.99f);
			sysParticles[particleIndex].setFixed(false);
			std::cout << "llll" << std::endl;
		}

		glm::vec3 posicio = sysParticles[particleIndex].getCurrentPosition();
		posSysPart[particleIndex] = posicio;
	}

	// Pass to OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, gVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, posSysPart.size()*sizeof(glm::vec3), &posSysPart[0], GL_STREAM_DRAW);
	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(_glProgram.getAttribLocation("vert"));
	glVertexAttribPointer(_glProgram.getAttribLocation("vert"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	// unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

/**
* Draw objects on the screen
*/
void Game::drawGame() {
	//Set the base depth to 1.0
	glClearDepth(1.0);

	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Bind the GLSL program. Only one code GLSL can be used at the same time
	_glProgram.use();


		//PARTICLES
	glBindVertexArray(gVAO[0]);
	// draw the VAO
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPointSize(4);
	glDrawArrays(GL_POINTS, 0, PARTICLE_NUM);
	// unbind the VAO
	glBindVertexArray(0);


		//CIRCLE
	glBindVertexArray(gVAO[1]);
	// draw the VAO
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_LINE_LOOP, 0, NUM_VERTEX_CIRCLE);
	// unbind the VAO
	glBindVertexArray(0);


		//TRIANGLE
	glBindVertexArray(gVAO[2]);
	// draw the VAO
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_LINE_LOOP, 0, 3);
	// unbind the VAO
	glBindVertexArray(0);


		//RECTANGLE 1
	glBindVertexArray(gVAO[3]);
	// draw the VAO
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	// unbind the VAO
	glBindVertexArray(0);


		//RECTANGLE 2
	glBindVertexArray(gVAO[4]);
	// draw the VAO
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	// unbind the VAO
	glBindVertexArray(0);


		//Unbind the program
	_glProgram.unuse();

		//Swap the display buffers (displays what was just drawn)
	_window.swapBuffer();
}



