#pragma once

#include "StateManager.h"
#include "QuadRenderer.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureBuffer.h"
#include "GPUBuffer.h"

class StateGame : public State
{
public:
	static StateGame instanceObj;

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

	glm::vec2 lastMousePos{ 0 };

	void init(StateManager& s) override;
	void update(StateManager& s, double dt) override;
	void render(StateManager& s) override;
	void mouseInput(StateManager& s, double xpos, double ypos) override;
	void scrollInput(StateManager& s, double xoff, double yoff) override;
	void mouseButtonInput(StateManager& s, int button, int action, int mods) override;
	void keyInput(StateManager&, int key, int scancode, int action, int mods) override;
	void windowResize(StateManager&, int width, int height) override;
};
