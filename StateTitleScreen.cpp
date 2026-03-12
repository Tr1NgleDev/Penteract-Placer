#include "StateTitleScreen.h"

StateTitleScreen StateTitleScreen::instanceObj;
StateTitleScreen* StateTitleScreen::instance()
{
	return &instanceObj;
}

void StateTitleScreen::init(StateManager& s)
{
	qr = QuadRenderer{ Shader::get("quad") };
	qr.init();

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

	glDisable(GL_DEPTH_TEST);

	int wW, wH;
	s.getSize(&wW, &wH);

	qr.setMode(QuadRenderer::MODE_FILL);
	qr.setColors({
		glm::vec4{ 0.3f, 0.14f, 0.34f, 1.0f },
		glm::vec4{ 0.4f, 0.27f, 0.65f, 1.0f },
		glm::vec4{ 0.4f, 0.27f, 0.65f, 1.0f },
		glm::vec4{ 0.3f, 0.14f, 0.34f, 1.0f } });
	qr.setPos(0, 0, wW, wH);
	qr.render();

	qr.setMode(QuadRenderer::MODE_FILL);
	qr.setColor(0.0f, 0.0f, 0.0f, 0.7f);
	qr.setPos(100, 500, 150, 50);
	qr.render();

	qr.setMode(QuadRenderer::MODE_LINES);
	qr.setColor(1.0f, 1.0f, 1.0f, 1.0f);
	qr.render();
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