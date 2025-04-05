#include "game.h"
#include <glad/glad.h>
#include "shader.h"
#include "openglErrorReporting.h"
#include "inputhandler.h"
#include "camera.h"
#include <glm/gtc/type_ptr.hpp>
#include "timer.h"
#include "texturesheet.h"
#include "chunk.h"
#include "blockdata.h"

Game::Game()
{

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

	SDL_SetRelativeMouseMode(SDL_TRUE);
	
	enableReportGlErrors();

	return true;
}

void Game::gameLoop()
{
	bool running = true;

	glClearColor(0.0f, 0.7f, 1.0f, 1.0f);

	Shader shader = Shader("shaders\\chunk.vert", "shaders\\chunk.frag");

	glUseProgram(shader.getProgramHandle());

	TextureSheet sheet = TextureSheet(16, 16, "textures\\terrain.png");

	BlockData blockData = BlockData();

	glBindTexture(GL_TEXTURE_2D, sheet.getSheet().getTextureHandle());

	unsigned int projectionLoc = glGetUniformLocation(shader.getProgramHandle(), "projection");
	unsigned int viewLoc = glGetUniformLocation(shader.getProgramHandle(), "view");
	unsigned int modelLoc = glGetUniformLocation(shader.getProgramHandle(), "model");

	unsigned int unitXLoc = glGetUniformLocation(shader.getProgramHandle(), "texUnitX");
	unsigned int unitYLoc = glGetUniformLocation(shader.getProgramHandle(), "texUnitY");
	unsigned int atlasSizeX = glGetUniformLocation(shader.getProgramHandle(), "atlasSizeX");
	unsigned int atlasSizeY = glGetUniformLocation(shader.getProgramHandle(), "atlasSizeY");

	Chunk* chunk = new Chunk();
	chunk->generateChunk();
	chunk->generateMesh(blockData);

	float oneX = sheet.getOneUnitX();
	float oneY = sheet.getOneUnitY();

	int atlasSize = 16;
	glUniform1fv(unitXLoc, 1, &oneX);
	glUniform1fv(unitYLoc, 1, &oneY);
	glUniform1iv(atlasSizeX, 1, &atlasSize);
	glUniform1iv(atlasSizeY, 1, &atlasSize);

	InputHandler inputHandler;
	Timer timer;
	Camera camera = Camera(glm::vec3(0.0f), 60.0f, (float)windowWidth / (float)windowHeight, 2.0f);

	glm::mat4 model = glm::mat4(1.0f);

	while (running)
	{
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		glViewport(0, 0, w, h);
		camera.updateProjectionMatrix(w / (float)h);

		inputHandler.update();
		timer.tick();

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
			}
		}
		if (inputHandler.getKeyUp(SDLK_ESCAPE))
		{
			running = false;
		}

		camera.update(timer.deltaTime, inputHandler);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		chunk->render();

		glBindVertexArray(0);

		SDL_GL_SwapWindow(window);
	}
}
