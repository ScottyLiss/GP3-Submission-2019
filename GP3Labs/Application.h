#pragma once
#include "Entity.h"
#define WINDOW_W Application::GetInstance()->GetWindowWidth()
#define WINDOW_H Application::GetInstance()->GetWindowHeight()

enum AppState
{
	INITILISING, RUNNING, QUITTING
};


class Camera; //forward declaratio

class Application
{
private:
	std::vector<Entity*> m_entities;
	Camera* m_mainCamera = nullptr;

	glm::vec3 lightDirection = glm::vec3(10, 10, 0);

	//private variables
	static Application* m_application;
	SDL_Window* m_window = nullptr;
	SDL_Surface* gui_Surface;
	SDL_Renderer* gRenderer = NULL;
	SDL_GLContext m_glContext = nullptr;
	int m_windowWidth = 1280;
	int m_windowHeight = 720;
	float mouseSensitivity = 0.16f;
	AppState m_appState = AppState::INITILISING;
	float m_worldDeltaTime = 0.f;

	const int JOYSTICK_DEAD_ZONE = 8000;
	float xDir, yDir, moveX,moveY;
	bool joyStickConnected = false;

	//private functions
	Application();
	void InitResources();
	void Init();
	void OpenGlInit();
	void Loop();
	void Quit();
	void Update(float deltaTime);
	void Render();
	void GameInit();
	void Movement();
	void ModelRenderUpdate(std::shared_ptr<ShaderProgram> shader);


public:
	//public functions
	~Application();
	static Application* GetInstance();
	void Run();
	inline int GetWindowHeight() { return m_windowHeight; }
	inline int GetWindowWidth() { return m_windowWidth; }

	inline Camera* GetCamera() { return m_mainCamera; }
	void SetCamera(Camera* camera);

	void renderQuad();

	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gAlbedoSpec;
	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;
	std::vector<float> lightRadius;
};
