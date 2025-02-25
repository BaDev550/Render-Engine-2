#include "VEInput.h"

bool VEInput::isKeyPressed(int keycode, bool shift, bool control, bool whenPaused)
{
    if (glfwGetInputMode(glfwGetCurrentContext(), GLFW_CURSOR) != GLFW_CURSOR_DISABLED && !whenPaused) return false;

	if (shift)
	{
		return (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && glfwGetKey(glfwGetCurrentContext(), keycode) == GLFW_PRESS);
	}
    if (control) {
		return (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(glfwGetCurrentContext(), keycode) == GLFW_PRESS);
    }
    return (glfwGetKey(glfwGetCurrentContext(), keycode) == GLFW_PRESS);
}


bool VEInput::isKeyReleased(int keycode)
{
    return (glfwGetKey(glfwGetCurrentContext(), keycode) == GLFW_RELEASE);
}

bool VEInput::isMouseButtonPressed(int button)
{
    return (glfwGetMouseButton(glfwGetCurrentContext(), button) == GLFW_PRESS);
}

int prevMouseState = GLFW_RELEASE;
bool VEInput::isMouseButtonClicked(int button)
{
    int currentMouseState = glfwGetMouseButton(glfwGetCurrentContext(), button);
    bool bClicked = (prevMouseState == GLFW_RELEASE && currentMouseState == GLFW_PRESS);
    prevMouseState = currentMouseState;

    return bClicked;
}

bool VEInput::isMouseButtonReleased(int button)
{
    return (glfwGetMouseButton(glfwGetCurrentContext(), button) == GLFW_RELEASE);
}

void VEInput::update()
{
}
