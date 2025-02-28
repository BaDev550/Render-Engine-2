#include <VortexGapichsEngine/VortexGameEngine.h>

Entity player;
float playerSpeed = 5.0f;
RigidBodyComponent* rb;

void VEngine::start() {
	addLight("Spot Light", Spot);

	player = entityManager.createEntity("Player");

	addComponent<StaticMeshComponent>(player);
	addComponent<RigidBodyComponent>(player, glm::vec3(0.0f), 1.0f, false);
	addComponent<TransformComponent>(player, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	getComponent<StaticMeshComponent>(player)->staticModel.loadModel("resources/Models/mario_2/mario_2.obj");

	rb = getComponent<RigidBodyComponent>(player);
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

	if (ve_input.isKeyPressed(GLFW_KEY_DOWN))
		rb->velocity += glm::vec3(1.0f * playerSpeed, rb->velocity.y, rb->velocity.z) * deltaTime;
	if (ve_input.isKeyPressed(GLFW_KEY_UP))
		rb->velocity -= glm::vec3(1.0f * playerSpeed, rb->velocity.y, rb->velocity.z) * deltaTime;
	if (ve_input.isKeyPressed(GLFW_KEY_LEFT))
		rb->velocity += glm::vec3(rb->velocity.x, rb->velocity.y, 1.0f * playerSpeed) * deltaTime;
	if (ve_input.isKeyPressed(GLFW_KEY_RIGHT))
		rb->velocity -= glm::vec3(rb->velocity.x, rb->velocity.y, 1.0f * playerSpeed) * deltaTime;
}