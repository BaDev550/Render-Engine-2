#include "VEngine.h"

bool bShowLogger = false;
bool bShowFPS = false;

void VEngine::engineUpdate()
{
	while (!ve_window.windowClosed() && engineState != VE_ENGINE_STOPED)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ve_window.clearScreen();

		std::vector<Component*> allComponents;
		for (auto& [entity, name] : entityManager.entities) {
			if (entityManager.hasComponent<TransformComponent>(entity)) {
				allComponents.push_back(entityManager.getComponent<TransformComponent>(entity));
			}
			if (entityManager.hasComponent<RigidBodyComponent>(entity)) {
				allComponents.push_back(entityManager.getComponent<RigidBodyComponent>(entity));
			}
			if (entityManager.hasComponent<StaticMeshComponent>(entity)) {
				allComponents.push_back(entityManager.getComponent<StaticMeshComponent>(entity));
			}
		}

		if (engineState == VE_ENGINE_UPDATE) {
			float currentTime = glfwGetTime();
			deltaTime = currentTime - lastFrame;
			lastFrame = currentTime;
			FPS = 1.0f / deltaTime;

			pyhsicsSystem.Update(allComponents, deltaTime);
			update(deltaTime);
		}

		renderEngineOptions();
		showEntityList();
		ve_renderer.render(ve_window, renderingSystem, allComponents);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ve_renderer.processInput(deltaTime);
		ve_window.updateBuffer();
	}
}

void VEngine::addLight(LightComponent::LightType type)
{
	LightComponent tmp_light(glm::vec3(0.0f), glm::vec3(1.0f), 1.0f, type);
	lightManager.addLight(tmp_light);
}

void VEngine::restart()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	core.saveGameSettings(ve_game_settings);
	ve_window.setVEWindowHint(VE_WINDOW_DELETE);
	engineState = VE_ENGINE_STOPED;
	engineInit();
}

void VEngine::refresh()
{
	ve_window.setIconTexture(ve_game_settings.icon.c_str());
}

void VEngine::showEntityList() {
	ImGui::Begin("Entity Manager");

	static char entityName[128] = "";

	ImGui::InputText("Entity Name", entityName, IM_ARRAYSIZE(entityName));
	if (ImGui::Button("Add Entity")) {
		if (strlen(entityName) > 0) {
			entityManager.createEntity(entityName);
			entityName[0] = '\0';
		}
	}
	if (ImGui::Button("Add Light")) {
		addLight(LightComponent::LightType::Point);
	}

	ImGui::Separator();

	for (auto& [entity, name] : entityManager.entities) {
		if (ImGui::TreeNode(name.c_str())) {
			entityManager.showEntityInImGui(entity);
			ImGui::TreePop();
		}
	}
	for (auto& light : lightManager.lights) {
		if (ImGui::TreeNode("Light ")) {
			lightManager.renderLightImGui(light);
			ImGui::TreePop();
		}
	}

	ImGui::End();
}

void VEngine::renderEngineOptions()
{
	if (ImGui::Begin("Options")) {
		ve_game_settings.name.resize(30, '\0');
		ve_game_settings.icon.resize(30, '\0');
		ve_game_settings.version.resize(30, '\0');
		ve_game_settings.author.resize(30, '\0');

		if (bShowFPS)
			ImGui::Text("FPS: %.1f", FPS);
		if (ImGui::TreeNode("Engine Options"))
		{
			// Game Settings

			ImGui::Text("Engine Name: %s", ve_settings.name.c_str());
			ImGui::Text("Engine Version: %s", ve_settings.version.c_str());
			if (ImGui::Checkbox("Show FPS", &ve_settings.show_fps)) {
				bShowFPS = ve_settings.show_fps;
			}

			ImGui::Checkbox("Debug Mode", &ve_settings.debug_mode);
			ImGui::Checkbox("Show Stats", &ve_settings.show_stats);
			if (ImGui::Button("Show Logger")) {
				bShowLogger = !bShowLogger;
			}
			ImGui::Combo("Render Mode", &ve_window.renderMode, "Default\0Light\0Texture\0Depth\0Wireframe\0\0");
			if (ve_window.renderMode == 4) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			ImGui::TreePop();
		}
		if (bShowLogger)
			ve_logger.renderImGui();
		if (ImGui::TreeNode("Game Settings")) {
			ImGui::InputText("Game Title", ve_game_settings.name.data(), ve_game_settings.name.size());
			ImGui::InputText("Game Icon", ve_game_settings.icon.data(), ve_game_settings.icon.size());
			ImGui::Image((void*)(intptr_t)ve_window.iconTextureID, ImVec2(64, 64));
			ImGui::InputText("Game Version", ve_game_settings.version.data(), ve_game_settings.version.size());
			ImGui::InputText("Game Author", ve_game_settings.author.data(), ve_game_settings.author.size());

			if (ImGui::Checkbox("Antialiasing", &ve_game_settings.bAntialiasing)) {
				ve_game_settings.AntialiasingCount = ve_game_settings.bAntialiasing ? 4 : 0;
			}

			if (ve_game_settings.bAntialiasing) {
				ImGui::InputInt("MSAA Antialiasing Level", &ve_game_settings.AntialiasingCount);
			}

			ImGui::Checkbox("VSync", &ve_game_settings.bVSync);
			ImGui::TreePop();
		}

		if (ImGui::Button("Save Settings", ImVec2(200, 25))) {
			// Window && Render Propeties
			ve_game_settings.renderMode = ve_window.renderMode;

			core.saveGameSettings(ve_game_settings);
			refresh();
		}
	}
	ImGui::End();
}

std::string VEngine::getCurrentTime()
{
	std::time_t now = std::time(nullptr);
	std::tm localTime;
	localtime_s(&localTime, &now);
	std::ostringstream oss;
	oss << (localTime.tm_year + 1900) << "-"
		<< (localTime.tm_mon + 1) << "-"
		<< localTime.tm_mday << "-"
		<< localTime.tm_hour << "-"
		<< localTime.tm_min << "-"
		<< localTime.tm_sec;
	return oss.str();
}

void VEngine::engineInit()
{
	std::string logFile = ENGINE_PATH + "engine/logs/log-" + getCurrentTime() + ".txt";
	ve_logger.setLogFile(logFile);

	core.init();

	ve_settings = core.getSettings();
	ve_game_settings = core.getRenderSettings();

	ve_window.createWindow(800, 800, ve_game_settings);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplGlfw_InitForOpenGL(ve_window.window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	engineState = VE_ENGINE_UPDATE;

	start();
	ve_renderer.start(ve_window);
	engineUpdate();
}
