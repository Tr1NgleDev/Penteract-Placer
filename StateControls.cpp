#include "StateControls.h"

StateControls StateControls::instanceObj;
StateControls* StateControls::instance()
{
	return &instanceObj;
}

void StateControls::init(StateManager& s)
{
	titleText.setText("Controls:");
	titleText.setSize(3);
	titleText.setAlignX(ui::ALIGN_CENTER_X);
	titleText.setOffsetY(100);

	controlsText.setText(
		"Movement (BC): WASD\n"
		"Move Down (-A): SHIFT\n"
		"Move Up (+A): SPACE\n"
		"Strafe Kata (-D): Q\n"
		"Strafe Ana (+D): E\n"
		"Strafe -E: R\n"
		"Strafe +E: F\n"
		"Select Block: 0-9 or scroll wheel\n"
		"Break Block: left-click\n"
		"Place Block: right-click\n"
		"Look ab/ac: mouse\n"
		"Look bc/bd: middle-click + mouse\n"
		"Open Console: \"`\" key (right next to the \"1\" key)\n"
	);
	controlsText.setAlignX(ui::ALIGN_CENTER_X);
	controlsText.setOffsetY(150);

	page.clear();
	page.addElem(&titleText);
	page.addElem(&controlsText);

	s.setUiPage(&page);

	int wWidth, wHeight;
	glfwGetWindowSize(s.getWindow(), &wWidth, &wHeight);
	windowResize(s, wWidth, wHeight);
}

void StateControls::close(StateManager& s)
{
	s.setUiPage(nullptr);
}

void StateControls::pause(StateManager& s)
{
	s.setUiPage(nullptr);
}

void StateControls::resume(StateManager& s)
{
	s.setUiPage(&page);
}

void StateControls::render(StateManager& s)
{
}

void StateControls::keyInput(StateManager& s, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		s.popState();
	}
}

void StateControls::windowResize(StateManager& s, int width, int height)
{
	controlsText.setWrapWidth(width - 100);
}