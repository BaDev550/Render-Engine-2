#include "VEWindow.h"
#include <stb/stb_image.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

VEWindow::~VEWindow()
{
	glfwDestroyWindow(window);
	glfwTerminate();
	SEND_MSG(INFO, "WINDOW", "Window Destroyed");
}

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		return;
	std::cerr << "OpenGL Debug Message: " << message << std::endl;
}

bool VEWindow::setIcon(const char* Icon, GLFWimage* icon)
{
	int width, height, channels;
	unsigned char* image = stbi_load(Icon, &width, &height, &channels, 4);
	if (image) {
		GLFWimage icon;
		icon.width = width;
		icon.height = height;
		icon.pixels = image;

		glfwSetWindowIcon(window, 1, &icon);

		stbi_image_free(image);
		SEND_MSG(INFO, "ICON", "Icon loaded");
		return true;
	}
	else {
		SEND_MSG(ERROR, "ICON", "Icon loaded failed");
		return false;
	}
}

bool VEWindow::createWindow(int width, int height, GameSettings& settings)
{		
	w_Width = width;
	w_Height = height;
	w_Title = VECore::settings.name.c_str();

	if (!glfwInit()) {
		SEND_MSG(ERROR, "WINDOW", "GLFW Init failed");
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, settings.AntialiasingCount);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	SEND_MSG(INFO, "WINDOW", "GLFW Initialized");

	monitor = glfwGetPrimaryMonitor();
	mode = (GLFWvidmode*)glfwGetVideoMode(monitor);

	w_Width = mode->width;
	w_Height = mode->height;

	window = glfwCreateWindow(w_Width, w_Height, w_Title, NULL, NULL);
	if (window == NULL) {
		SEND_MSG(ERROR, "WINDOW", "Window creation failed");
		return false;
	}
	SEND_MSG(INFO, "WINDOW", "Window Created");
	glfwMakeContextCurrent(window);
	glfwSwapInterval(VECore::getRenderSettings().bVSync);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwGetFramebufferSize(window, &wfb_Width, &wfb_Height);
	setIcon(VECore::settings.icon.c_str(), w_Icon);

	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	}
	else
	{
		SEND_MSG(ERROR, "WINDOW", "GLAD Init failed");
		return false;
	}
	glViewport(0, 0, w_Width, w_Height);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	renderMode = settings.renderMode;
	setIconTexture(settings.icon.c_str());
	return true;
}

void VEWindow::clearScreen()
{
	glfwGetFramebufferSize(window, &wfb_Width, &wfb_Height);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void VEWindow::updateBuffer()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void VEWindow::setIconTexture(const char* Icon)
{
	glGenTextures(1, &iconTextureID);
	glBindTexture(GL_TEXTURE_2D, iconTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, channels;
	unsigned char* data = stbi_load(Icon, &width, &height, &channels, 0);
	if (data) {
		GLenum format{};
		GLenum dataFormat{};
		if (channels == 1) {
			format = dataFormat = GL_RED;
		}
		else if (channels == 3) {
			format = GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (channels == 4) {
			format = GL_RGBA;
			dataFormat = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		SEND_MSG_TERMINATE(ERROR, "GAME_ICON", "Cannot load game icon");
		return;
	}
	stbi_image_free(data);
}

void VEWindow::setVEWindowHint(VEWindowHints hint, int value)
{
	switch (hint)
	{
	case VE_WINDOW_RESIZABLE:
		glfwWindowHint(GLFW_RESIZABLE, value);
		break;
	case VE_WINDOW_SHOW_CURSOR:
		if (value == 0) {
			value = GLFW_CURSOR_DISABLED;
		}
		else {
			value = GLFW_CURSOR_NORMAL;
		}
		glfwSetInputMode(window, GLFW_CURSOR, value);
		break;
	case VE_WINDOW_CURSOR_MODE:
		glfwSetInputMode(window, GLFW_CURSOR, value);
		break;
	case VE_WINDOW_FULLSCREEN:
		glfwSetWindowMonitor(window, (value == 1) ? glfwGetPrimaryMonitor() : NULL, 0, 0, w_Width, w_Height, GLFW_DONT_CARE);
		if (value == 1) {
			w_Width, w_Height = mode->width, mode->height;
			glfwSetWindowSize(window, w_Width, w_Height);
		}
		break;
	case VE_WINDOW_CLOSE:
		glfwSetWindowShouldClose(window, value);
		break;
	case VE_WINDOW_DELETE:
		glfwTerminate();
		glfwDestroyWindow(window);
		SEND_MSG(INFO, "WINDOW", "Window Destroyed");
		break;
	case VE_WINDOW_VSYNC:
		glfwSwapInterval(value);
		break;
	default:
		break;
	}
}
