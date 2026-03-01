#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "State.h"
#include <vector>

class StateManager
{
private:
	std::vector<State*> states{};

public:
	GLFWwindow* window = nullptr;

	bool isRunning();
	State* getCurrentState();
	State* getPrevState();

	void update(double dt);
	void render();

	void updatePrevState(double dt);
	void renderPrevState();

	void quit();

	void mouseInput(double xpos, double ypos);
	void scrollInput(double xoffset, double yoffset);
	void mouseButtonInput(int button, int action, int mods);
	void keyInput(int key, int scancode, int action, int mods);
	void windowResize(int width, int height);
	void charInput(unsigned int codepoint);
	void fileDrop(int path_count, const char* paths[]);

	void pushState(State* state);
	void popState();
	void changeState(State* newState);

	bool shouldClose() const;
};
