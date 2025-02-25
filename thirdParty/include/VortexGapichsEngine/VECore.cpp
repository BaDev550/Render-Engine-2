#include "VECore.h"

EngineSettings VECore::settings;
GameSettings VECore::gameSettings;

void VECore::init()
{
	loadEngineSettings();
	loadGameSettings();
}

void VECore::loadEngineSettings()
{
    std::ifstream file("EngineSettings.json");
    if (!file.is_open()) {
        SEND_MSG(ERROR, "ENGINE", "Failed to open EngineSettings.json");
        return;
    }

    std::streamsize size = file.tellg();
    assert(size <= 0);

    file.seekg(0, std::ios::beg);
    json setting_data;
    file >> setting_data;

    for (auto it = setting_data.begin(); it != setting_data.end(); ++it) {
		std::string setting_name = it.key();
        json setting_data = it.value();

		settings.name = setting_data["engine_name"].get<std::string>();
		settings.version = setting_data["engine_version"].get<std::string>();
		settings.author = setting_data["engine_author"].get<std::string>();
		settings.icon = ENGINE_PATH + setting_data["engine_icon"].get<std::string>();

		settings.debug_mode = setting_data["setting_debug_mode"].get<bool>();
		settings.show_fps = setting_data["setting_show_fps"].get<bool>();
		settings.show_stats = setting_data["setting_show_stats"].get<bool>();
    }
}

void VECore::loadGameSettings()
{
	std::ifstream file("GameSettings.json");
	if (!file.is_open()) {
		SEND_MSG(ERROR, "ENGINE", "Failed to open EngineSettings.json");
		return;
	}

	std::streamsize size = file.tellg();
	assert(size <= 0);

	file.seekg(0, std::ios::beg);
	json setting_data;
	file >> setting_data;

	for (auto it = setting_data.begin(); it != setting_data.end(); ++it) {
		std::string setting_name = it.key();
		json setting_data = it.value();

		gameSettings.name = setting_data["game_name"].get<std::string>();
		gameSettings.version = setting_data["game_version"].get<std::string>();
		gameSettings.author = setting_data["game_author"].get<std::string>();
		gameSettings.icon = setting_data["game_icon"].get<std::string>();

		gameSettings.bAntialiasing = setting_data["render_antialiasing"].get<bool>();
		gameSettings.AntialiasingCount = setting_data["render_antialiasing_count"].get<int>();
		gameSettings.renderMode = setting_data["render_mode"].get<int>();
		gameSettings.bShadows = setting_data["render_shadows"].get<bool>();
		gameSettings.bVSync = setting_data["render_vsync"].get<bool>();
		gameSettings.bBloom = setting_data["render_bloom"].get<bool>();
	}
}

std::string trimString(const std::string& str) {
	size_t end = str.find('\0');
	return (end == std::string::npos) ? str : str.substr(0, end);
}

void VECore::saveGameSettings(GameSettings newSettings)
{
	gameSettings = newSettings;
	std::ofstream file("GameSettings.json");
	if (!file.is_open()) {
		SEND_MSG(ERROR, "ENGINE", "Failed to open GameSettings.json");
		return;
	}

	json setting_data;
	setting_data["game_settings"]["game_name"] = trimString(gameSettings.name);
	setting_data["game_settings"]["game_version"] = trimString(gameSettings.version);
	setting_data["game_settings"]["game_author"] = trimString(gameSettings.author);
	setting_data["game_settings"]["game_icon"] = trimString(gameSettings.icon);

	setting_data["game_settings"]["render_antialiasing"] = gameSettings.bAntialiasing;
	setting_data["game_settings"]["render_antialiasing_count"] = gameSettings.AntialiasingCount;
	setting_data["game_settings"]["render_shadows"] = gameSettings.bShadows;
	setting_data["game_settings"]["render_bloom"] = gameSettings.bBloom;
	setting_data["game_settings"]["render_vsync"] = gameSettings.bVSync;
	setting_data["game_settings"]["render_mode"] = gameSettings.renderMode;

	file << setting_data.dump(4);

	file.close();
}
