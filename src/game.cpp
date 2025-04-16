#include "game.h"
#include <glad/glad.h>
#include "openglErrorReporting.h"
#include <glm/gtc/type_ptr.hpp>
#include "timer.h"
#include "world.h"
#include "blockdata.h"
#include "GameState/PlayingGameState.h"

Game::Game() 
	: camera(glm::vec3(0), 60.0f, (float)windowWidth / (float)windowHeight)
{
	running = true;
	currentState = nullptr;
}

bool Game::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "Failed to initialize SDL!" << std::endl;
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetSwapInterval(1);

	window = SDL_CreateWindow("I am almost Minecraft", 100, 100, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	context = SDL_GL_CreateContext(window);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
		return false;
	}

	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "OpenGL Version: " << version << std::endl;

	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	glViewport(0, 0, windowWidth, windowHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	SDL_SetRelativeMouseMode(SDL_TRUE);
	
	enableReportGlErrors();

	// Game State Setup
	PlayingGameState* playingState = new PlayingGameState(this, resourceManager);
	switchToState(playingState);

	return true;
}

void Game::gameLoop()
{
	Timer timer;
	while (running)
	{
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		glViewport(0, 0, w, h);
		camera.updateProjectionMatrix(w / (float)h);

		inputHandler.update();
		timer.tick();

		handleEvents();

		if (inputHandler.getKeyUp(SDLK_ESCAPE))
		{
			running = false;
		}

		// Game Updates
		currentState->update(timer.deltaTime, inputHandler);

		// Rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		currentState->render();

		SDL_GL_SwapWindow(window);
	}
}

Camera& Game::getCamera()
{
	return camera;
}

void Game::switchToState(BaseGameState* newState)
{
	if (currentState != nullptr)
	{
		currentState->onExit();
		delete currentState;
	}
	currentState = newState;
	currentState->onEnter();
}

void Game::handleEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYDOWN:
			inputHandler.keyUpdate(e);
			break;
		case SDL_KEYUP:
			inputHandler.keyUpdate(e);
			break;
		case SDL_MOUSEBUTTONDOWN:
			inputHandler.mouseUpdate(e);
			break;
		case SDL_MOUSEBUTTONUP:
			inputHandler.mouseUpdate(e);
			break;
		}
	}
}