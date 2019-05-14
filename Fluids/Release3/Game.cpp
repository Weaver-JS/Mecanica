#include "Game.h"

Game::Game(std::string windowTitle, int screenWidth, int screenHeight, bool enableLimiterFPS, int maxFPS, bool printFPS) :
	_windowTitle(windowTitle), 
	_screenWidth(screenWidth), 
	_screenHeight(screenHeight),
	_gameState(GameState::INIT), 
	_fpsLimiter(enableLimiterFPS, maxFPS, printFPS),
	_camera(_screenWidth, _screenHeight){
	_filled = false;
	_cameraActive = CameraAvtive::FIRTS_PERSON;
}

void Game::run() {
	initSystems();
	gameLoop();
}

void Game::initSystems() {
	_window.create(_windowTitle, _screenWidth, _screenHeight, 0);		
	initShaders();
	_openGLBuffers.initializeBuffers(_colorProgram);

	_gameElements.loadScene("./resources/gameObjects.txt");
	_gameElements.loadBasic3DObjects();

	//Cloth
	glm::vec3 _pos;
	for (int numRopes = 0; numRopes <NUM_ROPES; numRopes++)
	{	
		_pos = glm::vec3(-0.9f, -0.5f + numRopes*(DISTANCE / NUM_ROPES),1.0f);
		for (int i = 0; i < NUM_PARTICLES;i++)
		{
			Particle p;
			(i == 0) ? p.setFixed(true) : p.setFixed(false);
			if (i == NUM_PARTICLES/2 && numRopes == NUM_ROPES/2)
			{
				p.setPosition(_pos.x + i*(DISTANCE / NUM_PARTICLES), _pos.y , _pos.z*2);
			}
			else
			{
				p.setPosition(_pos.x + i*(DISTANCE / NUM_PARTICLES), _pos.y, _pos.z);
			}
			
			p.setBouncing(0.1f);
			//if (i != 0) p.setForce(0.0f, 0.0f, -9.8f);
			_particleVector[numRopes][i] = p;
		}
	}
	
	_sphere = Sphere(glm::vec3(0), SPHERE_RADIUS);
	_box.setVertices(glm::vec3(_gameElements.spherePosition));

	_refresh_Time = 0.2f;
	_physicsTime = (float)SDL_GetTicks();
	_player.update();
}

void Game::resetParticles()
{
	glm::vec3 _pos;
	for (int numRopes = 0; numRopes <NUM_ROPES; numRopes++)
	{
		_pos = glm::vec3(-0.9f, -0.5f + numRopes*(DISTANCE / NUM_ROPES), 0.0f);
		for (int i = 0; i < NUM_PARTICLES; i++)
		{
			Particle p = _particleVector[numRopes][i];
			if(fixParticles)(i == 0) ? p.setFixed(true) : p.setFixed(false);
			else p.setFixed(false);
			p.setPosition(_pos.x + i*(DISTANCE / NUM_PARTICLES), _pos.y, _pos.z);
			p.setBouncing(0.1f);
			p.setForce(glm::vec3(0.0f, 0.0f, 0.0f));
			if (i != 0) p.setForce(0.0f, 0.0f, -9.8f);
			_particleVector[numRopes][i] = p;
		}
	}
}

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
		_fpsLimiter.startSynchronization();
		processInput();
		executePlayerCommands();
		doPhysics();
		renderGame();	
		_fpsLimiter.forceSynchronization();
	}
}

void Game::calculateSpringsForce()
{
	const float ke = 500.0f;
	const float l = 0.15f;
	const float kd = 0.01f;
	//Reset forces & Set gravity
	for (int r = 0; r < NUM_ROPES; r++){
		for (int c = 0; c < NUM_PARTICLES; c++){
			_particleVector[r][c].setForce(glm::vec3(0.0f, 0.0f, 0.0f));
			_particleVector[r][c].setForce(glm::vec3(0.0f, 0.0f, -9.8f));
		}
	}

	//Streach
	for (int r = 0; r < NUM_ROPES; r++){
		for (int c = 0; c < NUM_PARTICLES-1; c++){
			Particle* p = &_particleVector[r][c];
			Particle* pNext = &_particleVector[r][c + 1];
			glm::vec3 vPos = pNext->getCurrentPosition() - p->getCurrentPosition();
			glm::vec3 vVel = pNext->getVelocity() - p->getVelocity();
			float vPosLength = glm::length(vPos);
			glm::vec3 force = (ke*(vPosLength - l) + kd * glm::dot(vVel, glm::normalize(vPos)))*glm::normalize(vPos);
			p->addForce(force);
			pNext->addForce(-force);
		}
	}
	for (int r = 0; r < NUM_ROPES-1; r++){
		for (int c = 0; c < NUM_PARTICLES; c++){
			Particle* p = &_particleVector[r][c];
			Particle* pNext = &_particleVector[r+1][c];
			glm::vec3 vPos = pNext->getCurrentPosition() - p->getCurrentPosition();
			glm::vec3 vVel = pNext->getVelocity() - p->getVelocity();
			float vPosLength = glm::length(vPos);
			glm::vec3 force = (ke*(vPosLength - l) + kd * glm::dot(vVel, glm::normalize(vPos)))*glm::normalize(vPos);
			p->addForce(force);
			pNext->addForce(-force);
		}
	}

	//Shear
	for (int r = 0; r < NUM_ROPES-1; r++) {
		for (int c = 0; c < NUM_PARTICLES - 1; c++) {
			Particle* p = &_particleVector[r][c];
			Particle* pNext = &_particleVector[r+1][c+1];
			glm::vec3 vPos = pNext->getCurrentPosition() - p->getCurrentPosition();
			glm::vec3 vVel = pNext->getVelocity() - p->getVelocity();
			float vPosLength = glm::length(vPos);
			glm::vec3 force = (ke*(vPosLength - l) + kd * glm::dot(vVel, glm::normalize(vPos)))*glm::normalize(vPos);
			p->addForce(force);
			pNext->addForce(-force);


			/* EXCEPTIONS */
		/*	if (c == NUM_PARTICLES - 2)
			{
				p = &_particleVector[r][NUM_PARTICLES - 1];
				p->addForce(force);
			}*/

			
			
		}
	}
	for (int r = 1; r < NUM_ROPES; r++) {
		for (int c = 0; c < NUM_PARTICLES-1; c++) {
			Particle* p = &_particleVector[r][c];
			Particle* pNext = &_particleVector[r-1][c+1];
			glm::vec3 vPos = pNext->getCurrentPosition() - p->getCurrentPosition();
			glm::vec3 vVel = pNext->getVelocity() - p->getVelocity();
			float vPosLength = glm::length(vPos);
			glm::vec3 force = (ke*(vPosLength - l) + kd * glm::dot(vVel, glm::normalize(vPos)))*glm::normalize(vPos);
			p->addForce(force);
			pNext->addForce(-force);
			
			
		}
	} 

	//Bend
	for (int r = 0; r < NUM_ROPES; r++){
		for (int c = 0; c < NUM_PARTICLES - 2; c++){
			Particle* p = &_particleVector[r][c];
			Particle* pNext = &_particleVector[r][c + 2];
			glm::vec3 vPos = pNext->getCurrentPosition() - p->getCurrentPosition();
			glm::vec3 vVel = pNext->getVelocity() - p->getVelocity();
			float vPosLength = glm::length(vPos);
			glm::vec3 force = (ke*(vPosLength - l) + kd * glm::dot(vVel, glm::normalize(vPos)))*glm::normalize(vPos);
			p->addForce(force);
			pNext->addForce(-force);
		/*	if (c == NUM_PARTICLES - 1)
			{
				p = &_particleVector[r][NUM_PARTICLES - 2];
				p->addForce(force);
			}*/
		}
	}
	for (int r = 0; r < NUM_ROPES - 2; r++){
		for (int c = 0; c < NUM_PARTICLES; c++){
			Particle* p = &_particleVector[r][c];
			Particle* pNext = &_particleVector[r + 2][c];
			glm::vec3 vPos = pNext->getCurrentPosition() - p->getCurrentPosition();
			glm::vec3 vVel = pNext->getVelocity() - p->getVelocity();
			float vPosLength = glm::length(vPos);
			glm::vec3 force = (ke*(vPosLength - l) + kd * glm::dot(vVel, glm::normalize(vPos)))*glm::normalize(vPos);
			p->addForce(force);
			pNext->addForce(-force);

			
		}
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
		case SDL_QUIT: _gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION: _inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
			break;
		case SDL_KEYDOWN: _inputManager.pressKey(evnt.key.keysym.sym);
			break;
		case SDL_KEYUP: _inputManager.releaseKey(evnt.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN: _inputManager.pressKey(evnt.button.button);
			break;
		case SDL_MOUSEBUTTONUP: _inputManager.releaseKey(evnt.button.button);
			break;
		}
	}
}

void Game::executePlayerCommands() {
	//Mouse Inputs
	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT)){glm::ivec2 mouseCoords = _inputManager.getMouseCoords();}
	if (_inputManager.isKeyPressed(SDL_BUTTON_RIGHT)) {glm::ivec2 mouseCoords = _inputManager.getMouseCoords();}

	//Move Player
	if (_inputManager.isKeyDown(SDLK_LEFT))	_player.rotate(PLAYER_SPEED);
	if(_inputManager.isKeyDown(SDLK_RIGHT))	_player.rotate(-PLAYER_SPEED);
	if (_inputManager.isKeyDown(SDLK_UP)) _player.up(PLAYER_SPEED);
	if (_inputManager.isKeyDown(SDLK_DOWN)) _player.up(-PLAYER_SPEED);
	if (_inputManager.isKeyDown(SDLK_0)) _player.move(PLAYER_SPEED);
	if (_inputManager.isKeyDown(SDLK_9)) _player.move(-PLAYER_SPEED);

	if (_inputManager.isKeyDown(SDLK_e)) {
		_gameElements.spherePosition += glm::vec3(0.0f, 0.0f, 0.02f);
		_sphere.setCenter(_gameElements.spherePosition);
		_gameElements.getGameElement(0)._translate = _gameElements.spherePosition;
	}
	if (_inputManager.isKeyDown(SDLK_q)) {
		_gameElements.spherePosition += glm::vec3(0.0f, 0.0f, -0.02f);
		_sphere.setCenter(_gameElements.spherePosition);
		_gameElements.getGameElement(0)._translate = _gameElements.spherePosition;
	}
	if (_inputManager.isKeyDown(SDLK_d)) {
		_gameElements.spherePosition += glm::vec3(0.0f, 0.02f, 0.0f);
		_sphere.setCenter(_gameElements.spherePosition);
		_gameElements.getGameElement(0)._translate = _gameElements.spherePosition;
	}
	if (_inputManager.isKeyDown(SDLK_a)) {
		_gameElements.spherePosition += glm::vec3(0.0f, -0.02f, 0.0f);
		_sphere.setCenter(_gameElements.spherePosition);
		_gameElements.getGameElement(0)._translate = _gameElements.spherePosition;
	}
	if (_inputManager.isKeyDown(SDLK_s)) {
		_gameElements.spherePosition += glm::vec3(0.02f, 0.0f, 0.0f);
		_sphere.setCenter(_gameElements.spherePosition);
		_gameElements.getGameElement(0)._translate = _gameElements.spherePosition;
	}
	if (_inputManager.isKeyDown(SDLK_w)) {
		_gameElements.spherePosition += glm::vec3(-0.02f, 0.0f, 0.0f);
		_sphere.setCenter(_gameElements.spherePosition);
		_gameElements.getGameElement(0)._translate = _gameElements.spherePosition;
	}

	if (_inputManager.isKeyPressed(SDLK_c)) collissions = !collissions;
	if (_inputManager.isKeyPressed(SDLK_r)) resetParticles();
	if (_inputManager.isKeyPressed(SDLK_t)) fixParticles = !fixParticles;

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
	//Filled
	if (_inputManager.isKeyPressed(SDLK_f)) _filled = !_filled;
	//Exit Game
	if (_inputManager.isKeyPressed(SDLK_ESCAPE)) _gameState = GameState::EXIT;

}

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

	//calculateSpringsForce();
	calculateHeightFieldsForce();
	for (int numRopes = 0; numRopes < NUM_ROPES; numRopes++){	
		for (int i = 0; i < NUM_PARTICLES;i++){
			int index = i + _gameElements.getStartParticles() + ((_gameElements.getNumParticles()) * numRopes);
			//COLLISSION
			if(collissions)_particleVector[numRopes][i].collideWithSphere(_sphere);
			//_particleVector[numRopes][i].collideWithBox(_box);

			_particleVector[numRopes][i].updateParticle(0.01f, (pSystemSolver == EULERSEMI) ? Particle::UpdateMethod::EulerSemi : Particle::UpdateMethod::Verlet);
			_gameElements.getGameElement(index)._translate = _particleVector[numRopes][i].getCurrentPosition();	 
		}
	}
}

void Game::renderGame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_colorProgram.use();
	glUniformMatrix4fv(_viewMatrixUniform, 1, GL_FALSE, glm::value_ptr(_camera.GetViewMatrix()));
	glUniformMatrix4fv(_projectionMatrixUniform, 1, GL_FALSE, glm::value_ptr(_camera.GetProjectionMatrix()));

	if(_filled) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GameObject currentRenderedGameElement;
	for (int i = 0; i < _gameElements.getNumGameElements(); i++) {	
		currentRenderedGameElement = _gameElements.getGameElement(i);
		glm::mat4 modelMatrix;
		modelMatrix = glm::translate(modelMatrix, currentRenderedGameElement._translate);
		if (currentRenderedGameElement._angle != 0) modelMatrix = glm::rotate(modelMatrix, glm::radians(currentRenderedGameElement._angle), currentRenderedGameElement._rotation);
		modelMatrix = glm::scale(modelMatrix, currentRenderedGameElement._scale);
		glUniformMatrix4fv(_modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		if (i >= _gameElements.getStartParticles()) {
			GameObject newObject;
			newObject._objectType = QUAD;
			newObject._translate = glm::vec3(0.0f, 0.0f, 2.0f);
			newObject._angle = 90;
			newObject._rotation = glm::vec3(0.0f, 0.0f, 1.0f);
			newObject._scale = glm::vec3(0.2f, 0.2f, 0.2f);
			newObject._mode = 4;
		}

		_openGLBuffers.sendDataToGPU(&_gameElements.getData(currentRenderedGameElement._objectType), _gameElements.getNumVertices(currentRenderedGameElement._objectType));
	} 
	_colorProgram.unuse();
	_window.swapBuffer();

}

void Game::calculateHeightFieldsForce()
{

	

	for (int r = 0; r < NUM_ROPES;r++)
	{
		for (int i = 0; i < NUM_PARTICLES;i++)
		{

			if (_particleVector[r][i].getCurrentPosition().z >1.0f)
			{
				_particleVector[r][i].setForce(glm::vec3(0.0f, 0.0f, -9.8f));
			}
			else _particleVector[r][i].setForce(glm::vec3(0.0f, 0.0f, 0.0f));
			if (r > 0 && i > 0 && r < NUM_ROPES - 1 && i < NUM_PARTICLES - 1)
				_particleVector[r][i].addForce(((_particleVector[r - 1][i].getCurrentPosition() + _particleVector[r + 1][i].getCurrentPosition() + _particleVector[r][i - 1].getCurrentPosition() + _particleVector[r][i + 1].getCurrentPosition()) / 4.0f - _particleVector[r][i].getCurrentPosition()) * 5000.0f);
			if (_particleVector[r][i].getCurrentPosition().z >1.0f)
			{
				_particleVector[r][i].addForce(glm::vec3(0.0f, 0.0f, -9.8f));
			}
			//_particleVector[r][i].setPosition(((_particleVector[r - 1][i].getCurrentPosition() + _particleVector[r + 1][i].getCurrentPosition() + _particleVector[r][i - 1].getCurrentPosition() + _particleVector[r][i + 1].getCurrentPosition()) / 4.0f - _particleVector[r][i].getCurrentPosition()) * 0.99f);
		}
	}
}


