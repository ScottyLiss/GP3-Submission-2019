#include "pch.h"
#include "Application.h"
#include "Common.h" 
#include "Log.h"
#include "MeshRenderer.h"
#include "Quad.h"
#include "CameraComp.h"
#include "Resources.h"
#include "Physics.h"
#include "BoxShape.h"
#include "SphereShape.h"
#include "ConeShape.h"
#include "CapsuleShape.h"
#include "CylinderShape.h"
#include "RigidBody.h"
#include "Input.h"



Application* Application::m_application = nullptr;
btVector3 btVec;
SDL_Joystick* gGameController = NULL;
SDL_Haptic* gControllerHaptic = NULL;

Application::Application()
{
}

void Application::Init()
{
	//performing initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		LOG_DEBUG(SDL_GetError(), Log::Error);
		exit(-1);
	}
	//setting openGL version to 4.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	//setting openGL context to core, not compatibility
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//creating window
	m_window = SDL_CreateWindow("GP3-GAME", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, m_windowWidth, m_windowHeight,
		SDL_WINDOW_OPENGL);
	SDL_CaptureMouse(SDL_TRUE);
	SDL_SetRelativeMouseMode(SDL_TRUE);


	OpenGlInit();
	GameInit();

}

void Application::OpenGlInit()
{
	//creating context (our opengl statemachine in which all our GL calls
	//will refer to)
	m_glContext = SDL_GL_CreateContext(m_window);
	// Using m_  to refer it as it being a member 

	CHECK_GL_ERROR();
	SDL_GL_SetSwapInterval(1);

	//initialsing glew (do this after establishing context!)
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "GLEW Error: " << glewGetErrorString(err) <<
			std::endl;
		exit(-1);
	}

	//enable depth testing
	GL_ATTEMPT(glEnable(GL_DEPTH_TEST));
	//set less or equal func for depth testing
	GL_ATTEMPT(glDepthFunc(GL_LEQUAL));
	//turn on back face culling
	GL_ATTEMPT(glEnable(GL_CULL_FACE));
	GL_ATTEMPT(glCullFace(GL_BACK));

	glViewport(0, 0, (GLsizei)m_windowWidth, (GLsizei)m_windowHeight);
}

void Application::InitResources()
{
	// Adding of Models to the resource manager 
	Resources::GetInstance()->AddModel("object/cube.obj"); // Used for terrain
	Resources::GetInstance()->AddModel("object/Pumpkin.obj");
	Resources::GetInstance()->AddModel("object/crate.obj");
	Resources::GetInstance()->AddModel("object/MK2.obj");
	Resources::GetInstance()->AddModel("object/hotdog.obj");
	Resources::GetInstance()->AddModel("object/Sword.obj");

	// Adding of the textures to the resouce manager
	Resources::GetInstance()->AddTexture("textures/Wood.jpg");
	Resources::GetInstance()->AddTexture("textures/Stone.jpg");
	Resources::GetInstance()->AddTexture("textures/GrassBlock.png");
	Resources::GetInstance()->AddTexture("textures/Pumpkin.jpg");
	Resources::GetInstance()->AddTexture("textures/crate.jpg");
	Resources::GetInstance()->AddTexture("textures/MK2.png");
	Resources::GetInstance()->AddTexture("textures/hotdog.png");
	Resources::GetInstance()->AddTexture("textures/Sword.png");

	// Adding of the shader(s) to the resource manager
	//Resources::GetInstance()->AddShader(std::make_shared<ShaderProgram>(ASSET_PATH + "shaders/BlinnPhong.vert",
	//	ASSET_PATH + "shaders/BlinnPhong.frag"), "Blinn"); // No longer in use
	//Resources::GetInstance()->AddShader(std::make_shared<ShaderProgram>(ASSET_PATH + "shaders/simple_VERT.vert",
	//	ASSET_PATH + "shaders/simple_FRAG.frag"), "Simple"); // No longer in use

	
	Resources::GetInstance()->AddShader(std::make_shared<ShaderProgram>(ASSET_PATH + "shaders/g_Buffer.vert",
		ASSET_PATH + "shaders/g_Buffer.frag"), "g_Buffer"); // G_Buffer
	Resources::GetInstance()->AddShader(std::make_shared<ShaderProgram>(ASSET_PATH + "shaders/D_Shading.vert",
		ASSET_PATH + "shaders/D_Shading.frag"), "D_Shading"); // Deferred Shading
}



void Application::GameInit()
{
	InitResources();

#pragma region Init Entities
	// Creates the floor
	Entity* a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(new MeshRenderer(
		Resources::GetInstance()->GetModel("object/cube.obj"),
		Resources::GetInstance()->GetShader("D_Shading"),
		Resources::GetInstance()->GetTexture("textures/GrassBlock.png"))
	);
	MeshRenderer* m = a->GetComponent<MeshRenderer>();
	a->GetTransform()->SetPosition(glm::vec3(0, -10.f, -20.f));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(100.f, 1.f,
		100.f)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	a->GetTransform()->SetScale(glm::vec3(100.f, 1.f, 100.f));
//-----------------------------------------------------------------------//

	//Creates the Camera 
	a = new Entity();
	m_entities.push_back(a);
	CameraComp* cc = new CameraComp();
	a->AddComponent(cc);
	m_entities.at(1)->GetTransform()->SetPosition(glm::vec3(0, 0, 0));


#pragma region Four Corner parts of the map
	//----------------------------------------------------------------------//
//					Four Corner stones to the map						//
//----------------------------------------------------------------------//
	a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("object/cube.obj"),
			Resources::GetInstance()->GetShader("D_Shading"),
			Resources::GetInstance()->GetTexture("textures/Stone.jpg"))
	);
	a->GetTransform()->SetPosition(glm::vec3(0, 0, -119.f));
	a->GetTransform()->SetScale(glm::vec3(100, 10, 1));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->
		Init(new BoxShape(glm::vec3(100, 10, 1)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	//-----------------------------------------------------//
	a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("object/cube.obj"),
			Resources::GetInstance()->GetShader("D_Shading"),
			Resources::GetInstance()->GetTexture("textures/Stone.jpg"))
	);
	a->GetTransform()->SetPosition(glm::vec3(0, 0, 79.f));
	a->GetTransform()->SetScale(glm::vec3(100, 10, 1));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->
		Init(new BoxShape(glm::vec3(100, 10, 1)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());

	//-----------------------------------------------------//
	a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("object/cube.obj"),
			Resources::GetInstance()->GetShader("D_Shading"),
			Resources::GetInstance()->GetTexture("textures/Stone.jpg"))
	);
	a->GetTransform()->SetPosition(glm::vec3(100, 0, -20));
	a->GetTransform()->SetScale(glm::vec3(100, 10, 1));
	a->GetTransform()->RotateEulerAxis(90, glm::vec3(0, 1, 0));
	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->
		Init(new BoxShape(glm::vec3(100, 10, 1)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
	//-----------------------------------------------------//
	a = new Entity();
	m_entities.push_back(a);
	a->AddComponent(
		new MeshRenderer(
			Resources::GetInstance()->GetModel("object/cube.obj"),
			Resources::GetInstance()->GetShader("D_Shading"),
			Resources::GetInstance()->GetTexture("textures/Stone.jpg"))
	);
	a->GetTransform()->SetPosition(glm::vec3(-100, 0, -20));
	a->GetTransform()->SetScale(glm::vec3(100, 10, 1));
	a->GetTransform()->RotateEulerAxis(-90, glm::vec3(0, 1, 0));

	a->AddComponent<RigidBody>();
	a->GetComponent<RigidBody>()->
		Init(new BoxShape(glm::vec3(100, 10, 1)));
	a->GetComponent<RigidBody>()->Get()->setMassProps(0, btVector3());
#pragma endregion

	//-----------------------------------------------------//
	// The 25 Pumpkin objects for physics 
	for (int i = 0; i < 25; i++)
	{
		Entity* a = new Entity();
		m_entities.push_back(a);
		a->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("object/Pumpkin.obj"),
				Resources::GetInstance()->GetShader("D_Shading"),
				Resources::GetInstance()->GetTexture("textures/Pumpkin.jpg"))
		);
		a->GetTransform()->SetPosition(glm::vec3(0, 5.f * i, -20.f));
		a->AddComponent<RigidBody>();
		a->GetComponent<RigidBody>()->Init(new SphereShape(3));
		a->GetTransform()->SetScale(glm::vec3(0.05f, 0.05f, 0.05f));
	}

	//-----------------------------------------------------//
	// The 25 Crates for Phyiscs
	for (int i = 0; i < 25; i++)
	{
		Entity* a = new Entity();
		m_entities.push_back(a);
		a->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("object/crate.obj"),
				Resources::GetInstance()->GetShader("D_Shading"),
				Resources::GetInstance()->GetTexture("textures/crate.jpg"))
		);
		a->GetTransform()->SetPosition(glm::vec3(0, 5.f * i, -20.f));
		a->AddComponent<RigidBody>();
		a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(1.f, 1.f, 1.f)));
		a->GetTransform()->SetScale(glm::vec3(0.05f, 0.05f, 0.05f));
	}

	//-----------------------------------------------------//
	// The 25 Grenades for Phyiscs
	for (int i = 0; i < 25; i++)
	{
		Entity* a = new Entity();
		m_entities.push_back(a);
		a->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("object/MK2.obj"),
				Resources::GetInstance()->GetShader("D_Shading"),
				Resources::GetInstance()->GetTexture("textures/MK2.png"))
		);
		a->GetTransform()->SetPosition(glm::vec3(0, 5.f * i, -20.f));
		a->AddComponent<RigidBody>();
		a->GetComponent<RigidBody>()->Init(new CapsuleShape(1.f, 1.f));
		a->GetTransform()->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	}

	//-----------------------------------------------------//
	// The 25 hotdogs for Phyiscs
	for (int i = 0; i < 25; i++)
	{
		Entity* a = new Entity();
		m_entities.push_back(a);
		a->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("object/hotdog.obj"),
				Resources::GetInstance()->GetShader("D_Shading"),
				Resources::GetInstance()->GetTexture("textures/hotdog.png"))
		);
		a->GetTransform()->SetPosition(glm::vec3(0, 5.f * i, -20.f));
		a->AddComponent<RigidBody>();
		a->GetComponent<RigidBody>()->Init(new CapsuleShape(0.5f, 0.5f));
		a->GetTransform()->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	}

	//-----------------------------------------------------//
	// The 25 Swords for Phyiscs
	for (int i = 0; i < 25; i++)
	{
		Entity* a = new Entity();
		m_entities.push_back(a);
		a->AddComponent(
			new MeshRenderer(
				Resources::GetInstance()->GetModel("object/Sword.obj"),
				Resources::GetInstance()->GetShader("D_Shading"),
				Resources::GetInstance()->GetTexture("textures/Sword.png"))
		);
		a->GetTransform()->SetPosition(glm::vec3(0, 5.f * i, -50.f));
		a->AddComponent<RigidBody>();
		a->GetComponent<RigidBody>()->Init(new BoxShape(glm::vec3(1.f, 0.15f,1.f)));
		a->GetTransform()->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
	}
#pragma endregion

#pragma region Deferred Rendering Buffer initialise 
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	// Position Color Buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Application::GetInstance()->GetWindowWidth(),
		Application::GetInstance()->GetWindowHeight(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// Normal Color Buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Application::GetInstance()->GetWindowWidth(),
		Application::GetInstance()->GetWindowHeight(), 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// - color + specular color buffer
	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Application::GetInstance()->GetWindowWidth(),
		Application::GetInstance()->GetWindowHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

	// - tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// create and attach depth buffer (renderbuffer)
	unsigned int rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Application::GetInstance()->GetWindowWidth(),
		Application::GetInstance()->GetWindowHeight());
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

#pragma region Init Lights in Scene
	const unsigned int NR_LIGHTS = 10;

for (unsigned int i = 0; i < NR_LIGHTS; i++)
{
	lightPositions.push_back(glm::vec3(1 * i, 1 * i , 10)); // Assigns positions of the lights

	lightColors.push_back(glm::vec3(i/2.f, 1, 1)); // Assigns color of the objects

	lightRadius.push_back(100); // Assigns the radius in which the light will affect objects
}

#pragma endregion

	// Assigns the uniform values
	Resources::GetInstance()->GetShader("D_Shading")->Use();
	Resources::GetInstance()->GetShader("D_Shading")->setInt("gPositon", 0);
	Resources::GetInstance()->GetShader("D_Shading")->setInt("gNormal", 1);
	Resources::GetInstance()->GetShader("D_Shading")->setInt("gAlbedoSpec", 2);

}

void Application::Movement() // Function that deals with the movement buttons 
{
	if (joyStickConnected)
	{
		m_entities.at(1)->GetTransform()->RotateEulerAxis(xDir,
			glm::vec3(0, 1, 0));

		m_entities.at(1)->GetTransform()->RotateEulerAxis(yDir,
			m_entities.at(1)->GetTransform()->GetRight());

		if (moveY == -1)
		{
			m_entities.at(1)->GetTransform()->AddPosition(m_entities.at(1)->GetTransform()->GetForward());
		}
		else if (moveY == 1)
		{
			m_entities.at(1)->GetTransform()->AddPosition(-m_entities.at(1)->GetTransform()->GetForward());
		}
		if (moveX == 1)
		{
			m_entities.at(1)->GetTransform()->AddPosition(m_entities.at(1)->GetTransform()->GetRight());
		}
		else if (moveX == -1)
		{
			m_entities.at(1)->GetTransform()->AddPosition(-m_entities.at(1)->GetTransform()->GetRight());
		}
	}

	if (INPUT->GetKey(SDLK_w)) // Move Forward
	{
		m_entities.at(1)->GetTransform()->AddPosition(m_entities.at(1)->GetTransform()->GetForward());
	} 
	else if (INPUT->GetKey(SDLK_s)) // Move Backward
	{
		m_entities.at(1)->GetTransform()->AddPosition(-m_entities.at(1)->GetTransform()->GetForward());
	}
	if (INPUT->GetKey(SDLK_a)) // Move Left 
	{
		m_entities.at(1)->GetTransform()->AddPosition(-m_entities.at(1)->GetTransform()->GetRight());
	} 
	else if (INPUT->GetKey(SDLK_d)) // Move Right
	{
		m_entities.at(1)->GetTransform()->AddPosition(m_entities.at(1)->GetTransform()->GetRight());
	}
	if (INPUT->GetKey(SDLK_SPACE)) // Move Upward
	{
		m_entities.at(1)->GetTransform()->AddPosition(glm::vec3(0,1,0));
	} 
	else if (INPUT->GetKey(SDLK_LSHIFT)) // Move downward
	{
		m_entities.at(1)->GetTransform()->AddPosition(glm::vec3(0,-1,0));
	}

}

void Application::Loop()
{
	m_appState = AppState::RUNNING;
	auto prevTicks = std::chrono::high_resolution_clock::now();
	SDL_Event event;

	//Check for joysticks
	if (SDL_NumJoysticks() < 1)
	{
		LOG_DEBUG("No controller active", Log::Warning);
	}
	else
	{
		joyStickConnected = true;
		//Load joystick
		gGameController = SDL_JoystickOpen(0);
		gControllerHaptic = SDL_HapticOpenFromJoystick(gGameController);
		LOG_DEBUG("Game controller is loaded", Log::Trace);
		if (gGameController == NULL)
		{
			joyStickConnected = false;
			LOG_DEBUG("Game controller is NULL", Log::Error);
		}
	}

	while (m_appState != AppState::QUITTING)
	{

	#pragma region SDL event & Key types
		//poll SDL events
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				m_appState = AppState::QUITTING;
				break;
			case SDL_WINDOWEVENT_CLOSE:
				m_appState = AppState::QUITTING;
				break;
			case SDL_MOUSEMOTION:
				INPUT->MoveMouse(glm::ivec2(event.motion.xrel, event.motion.yrel));
				glm::ivec2 movementPos = INPUT->GetMouseDelta();

				m_entities.at(1)->GetTransform()->RotateEulerAxis(movementPos.x * mouseSensitivity,
					glm::vec3(0, 1, 0));

				m_entities.at(1)->GetTransform()->RotateEulerAxis(movementPos.y * mouseSensitivity,
					m_entities.at(1)->GetTransform()->GetRight());
				break;
			case SDL_JOYBUTTONDOWN:
				LOG_DEBUG(std::to_string(event.jbutton.button), Log::Trace);
				// 0 = A 1 = B , 2 = X 3 = Y
				if (event.jbutton.button == 0)
				{
					SDL_HapticRumblePlay(gControllerHaptic, 0.75, 500);
					for (int i = 7; i < m_entities.size(); i++)
					{
						m_entities.at(i)->GetComponent<RigidBody>()->ApplyForce(btVector3(0, 30, 0));
					}
				}
				if (event.jbutton.button == 1)
				{
					SDL_HapticRumblePlay(gControllerHaptic, 0.75, 500);
					for (int i = 7; i < m_entities.size(); i++)
					{
						m_entities.at(i)->GetComponent<RigidBody>()->ApplyTorque(btVector3(0, 30, 0));
					}

				}
				break;
			case SDL_JOYAXISMOTION:
				// Right Stick, X/Y Axis
				if (event.jaxis.axis == 3)
				{
					if (event.jaxis.value < -JOYSTICK_DEAD_ZONE)
					{
						xDir = -1;
					}
					else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
					{
						xDir = 1;
					}
					else
					{
						xDir = 0;
					}
				}
				if (event.jaxis.axis == 4)
				{
					if (event.jaxis.value < -JOYSTICK_DEAD_ZONE)
					{
						yDir = -1;
					}

					else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
					{
						yDir = 1;
					}
					else
					{
						yDir = 0;
					}
				}

				// Left stick, X/Y axis
				if (event.jaxis.axis == 0)
				{
					if (event.jaxis.value < -JOYSTICK_DEAD_ZONE)
					{
						moveX = -1;
					}
					else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
					{
						moveX = 1;
					}
					else
					{
						moveX = 0;
					}
				}
				if (event.jaxis.axis == 1)
				{
					if (event.jaxis.value < -JOYSTICK_DEAD_ZONE)
					{
						moveY = -1;
					}

					else if (event.jaxis.value > JOYSTICK_DEAD_ZONE)
					{
						moveY = 1;
					}
					else
					{
						moveY = 0;
					}
				}
				break;
				
			case SDL_KEYDOWN:
				INPUT->SetKey(event.key.keysym.sym, true);


				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					
					m_appState = AppState::QUITTING;
					break;
				case SDLK_UP:
					for (int i = 7; i < m_entities.size(); i++)
					{
						m_entities.at(i)->GetComponent<RigidBody>()->ApplyForce(btVector3(0,30,0));
					}
					break;
				case SDLK_LEFT:
					for (int i = 7; i < m_entities.size(); i++)
					{
						m_entities.at(i)->GetComponent<RigidBody>()->ApplyTorque(btVector3(0,30,0));
					}
					break;
				}
			break;
			case SDL_KEYUP:
				INPUT->SetKey(event.key.keysym.sym, false);
			break;

			}
		}
		Movement(); // Call movement method, to handle the movement of the camera 
	#pragma endregion

		lightPositions.at(1) = m_entities.at(1)->GetTransform()->GetPosition();

		auto currentTicks = std::chrono::high_resolution_clock::now();
		float deltaTime =
			(float)std::chrono::duration_cast<std::chrono::microseconds>(currentTicks -
				prevTicks).count() / 100000;
		m_worldDeltaTime = deltaTime;
		prevTicks = currentTicks;

		Physics::GetInstance()->Update(deltaTime); // Updates Physics
		
		#pragma region Deferred Rendering
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Part 1 Working for Geometry Pass (i.e Color / Gemotry data into GBUFFER
	
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Resources::GetInstance()->GetShader("g_Buffer")->Use();
		Resources::GetInstance()->GetShader("g_Buffer")->setMat4("projection",
				Application::GetInstance()->GetCamera()->GetProj());
		Resources::GetInstance()->GetShader("g_Buffer")->setMat4("view",
				Application::GetInstance()->GetCamera()->GetView());
		ModelRenderUpdate(Resources::GetInstance()->GetShader("g_Buffer")); // Updates model to shader + binds it
	
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
				

		//part 2 Lighting pass 
		Resources::GetInstance()->GetShader("D_Shading")->Use(); 
		// Binds the active textures of position 0-1-2 to the positions of gPosition, gNormal and gAlbedoSpec
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);

			
		for (unsigned int i = 0; i < lightPositions.size(); i++)
		{
			Resources::GetInstance()->GetShader("D_Shading")->setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
			Resources::GetInstance()->GetShader("D_Shading")->setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
			const float constant = 1.f;
			const float linear = 0.057;
			const float quadratic = 0.058;
			Resources::GetInstance()->GetShader("D_Shading")->setFloat("lights[" + std::to_string(i) + "].Constant", constant);
			Resources::GetInstance()->GetShader("D_Shading")->setFloat("lights[" + std::to_string(i) + "].Linear", linear);
			Resources::GetInstance()->GetShader("D_Shading")->setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);
		}
	
		Resources::GetInstance()->GetShader("D_Shading")->setVec3("viewPos", Application::GetInstance()->GetCamera()->GetParentTransform()->GetPosition());
		renderQuad();


		//Update(deltaTime);
		Update(deltaTime);
		Render();
		SDL_GL_SwapWindow(m_window);
		Resources::GetInstance()->ReleaseUnusedResources(); // Release any resources that aren't being used in the game
	}
}



//-------------------------------------------------------------------------//
//			Model Render Update = Function to pass model to the shader     //
//			And then render the object within MeshRenderer.cpp			   //
//-------------------------------------------------------------------------//
void Application::ModelRenderUpdate(std::shared_ptr<ShaderProgram> shader)
{
	for (auto& a : m_entities)
	{
		a->UpdateModel(shader);
	}
}

void Application::Quit()
{
	//Close SDL
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
	SDL_Quit();
	Physics::GetInstance()->Quit();
}

Application::~Application()
{
}

Application* Application::GetInstance()
{
	if (m_application == nullptr)
	{
		m_application = new Application();
	}
	return m_application;
}

void Application::Run()
{
	Init();
	Loop();
	Quit();
}

void Application::Update(float deltaTime)
{
	for (auto& a : m_entities)
	{
		a->OnUpdate(deltaTime);
	}
}

void Application::SetCamera(Camera* camera)
{
	if (camera != nullptr)
	{
		m_mainCamera = camera;
		
	}
}

void Application::renderQuad() // Renders a quad over the screen, displaying the final result from the deferred shader
{
	if (quadVAO == 0)
	{
		
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}


void Application::Render() 
{
	m_mainCamera->Recalculate();


	/* // Current disabled for the fact that no object in scene use any other shader than Deferred shading
	for (auto& a : m_entities)
	{
		a->OnRender();  
	}
	*/
}
