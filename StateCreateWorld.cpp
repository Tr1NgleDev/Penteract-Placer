#include "StateCreateWorld.h"
#include "StateManager.h"
#include "StateGame.h"

StateCreateWorld StateCreateWorld::instanceObj;
StateCreateWorld* StateCreateWorld::instance()
{
	return &instanceObj;
}

void StateCreateWorld::init(StateManager& s)
{
	qr = QuadRenderer{ Shader::get("quad") };
	qr.init();

	worldNameText.setText("World Name:");
	worldNameText.setAlignX(ui::ALIGN_CENTER_X);
	worldNameText.setOffsetY(200);
	worldNameText.setColor(glm::vec4{ 0, 1, 0, 1 });

	worldNameInput.setText("My World");
	worldNameInput.setSize(250, 50);
	worldNameInput.setAlignX(ui::ALIGN_CENTER_X);
	worldNameInput.setOffsetY(250);

	cancelButton.setText("Cancel");
	cancelButton.setAction([this, &s]() { s.popState(); });
	cancelButton.setSize(170, 50);
	cancelButton.setOffsetX(25);
	cancelButton.setAlignY(ui::ALIGN_BOTTOM);
	cancelButton.setOffsetY(-25);

	createButton.setText("Create New World!");
	createButton.setAction([this, &s]() { s.changeState(StateGame::instance()); });
	createButton.setSize(280, 50);
	createButton.setAlignX(ui::ALIGN_RIGHT);
	createButton.setOffsetX(-25);
	createButton.setAlignY(ui::ALIGN_BOTTOM);
	createButton.setOffsetY(-25);

	page.clear();
	page.addElem(&worldNameText);
	page.addElem(&worldNameInput);
	page.addElem(&cancelButton);
	page.addElem(&createButton);

	s.setUiPage(&page);
}

void StateCreateWorld::close(StateManager& s)
{
	s.setUiPage(nullptr);
}

void StateCreateWorld::update(StateManager& s, double dt)
{
}

void StateCreateWorld::render(StateManager& s)
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);

	int wW, wH;
	s.getSize(&wW, &wH);

	qr.setMode(QuadRenderer::MODE_FILL);
	qr.setColors({
		glm::vec4{ 0.3f, 0.14f, 0.34f, 1.0f },
		glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f },
		glm::vec4{ 0.0f, 0.0f, 0.0f, 1.0f },
		glm::vec4{ 0.3f, 0.14f, 0.34f, 1.0f }
		});
	qr.setPos(0, 0, wW, wH);
	qr.render();
}

void StateCreateWorld::mouseInput(StateManager& s, double xpos, double ypos)
{
}

void StateCreateWorld::scrollInput(StateManager& s, double xoff, double yoff)
{
}

void StateCreateWorld::mouseButtonInput(StateManager& s, int button, int action, int mods)
{
}

void StateCreateWorld::keyInput(StateManager&, int key, int scancode, int action, int mods)
{
}

void StateCreateWorld::windowResize(StateManager&, int width, int height)
{
}