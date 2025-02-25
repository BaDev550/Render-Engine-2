#ifndef VEINPUT_CLASS
#define VEINPUT_CLASS

#include <glfw/glfw3.h>
#include <unordered_map>

class VEInput
{
private:

public:
	VEInput() {}

	bool isKeyPressed(int keycode, bool shift = false, bool control = false, bool whenPaused = false);
	bool isKeyReleased(int keycode);
	bool isMouseButtonPressed(int button);
	bool isMouseButtonClicked(int button);
	bool isMouseButtonReleased(int button);

	void update();
};

#endif // !VEINPUT_CLASS
