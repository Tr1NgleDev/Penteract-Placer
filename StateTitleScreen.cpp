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
	text.setOffsetY(300);
	text.setColor(glm::vec4{ 0, 1, 0, 1 });

	Texture::load("assets/textures/logo.png", 0, false, "logo.png");
	logo.setTexture(Texture::get("logo.png"));
	logo.setAlignX(ui::ALIGN_CENTER_X);
	logo.setScale(2.0f, 2.0f);

	page.addElem(&text);
	page.addElem(&logo);

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