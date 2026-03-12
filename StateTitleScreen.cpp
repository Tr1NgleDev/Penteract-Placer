#include "StateTitleScreen.h"

StateTitleScreen StateTitleScreen::instanceObj;
StateTitleScreen* StateTitleScreen::instance()
{
	return &instanceObj;
}

void StateTitleScreen::init(StateManager& s)
{
	text.setValue("hello world");
	text.setAlignX(ui::ALIGN_CENTER_X);
	text.setOffsetY(200);
	text.setColor(glm::vec4{ 0, 1, 0, 1 });

	page.addElem(&text);

	s.setUiPage(&page);
}

void StateTitleScreen::update(StateManager& s, double dt)
{
}

void StateTitleScreen::render(StateManager& s)
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void StateTitleScreen::mouseInput(StateManager& s, double xpos, double ypos)
{
}

void StateTitleScreen::scrollInput(StateManager& s, double xoff, double yoff)
{
}

void StateTitleScreen::mouseButtonInput(StateManager& s, int button, int action, int mods)
{
}

void StateTitleScreen::keyInput(StateManager&, int key, int scancode, int action, int mods)
{
}

void StateTitleScreen::windowResize(StateManager&, int width, int height)
{
}