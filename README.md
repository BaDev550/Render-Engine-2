# TestOfRenderEngine

# HELLOOOOO
# this is a render engine that I made I hope you like it 
# let me explain how this engine works and how you can make random shits :D

## Usefull functions macros
	first we have the SEND_MSG() macro let me show how you can use it
	SEND_MSG(ERROR, "README_FILE", "This readme file is to hot");

		First argument is the message type as you can see there is 3 types
		such as ERROR, WARN, INFO
		this macro helps you debug-cacth errors easer its gonna log the error like this and its gonna save every log into a folder inside of the include file

# IMAGE

	inside of the VortexRenderEngine/engine/logs you can find all of your logs

	SEND_MSG_TERMINATE(ERROR, "READ_ME_FILE", "I hope we dont crash");
		This is for much importand reasons this one is similar to the SEND_MSG only difference is this one closes the app

## How the VELogger Works
	static std::string logLevelToString(LogLevel level); 
		this one changes the enum to the string so we can save it to files and show to the screen

	ImVec4 getColorForLogLevel(LogLevel level); 
		this one is for ImGui at the top you can see a debbuger and some messages are green and some of them are red, orange this becouse this function
		this function takes the message level ERROR, WARN, INFO and turn them into a color

	static std::string getCurrentTimestamp();
		only for getting the current time in year-month-day-hour-minute-secounds

	void setLogFile(const std::string& filename) {
		logFile = std::ofstream(filename, std::ios::app);
		if (!logFile.is_open()) {
			throw std::runtime_error("Unable to open log file");
		}
	}
		oooo looks scary right, dont be this is only for opening the log file the filename you enter for me this is the ENGINE_PATH + "engine/logs"
		note: you have to open a folder for the logs for example if you say the file name is "logs" but dont open a folder called "logs" in your project file its gonna throw a error
		dont be scared :D
		IMPORTAND: the log file selection shoud be allways at the top on start function that you are gonna see after

	static void log(LogLevel level, std::string sender, std::string message, const char* file, int line);
		this is for logging && saving the logs 
		this is the same as SEND_MSG macro bc macro uses this funtion and replaces the file, line with __FILE__ __LINE__ or else we cannot see the correct file and line its only gonna show
		VELoggers file location and using macro fixes

## This was all
	# you see the logger class was easyy now we are gonna look whats under the hood
	# jk first we need to see the main engine classes




