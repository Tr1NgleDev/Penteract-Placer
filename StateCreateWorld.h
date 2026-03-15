#pragma once
#include "State.h"
#include "ui.h"

class StateCreateWorld : public State
{
public:

	static StateCreateWorld* instance();

	void init(StateManager& s) override;
	void close(StateManager& s) override;
	void update(StateManager& s, double dt) override;
	void render(StateManager& s) override;
	void mouseInput(StateManager& s, double xpos, double ypos) override;
	void scrollInput(StateManager& s, double xoff, double yoff) override;
	void mouseButtonInput(StateManager& s, int button, int action, int mods) override;
	void keyInput(StateManager&, int key, int scancode, int action, int mods) override;
	void windowResize(StateManager&, int width, int height) override;

private:

	static StateCreateWorld instanceObj;
	~StateCreateWorld() {}

	ui::page page;

	ui::text worldNameText;
	ui::text_input worldNameInput;
	ui::text_input worldSeedInput;
	ui::button cancelButton;
	ui::button createButton;

	ui::page loadingPage;
	ui::text loadingText;

	// for loading bar
	QuadRenderer qr;
};