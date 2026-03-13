#include "StateGame.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureBuffer.h"
#include "QuadRendererBasic.h"

StateGame StateGame::instanceObj;
StateGame* StateGame::instance()
{
	return &instanceObj;
}

void StateGame::init(StateManager& s)
{
	Shader::load("renderer", {
		{ GL_VERTEX_SHADER, "assets/shaders/renderer.vert" },
		{ GL_FRAGMENT_SHADER, "assets/shaders/renderer.frag" } });

	rendererShader = Shader::get("renderer");

	cameraBuf = GPUBuffer{ sizeof(Camera) };
	cameraBuf.use(0);

	cam.pos = { 0, 0, 0, 0, 0 };
	cam.up			= { 1, 0, 0, 0, 0 };
	cam.forward		= { 0, 1, 0, 0, 0 };
	cam.left		= { 0, 0, 1, 0, 0 };
	cam.over		= { 0, 0, 0, 1, 0 };
	cam.yonder		= { 0, 0, 0, 0, 1 };
	cam.vFov = glm::radians(90.0f);

	lastMousePos = glm::vec2{ 0 };
	glfwSetCursorPos(s.getWindow(), lastMousePos.x, lastMousePos.y);
	glfwSetInputMode(s.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void StateGame::update(StateManager& s, double dt)
{
	m5::vec5 moveDir{ 0 };

	if (keys.w) moveDir += cam.forward;
	if (keys.s) moveDir -= cam.forward;
	if (keys.a) moveDir += cam.left;
	if (keys.d) moveDir -= cam.left;
	if (keys.q) moveDir += cam.over;
	if (keys.e) moveDir -= cam.over;
	if (keys.r) moveDir += cam.yonder;
	if (keys.f) moveDir -= cam.yonder;
	//if (keys.space) moveDir += cam.up;
	//if (keys.shift) moveDir -= cam.up;

	moveDir = m5::normalize(moveDir);

	cam.pos += moveDir * 4.0f * dt;
}

void StateGame::render(StateManager& s)
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cameraBuf.uploadData<Camera>(&cam);

	rendererShader->use();
	QuadRendererBasic::render();
}

void StateGame::mouseInput(StateManager& s, double xpos, double ypos)
{
	float dX = -(xpos - lastMousePos.x) * 2;
	float dY = (ypos - lastMousePos.y) * 2;

	lastMousePos = glm::vec2{ xpos, ypos };

	float hAngleDelta = dX / 1000.0f * glm::pi<float>();
	float vAngleDelta = dY / 1000.0f * glm::pi<float>();

	// a vector pointing outwards from the front of the player
	m5::vec5 pd = m5::normalize(m5::cross(cam.left, m5::vec5::up(), cam.over, cam.yonder));

	if (!keys.mmb)
	{
		float v = glm::asin(glm::clamp(-cam.forward.a, -1.0f, 1.0f));
		vAngleDelta = glm::clamp(vAngleDelta + v, -glm::half_pi<float>() + 0.001f, glm::half_pi<float>() - 0.001f) - v;

		m5::rotor5 rotH{ m5::wedge(pd, cam.left), hAngleDelta }; // BC
		m5::rotor5 rotV{ m5::wedge({ 1, 0, 0, 0, 0 }, pd), vAngleDelta }; // AB

		orientation = rotH * rotV * orientation;
	}
	else
	{
		if (!keys.shift)
		{
			m5::rotor5 rotH{ m5::wedge(cam.over, cam.left), hAngleDelta }; // CD
			m5::rotor5 rotV{ m5::wedge(pd, cam.over), vAngleDelta }; // BD

			orientation = rotH * rotV * orientation;
		}
		else
		{
			m5::rotor5 rotH{ m5::wedge(cam.yonder, cam.left), hAngleDelta }; // CE
			m5::rotor5 rotV{ m5::wedge(pd, cam.yonder), vAngleDelta }; // BE

			orientation = rotH * rotV * orientation;
		}
	}

	cam.up		= m5::normalize(orientation.rotate(m5::vec5::up()));
	cam.forward	= m5::normalize(orientation.rotate(m5::vec5::forward()));
	cam.left	= m5::normalize(orientation.rotate(m5::vec5::left()));
	cam.over	= m5::normalize(orientation.rotate(m5::vec5::over()));
	cam.yonder	= m5::normalize(orientation.rotate(m5::vec5::yonder()));
}

void StateGame::scrollInput(StateManager& s, double xoff, double yoff)
{

}

void StateGame::mouseButtonInput(StateManager& s, int button, int action, int mods)
{
	if (action == GLFW_REPEAT) return;

	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT: keys.lmb = (action == GLFW_PRESS); break;
	case GLFW_MOUSE_BUTTON_MIDDLE: keys.mmb = (action == GLFW_PRESS); break;
	case GLFW_MOUSE_BUTTON_RIGHT: keys.rmb = (action == GLFW_PRESS); break;
	}
}

void StateGame::keyInput(StateManager& s, int key, int scancode, int action, int mods)
{
	if (action == GLFW_REPEAT) return;

	switch (key)
	{
	case GLFW_KEY_W: keys.w = (action == GLFW_PRESS); break;
	case GLFW_KEY_S: keys.s = (action == GLFW_PRESS); break;
	case GLFW_KEY_A: keys.a = (action == GLFW_PRESS); break;
	case GLFW_KEY_D: keys.d = (action == GLFW_PRESS); break;
	case GLFW_KEY_Q: keys.q = (action == GLFW_PRESS); break;
	case GLFW_KEY_E: keys.e = (action == GLFW_PRESS); break;
	case GLFW_KEY_R: keys.r = (action == GLFW_PRESS); break;
	case GLFW_KEY_F: keys.f = (action == GLFW_PRESS); break;
	case GLFW_KEY_SPACE: keys.space = (action == GLFW_PRESS); break;
	case GLFW_KEY_LEFT_SHIFT: keys.shift = (action == GLFW_PRESS); break;
	case GLFW_KEY_ESCAPE: s.popState(); break;
	}
}

void StateGame::windowResize(StateManager&, int width, int height)
{
	rendererShader->setUniform("screenSize", (float)width, (float)height, 1.0f / width, 1.0f / height);
}