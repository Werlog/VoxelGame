#include "game.h"
#include <glad/glad.h>
#include "openglErrorReporting.h"
#include <glm/gtc/type_ptr.hpp>
#include "timer.h"
#include "world.h"
#include "blockdata.h"
#include "GameState/PlayingGameState.h"
#include "GameState/MainMenuGameState.h"
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl2.h"
#include "imguiThemes.h"

Game::Game()
	: camera(glm::vec3(0), 65.0f, (float)windowWidth / (float)windowHeight)
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

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);

	window = SDL_CreateWindow("I am almost Minecraft", 100, 100, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	context = SDL_GL_CreateContext(window);

	SDL_GL_SetSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "Failed to initialize GLAD!" << std::endl;
		return false;
	}

	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "OpenGL Version: " << version << std::endl;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	glViewport(0, 0, windowWidth, windowHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//SDL_SetRelativeMouseMode(SDL_TRUE);
	
	enableReportGlErrors();

	// ImGUI
	ImGui::CreateContext();

	imguiThemes::embraceTheDarkness();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		//style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.f;
		style.Colors[ImGuiCol_DockingEmptyBg].w = 0.f;
	}

	ImGui_ImplSDL2_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init("#version 430");

	uiRenderer.init(resourceManager, windowWidth, windowHeight);
	debugRenderer.init(resourceManager, &camera);
	fpsCounter.init(resourceManager);

	glLineWidth(2.0f);

	// Game State Setup

	MainMenuGameState* mainMenu = new MainMenuGameState(this);
	switchToState(mainMenu);
	
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
		uiRenderer.updateProjectionMatrix(w, h);

		inputHandler.update();
		timer.tick();
		fpsCounter.update(timer.deltaTime);

		handleEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

		// Game Updates
		currentState->update(timer.deltaTime, inputHandler);

		// Rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		fpsCounter.render(uiRenderer);

		currentState->render();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}

		SDL_GL_SwapWindow(window);
	}
}

Camera& Game::getCamera()
{
	return camera;
}

UIRenderer& Game::getUIRenderer()
{
	return uiRenderer;
}

DebugRenderer& Game::getDebugRenderer()
{
	return debugRenderer;
}

ResourceManager& Game::getResourceManager()
{
	return resourceManager;
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

void Game::quitGame()
{
	running = false;
}

void Game::setMousePosition(int x, int y)
{
	SDL_WarpMouseInWindow(window, x, y);
}

void Game::handleEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
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
		case SDL_MOUSEWHEEL:
			inputHandler.scrollUpdate(e);
			break;
		case SDL_TEXTINPUT:
			inputHandler.textInputUpdate(e);
			break;
		}
	}
}