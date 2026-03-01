#include "StateGame.h"

StateGame StateGame::instanceObj{};

void StateGame::init(StateManager& s)
{

}
void StateGame::update(StateManager& s, double dt)
{
	if (s.shouldClose())
	{
		s.popState();
		return;
	}
}
void StateGame::render(StateManager& s)
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void StateGame::mouseInput(StateManager& s, double xpos, double ypos)
{

}
void StateGame::scrollInput(StateManager& s, double xoff, double yoff)
{

}
void StateGame::mouseButtonInput(StateManager& s, int button, int action, int mods)
{

}
void StateGame::keyInput(StateManager&, int key, int scancode, int action, int mods)
{

}
void StateGame::windowResize(StateManager&, int width, int height)
{
	glViewport(0, 0, width, height);
}
