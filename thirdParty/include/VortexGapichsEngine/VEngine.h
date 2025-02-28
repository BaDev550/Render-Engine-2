#ifndef VORTEX_ENGINE
#define VORTEX_ENGINE

#include "VECore.h"
#include "VEWindow.h"
#include "VEInput.h"
#include "VELogger.h"

#include "ComponentManager.h"
#include "EntityManager.h"
#include "Systems.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "VERenderer.h"

enum EngineState
{
	VE_ENGINE_STOPED,
	VE_ENGINE_UPDATE,
	VE_ENGINE_PAUSED
};

class VEngine
{
private:
	VECore core;
	VEWindow ve_window;
	VERenderer ve_renderer;
	VELogger ve_logger;
	VEInput ve_input;
	EngineState engineState = VE_ENGINE_STOPED;

	EngineSettings ve_settings;
	GameSettings ve_game_settings;

	PhysicsSystem pyhsicsSystem;
	RenderingSystem renderingSystem;
	EntityManager entityManager;

	void engineUpdate();

	float deltaTime;
	float lastFrame;
	float FPS;

	void renderEngineOptions();
	void showEntityList();
	std::string getCurrentTime();
public:
	VEngine() {
		renderingSystem.setLightManager(&entityManager);
	};

	void engineInit();

	void restart();
	void refresh();
	void update(float deltaTime);
	void start();
	void processInput();

	template<typename T, typename... Args>
	void addComponent(Entity entity, Args&&... args);

	template<typename T>
	T* getComponent(Entity entity);

	void addLight(std::string name, LightType type);
};
#endif // !VORTEX_ENGINE

template<typename T, typename... Args>
inline void VEngine::addComponent(Entity entity, Args&&... args)
{
	if (!entityManager.hasComponent<T>(entity)) {
		entityManager.addComponent<T>(entity, std::make_unique<T>(std::forward<Args>(args)...));
	}
}

template<typename T>
inline T* VEngine::getComponent(Entity entity)
{
	if (entityManager.hasComponent<T>(entity)) {
		return entityManager.getComponent<T>(entity);
	}
}
