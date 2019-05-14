#include "Game.h"


/**
* Constructor
* Note: It uses an initialization list to set the parameters
* @param windowTitle is the window title
* @param screenWidth is the window width
* @param screenHeight is the window height
*/
Game::Game(std::string windowTitle, int screenWidth, int screenHeight, bool enableLimiterFPS, int maxFPS, bool printFPS) :
	_windowTitle(windowTitle), 
	_screenWidth(screenWidth), 
	_screenHeight(screenHeight),
	_gameState(GameState::INIT), 
	_fpsLimiter(enableLimiterFPS, maxFPS, printFPS),
	_camera(_screenWidth, _screenHeight){
	_filled = false;
	_cameraActive = CameraAvtive::THIRT_PERSON;
}

/**
* Destructor
*/
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
}

/*
* Initializes all the game engine components
*/
void Game::initSystems() {
		//Create an Opengl window using SDL
	_window.create(_windowTitle, _screenWidth, _screenHeight, 0);		
		//Compile and Link shader
	initShaders();
		//Set up the openGL buffers
	_openGLBuffers.initializeBuffers(_colorProgram);
		//Load the current scene
	_gameElements.loadScene("./resources/gameObjects.txt");
	//_speedIncrement = 1.0f;

	//Load Scenne
	_gameElements.loadBasic3DObjects();

	//Cloth
	for (int i = 0; i < _gameElements.getNumRopes(); i++) {
		Rope r;
		r.initParticles(glm::vec3(-0.2f, 0.0f + (i*2.0f), 1.0f), _gameElements.getNumParticles(), _gameElements.getNumParticles() - 1, 500.0f, 0.01f, 5.0f);
		_clothVector.push_back(r);
	} 
	
	_sphere = Sphere(glm::vec3(0), SPHERE_RADIUS);
	_box.setVertices(glm::vec3(-0.3f, 2.3f, 0.0f));

	_refresh_Time = 0.2f;
	_physicsTime = (float)SDL_GetTicks();
}

/*
* Initialize the shaders:
* Compiles, sets the variables between C++ and the Shader program and links the shader program
*/
void Game::initShaders() {
		//Compile the shaders
	_colorProgram.addShader(GL_VERTEX_SHADER, "./resources/shaders/vertex-shader.txt");
	_colorProgram.addShader(GL_FRAGMENT_SHADER, "./resources/shaders/fragment-shader.txt");
	_colorProgram.compileShaders();
		//Attributes must be added before linking the code
	_colorProgram.addAttribute("vertexPositionGame");
	_colorProgram.addAttribute("vertexColor");
		//Link the compiled shaders
	_colorProgram.linkShaders();
		//Bind the uniform variables. You must enable shaders before gettting the uniforme variable location
	_colorProgram.use();
	_modelMatrixUniform = _colorProgram.getUniformLocation("modelMatrix");
	_viewMatrixUniform = _colorProgram.getUniformLocation("viewMatrix");
	_projectionMatrixUniform = _colorProgram.getUniformLocation("projectionMatrix");
	_colorProgram.unuse();
}

void Game::initPlayer(){
	_player.position = glm::vec3(0.0f, 2.0f, 2.0f);
	_player.lives = 3;
	_player.score = 0;
}

void Game::gameLoop() {	
	_gameState = GameState::PLAY;
	while (_gameState != GameState::EXIT) {		
			//Start synchronization between refresh rate and frame rate
		_fpsLimiter.startSynchronization();
			//Process the input information (keyboard and mouse)
		processInput();
			//Execute the player actions (keyboard and mouse)
		executePlayerCommands();
			//Update the game status
			doPhysics();
			//Draw the objects on the screen
		renderGame();	
			//Force synchronization
		_fpsLimiter.forceSynchronization();
	}
}

void Game::processInput() {
	_inputManager.update();
	//Review https://wiki.libsdl.org/SDL_Event to see the different kind of events
	//Moreover, table show the property affected for each event type
	SDL_Event evnt;
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
* Executes the actions sent by the user by means of the keyboard and mouse
*/
void Game::executePlayerCommands() {
	//Mouse Inputs
	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)){
		//glm::ivec2 mouseCoords = _inputManager.getMouseCoords();
	}
	if (_inputManager.isKeyPressed(SDL_BUTTON_RIGHT)) {
		//glm::ivec2 mouseCoords = _inputManager.getMouseCoords();
	}

		//Keyboard Inputs
	//Move Player
	if (_inputManager.isKeyDown(SDLK_LEFT)) {
		_player.rotate(PLAYER_SPEED);
	}
	if(_inputManager.isKeyDown(SDLK_RIGHT)) {
		_player.rotate(-PLAYER_SPEED);
	}
	if (_inputManager.isKeyDown(SDLK_UP)) {
		_player.move(PLAYER_SPEED);
	}
	if (_inputManager.isKeyDown(SDLK_DOWN)) {
		_player.move(-PLAYER_SPEED);
	}

	//ChangeCamera
	if (_inputManager.isKeyDown(SDLK_1)) {
		_cameraActive = CameraAvtive::FIRTS_PERSON;
		_camera.setPrespectiveProjection();
	}
	else if (_inputManager.isKeyDown(SDLK_2)) {
		_cameraActive = CameraAvtive::THIRT_PERSON;
		_camera.setPrespectiveProjection();
	}
	else if (_inputManager.isKeyDown(SDLK_3)) {
		_cameraActive = CameraAvtive::ORTHOGRAFIC;
		_camera.setOrthogtaphicProjection();
		_camera.updateCamera();
	}


	if (_inputManager.isKeyPressed(SDLK_f)) {
		_filled = !_filled;
	}

	//Exit Game
	if (_inputManager.isKeyPressed(SDLK_ESCAPE)) {
		_gameState = GameState::EXIT;
	}

}

/*
* Update the game objects based on the physics
*/
void Game::doPhysics() {

	//Cameras
	switch (_cameraActive)
	{
	case CameraAvtive::FIRTS_PERSON:
		_camera.SetCameraPos(_player.position);
		_camera.SetCameraFront(glm::vec3(0.0f));
		_camera.SetProjectionHeight(0.5f);
		_camera.SetProjectionWith(0.5f);
		_camera.updateCamera();
		break;
	case CameraAvtive::THIRT_PERSON:
		_camera.SetCameraPos(glm::vec3(3.0f, -1.0f, 2.5f));
		_camera.SetCameraFront(glm::vec3(0.0f, 0.0f, 0.0f));
		_camera.SetProjectionHeight(0.0f);
		_camera.SetProjectionWith(0.0f);
		_camera.updateCamera();
		break;
	case CameraAvtive::ORTHOGRAFIC:
		_camera.SetCameraPos(glm::vec3(0.0f, 20.0f, 10.0f) + 0.0f);
		_camera.SetCameraFront(glm::vec3(0.0f, -1.0f, 7.0f));
		_camera.SetProjectionHeight(15.0f);
		_camera.SetProjectionWith(15.0f);
		_camera.updateCamera();
		break;
	default:
		break;
	}

	for (int i = 0; i < _gameElements.getNumRopes(); i++) {
		for (int j = 0; j < _clothVector[i].getDrawnParticles(); j++) {
			int index = j + _gameElements.getStartParticles() + (_clothVector[i].getDrawnParticles() * i);
			_clothVector[i].updateParticles(j, _gameElements.getGameElement(index), fixParticles, 0.01f, pSystemSolver);
			_clothVector[i].getParticle(j).collideWithSphere(_sphere);
			_clothVector[i].getParticle(j).collideWithBox(_box);
		}
		_clothVector[i].updateSprings();
	}

}

/**
* Draw the sprites on the screen
*/
void Game::renderGame() {
		//Temporal variable
	GameObject currentRenderedGameElement;

		//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Bind the GLSL program. Only one code GLSL can be used at the same time
	_colorProgram.use();
	glUniformMatrix4fv(_viewMatrixUniform, 1, GL_FALSE, glm::value_ptr(_camera.GetViewMatrix()));
	glUniformMatrix4fv(_projectionMatrixUniform, 1, GL_FALSE, glm::value_ptr(_camera.GetProjectionMatrix()));

	if(_filled) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//For each one of the elements: Each object MUST BE RENDERED based on its position, rotation and scale data
	for (int i = 0; i < _gameElements.getNumGameElements(); i++) {	

		//if (i != _gameElements.getNumParticles()+_gameElements.getStartParticles()-1)
		{
			currentRenderedGameElement = _gameElements.getGameElement(i);
			glm::mat4 modelMatrix;

			//TODO: Compute its model transformation matrix
			modelMatrix = glm::translate(modelMatrix, currentRenderedGameElement._translate);
			if (currentRenderedGameElement._angle != 0)
				modelMatrix = glm::rotate(modelMatrix, glm::radians(currentRenderedGameElement._angle), currentRenderedGameElement._rotation);
			modelMatrix = glm::scale(modelMatrix, currentRenderedGameElement._scale);

			//TODO: Pass the matrix as an uniform variable 
			glUniformMatrix4fv(_modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));

			//Send data to GPU
			_openGLBuffers.sendDataToGPU(&_gameElements.getData(currentRenderedGameElement._objectType), _gameElements.getNumVertices(currentRenderedGameElement._objectType));
		}
	} 

	//Unbind the program
	_colorProgram.unuse();

	//Swap the display buffers (displays what was just drawn)
	_window.swapBuffer();

}


