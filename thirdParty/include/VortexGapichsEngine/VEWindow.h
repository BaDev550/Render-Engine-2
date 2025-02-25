#ifndef VEWINDOW_CLASS
#define VEWINDOW_CLASS

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VECore.h"
#include "VELogger.h"
#include <iostream>
#include <string>

struct GameSettings;

enum VEWindowHints {
	VE_WINDOW_RESIZABLE,
	VE_WINDOW_SHOW_CURSOR,
	VE_WINDOW_CURSOR_MODE,
	VE_WINDOW_FULLSCREEN,
	VE_WINDOW_CLOSE,
	VE_WINDOW_DELETE,
	VE_WINDOW_VSYNC
};

class VEWindow
{
private:
	unsigned int w_Width = 800, w_Height = 800;
	const char* w_Title = "OpenGL Window";
	int wfb_Width, wfb_Height;


	GLFWimage* w_Icon = NULL;
	GLFWmonitor* monitor = NULL;
	GLFWvidmode* mode = NULL;

	bool setIcon(const char* Icon, GLFWimage* icon);
public:
	GLFWwindow* window = NULL;
	int renderMode;

	unsigned int iconTextureID;
	unsigned int iconWidth, iconHeight;

	VEWindow() {}
	~VEWindow();

	bool createWindow(int width, int height, GameSettings& settings);
	
	void clearScreen();
	void updateBuffer();

	void setIconTexture(const char* Icon);
	void setVEWindowHint(VEWindowHints hint, int value = 0);
	__forceinline bool windowClosed() { return glfwWindowShouldClose(window); }
	int getFrameBufferWidth() { return wfb_Width; }
	int getFrameBufferHeight() { return wfb_Height; }
};

#endif // !VEWINDOW_CLASS
