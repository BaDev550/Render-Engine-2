#include "VERenderer.h"

Camera testCam;

void VERenderer::start(VEWindow& window)
{
	core_shader.loadShader("shaders/core_vertex.glsl", "shaders/core_fragment.glsl");
	core_shader.setUBOBlockBinding("Matrices", 0);
	core_shader.createUBO();
	post_process.start(window.getFrameBufferWidth(), window.getFrameBufferHeight());
	cube_map.createCubemap();
}

void VERenderer::render(VEWindow& window, RenderingSystem system, const std::vector<Component*>& components)
{
	post_process.clearScreen();

	post_process.bindFBO();
	core_shader.use();

	glm::mat4 cameraMatrix = testCam.GetViewMatrix();

	cube_map.render(cameraMatrix);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)window.getFrameBufferWidth() / (float)window.getFrameBufferHeight(), 0.1f, 100.0f);
	core_shader.setMat4UniformBuffer(0, projection);
	core_shader.setMat4UniformBuffer(1, cameraMatrix);
	core_shader.setInt("renderMode", window.renderMode);
	core_shader.setVec3("cameraPos", testCam.Position);

	system.Render(components, core_shader);

	post_process.clearFBO();
	core_shader.setInt("shadowMap", 1);
	post_process.render();
}

void VERenderer::processInput(float dt)
{
	if (input.isKeyPressed(GLFW_KEY_W))
		testCam.ProcessKeyboard(FORWARD, dt);
	if (input.isKeyPressed(GLFW_KEY_S))
		testCam.ProcessKeyboard(BACKWARD, dt);
	if (input.isKeyPressed(GLFW_KEY_D))
		testCam.ProcessKeyboard(RIGHT, dt);
	if (input.isKeyPressed(GLFW_KEY_A))
		testCam.ProcessKeyboard(LEFT, dt);

	double mouseX, mouseY;
	glfwGetCursorPos(glfwGetCurrentContext(), &mouseX, &mouseY);
	testCam.ProcessMouse(mouseX, mouseY);
}
