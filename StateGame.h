#pragma once
#include "StateManager.h"
#include "GPUBuffer.h"
#include "Math5D.h"
#include "World.h"
#include "TextureBuffer.h"

class Shader;

class StateGame : public State
{
public:

	bool shadows = true;
	bool ambientOcclusion = true;

	static StateGame* instance();

	void init(StateManager& s) override;
	void close(StateManager& s) override;
	void update(StateManager& s, double dt) override;
	void render(StateManager& s) override;
	void mouseInput(StateManager& s, double xpos, double ypos) override;
	void scrollInput(StateManager& s, double xoff, double yoff) override;
	void mouseButtonInput(StateManager& s, int button, int action, int mods) override;
	void keyInput(StateManager&, int key, int scancode, int action, int mods) override;
	void windowResize(StateManager&, int width, int height) override;

	// creates a world with a given edge length,
	// returning a pointer so the caller can initialize the world data.
	World* createWorld(uint8_t edgeLength);

private:

	static StateGame instanceObj;
	~StateGame() {}

	struct alignas(16) Camera
	{
		m5::vec5 pos; uint32_t _pad0[3]{ 0 };
		m5::vec5 up; uint32_t _pad2[3]{ 0 };
		m5::vec5 forward; uint32_t _pad3[3]{ 0 };
		m5::vec5 left; uint32_t _pad1[3]{ 0 };
		m5::vec5 over; uint32_t _pad4[3]{ 0 };
		m5::vec5 yonder; uint32_t _pad5[3]{ 0 };
		float vFov = glm::pi<float>() * 0.5f;
		uint32_t _pad6[3]{ 0 };
	};

	m5::rotor5 orientation;
	Camera cam;
	GPUBuffer cameraBuf;
	Shader* rendererShader;
	TextureBuffer chunksBuffer;
	std::vector<TextureBuffer> blockDataBuffers;
	std::vector<uint64_t> blockDataHandles;
	GPUBuffer blockDataHandlesBuffer;
	GPUBuffer blockTexturesBuffer;
	GPUBuffer tileTextureHandles;

	struct
	{
		bool w;
		bool s;
		bool a;
		bool d;
		bool q;
		bool e;
		bool r;
		bool f;
		bool space;
		bool shift;
		bool lmb;
		bool mmb;
		bool rmb;
	} keys;

	glm::dvec2 lastMousePos{ 0 };
	void enableCursor(GLFWwindow* window);
	void disableCursor(GLFWwindow* window);

	World world;
	m5::vec5 lightDir = m5::normalize({ 7.0f, 2.0f, 1.5f, 1.0f, 1.0f });
	
	ui::page pauseMenu;
	
	ui::text pausedText;
	ui::button backToGameButton;
	ui::button quitToTitleButton;
	ui::button toggleShadowsButton;
	ui::button toggleAOButton;

	ui::page ui;
	ui::text fpsText;
	ui::text coordsText;

	void updateRendererData();
};