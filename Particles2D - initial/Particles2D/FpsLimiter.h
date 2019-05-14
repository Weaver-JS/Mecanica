#pragma once

	///Calculates FPS and also limits FPS
	class FpsLimiter { 
	public:
		FpsLimiter();

		// Sets the desired max FPS
		void setMaxFPS(int maxFPS);

		void begin();

		// end() will return the current FPS as a float
		float end();
	private:
		// Calculates the current FPS
		void calculateFPS();

		// Variables
		float _fps;
		int _maxFPS;
		unsigned int _frameTime;
		unsigned int _startTicks;
	};

