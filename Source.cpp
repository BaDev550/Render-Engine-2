#include <VortexGapichsEngine/VortexGameEngine.h>

Entity mario;

void VEngine::start() {
	addLight(LightComponent::LightType::Point);

	mario = entityManager.createEntity("Mario");

	addComponent<StaticMeshComponent>(mario);
	addComponent<TransformComponent>(mario, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	getComponent<StaticMeshComponent>(mario)->staticModel.texture.loadTexture("resources/Textures/wood.png");
	getComponent<StaticMeshComponent>(mario)->staticModel.loadModel("resources/Models/mario_2/mario_2.obj");
}

void VEngine::update(float dt) {

	processInput();
}

void VEngine::processInput() {
	if (ve_input.isKeyPressed(GLFW_KEY_ESCAPE))
		ve_window.setVEWindowHint(VE_WINDOW_CLOSE, true);
	if (ve_input.isKeyPressed(GLFW_KEY_M, false, false, true))
		ve_window.setVEWindowHint(VE_WINDOW_SHOW_CURSOR, true);
	if (ve_input.isKeyPressed(GLFW_KEY_N, false, false, true))
		ve_window.setVEWindowHint(VE_WINDOW_SHOW_CURSOR, false);
}