#ifndef VECORE_CLASS
#define VECORE_CLASS

#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "VEWindow.h"
#include "VELogger.h"

const std::string ENGINE_PATH = "thirdParty/include/VortexGapichsEngine/";

using json = nlohmann::json;

struct EngineSettings
{
	std::string name;
	std::string version;
	std::string author;
	std::string icon;

	bool debug_mode;
	bool show_fps;
	bool show_stats;
};

struct GameSettings
{
	std::string name;
	std::string version;
	std::string author;
	std::string icon;

	bool bAntialiasing = true;
	bool bVSync = true;
	bool bShadows = true;
	bool bBloom = false;
	int AntialiasingCount = 4;
	int renderMode = 0;
};

class VECore
{
private:
public:
	static EngineSettings settings;
	static GameSettings gameSettings;

	VECore() {};

	void init();
	void loadEngineSettings();
	void loadGameSettings();

	void saveGameSettings(GameSettings newSettings);

	static EngineSettings getSettings() { return settings; }
	static GameSettings getRenderSettings() { return gameSettings; }
};

#endif // !VECORE_CLASS
