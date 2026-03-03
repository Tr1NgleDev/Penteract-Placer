#include "StateManager.h"

bool StateManager::isRunning()
{
	return !states.empty();
}
State* StateManager::getCurrentState()
{
	if (states.empty()) return nullptr;
	return states.back();
}
State* StateManager::getPrevState()
{
	if (states.size() < 2) return nullptr;
	return states[states.size() - 2];
}

void StateManager::update(double dt)
{
	if (isRunning())
	{
		getCurrentState()->update(*this, dt);
	}
}
void StateManager::render()
{
	if (isRunning())
	{
		getCurrentState()->render(*this);
	}

	glfwSwapBuffers(window);
}

void StateManager::updatePrevState(double dt)
{
	State* prevState = getPrevState();
	if (prevState)
	{
		prevState->update(*this, dt);
	}
}
void StateManager::renderPrevState()
{
	State* prevState = getPrevState();
	if (prevState)
	{
		prevState->render(*this);
	}
}

void StateManager::quit()
{
	glfwSetWindowShouldClose(window, true);
}

bool StateManager::shouldClose() const
{
	return glfwWindowShouldClose(window);
}

void StateManager::mouseInput(double xpos, double ypos)
{
	if (isRunning())
	{
		getCurrentState()->mouseInput(*this, xpos, ypos);
	}
}
void StateManager::scrollInput(double xoffset, double yoffset)
{
	if (isRunning())
	{
		getCurrentState()->scrollInput(*this, xoffset, yoffset);
	}
}
void StateManager::mouseButtonInput(int button, int action, int mods)
{
	if (isRunning())
	{
		getCurrentState()->mouseButtonInput(*this, button, action, mods);
	}
}
void StateManager::keyInput(int key, int scancode, int action, int mods)
{
	if (isRunning())
	{
		getCurrentState()->keyInput(*this, key, scancode, action, mods);
	}
}
void StateManager::windowResize(int width, int height)
{
	width = glm::max(width, 1);
	height = glm::max(height, 1);

	for (auto& state : states)
	{
		state->windowResize(*this, width, height);
	}

	render();
}
void StateManager::charInput(unsigned int codepoint)
{
	if (isRunning())
	{
		getCurrentState()->charInput(*this, codepoint);
	}
}
void StateManager::fileDrop(int path_count, const char* paths[])
{
	if (isRunning())
	{
		getCurrentState()->fileDrop(*this, path_count, paths);
	}
}

void StateManager::pushState(State* state)
{
	if (!states.empty())
	{
		states.back()->pause(*this);
	}

	states.emplace_back(state);

	state->init(*this);
}

void StateManager::popState()
{
	if (!states.empty())
	{
		states.back()->close(*this);

		states.pop_back();

		if (!states.empty())
		{
			states.back()->resume(*this);
		}
	}
}

void StateManager::changeState(State* newState)
{
	popState();
	pushState(newState);
}

void StateManager::getSize(int* width, int* height) const
{
	glfwGetFramebufferSize(window, width, height);
}
