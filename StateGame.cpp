#include "StateGame.h"
#include "StateTitleScreen.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureBuffer.h"
#include "QuadRendererBasic.h"
#include "Directory.h"
#include "utils.h"

StateGame StateGame::instanceObj;
StateGame* StateGame::instance()
{
	return &instanceObj;
}

void StateGame::init(StateManager& s)
{
	rendererShader = Shader::get("renderer");

	cameraBuf = GPUBuffer{ sizeof(Camera) };
	cameraBuf.use(0);

	cam.pos = { 18.0f, 3.5f, 3.5f, 3.5f, 3.5f };
	cam.up = m5::vec5::up();
	cam.forward = m5::vec5::forward();
	cam.left = m5::vec5::left();
	cam.over = m5::vec5::over();
	cam.yonder = m5::vec5::yonder();
	cam.vFov = glm::radians(90.0f);
	orientation = {};

	disableCursor(s.getWindow());

	int wWidth, wHeight;
	s.getSize(&wWidth, &wHeight);
	windowResize(s, wWidth, wHeight);

	{
		pausedText.setText("Paused...");
		pausedText.setSize(3);
		pausedText.setShadow(true);
		pausedText.setAlignX(ui::ALIGN_CENTER_X);
		pausedText.setOffsetY(300);

		backToGameButton.setText("Back To Game");
		backToGameButton.setAction([this, &s]()
			{
				disableCursor(s.getWindow());
				s.setUiPage(nullptr);
			}
		);
		backToGameButton.setSize(210, 50);
		backToGameButton.setAlignX(ui::ALIGN_CENTER_X);
		backToGameButton.setOffsetY(450);

		quitToTitleButton.setText("Quit To Title");
		quitToTitleButton.setAction([this, &s]()
			{
				// TODO: save world data
				s.setUiPage(nullptr);
				s.changeState(StateTitleScreen::instance());
			}
		);
		quitToTitleButton.setSize(230, 50);
		quitToTitleButton.setAlignX(ui::ALIGN_CENTER_X);
		quitToTitleButton.setOffsetY(550);

		// TODO: change these into checkboxes maybe
		toggleShadowsButton.setText(std::format("Shadows: {}", shadows ? "On" : "Off"));
		toggleShadowsButton.setAction([this, &s]()
			{
				shadows = !shadows;
				toggleShadowsButton.setText(std::format("Shadows: {}", shadows ? "On" : "Off"));
			});
		toggleShadowsButton.setSize(210, 50);
		toggleShadowsButton.setAlignX(ui::ALIGN_LEFT);
		toggleShadowsButton.setAlignY(ui::ALIGN_BOTTOM);
		toggleShadowsButton.setOffsetX(10);
		toggleShadowsButton.setOffsetY(-80);

		toggleAOButton.setText(std::format("AO: {}", ambientOcclusion ? "On" : "Off"));
		toggleAOButton.setAction([this, &s]()
			{
				ambientOcclusion = !ambientOcclusion;
				toggleAOButton.setText(std::format("AO: {}", ambientOcclusion ? "On" : "Off"));
			});
		toggleAOButton.setSize(210, 50);
		toggleAOButton.setAlignX(ui::ALIGN_LEFT);
		toggleAOButton.setAlignY(ui::ALIGN_BOTTOM);
		toggleAOButton.setOffsetX(10);
		toggleAOButton.setOffsetY(-10);

		pauseMenu.clear();
		pauseMenu.addElem(&pausedText);
		pauseMenu.addElem(&backToGameButton);
		pauseMenu.addElem(&quitToTitleButton);
		pauseMenu.addElem(&toggleShadowsButton);
		pauseMenu.addElem(&toggleAOButton);
	}

	{
		fpsText.setText("");
		fpsText.setSize(2);
		fpsText.setShadow(true);
		fpsText.setAlignX(ui::ALIGN_LEFT);
		fpsText.setAlignY(ui::ALIGN_TOP);
		fpsText.setOffsetX(5);
		fpsText.setOffsetY(5);

		coordsText.setText("");
		coordsText.setSize(1);
		coordsText.setShadow(true);
		coordsText.setAlignX(ui::ALIGN_LEFT);
		coordsText.setAlignY(ui::ALIGN_TOP);
		coordsText.setOffsetX(5);
		coordsText.setOffsetY(25);

		ui.clear();
		ui.addElem(&fpsText);
		ui.addElem(&coordsText);
		ui.init(s.getWindow());
	}

	{
		console.qr = QuadRenderer{ Shader::get("quad") };
		console.qr.init();

		console.inputText.setShadow(false);
		console.inputText.setAlignX(ui::ALIGN_LEFT);
		console.inputText.setAlignY(ui::ALIGN_TOP);
		console.inputText.setOffsetX(5);
		console.inputText.setOffsetY(console.height - 8 - 5);
		console.inputText.setSize(1);

		console.logText.setShadow(false);
		console.logText.setAlignX(ui::ALIGN_LEFT);
		console.logText.setAlignY(ui::ALIGN_TOP);
		console.logText.setOffsetX(5);
		console.logText.setOffsetY(console.height - 8 * 2 - 5);
		console.logText.setSize(1);
		console.logText.setWrapWidth(wWidth);

		updateConsoleInput();

		console.ui.clear();
		console.ui.addElem(&console.inputText);
		console.ui.addElem(&console.logText);
	}

	createWorld(3);

	cam.pos = {
		32.0f,
		world.getEdgeLength() * Chunk::SIZE / 2.0f,
		world.getEdgeLength() * Chunk::SIZE / 2.0f,
		world.getEdgeLength() * Chunk::SIZE / 2.0f,
		world.getEdgeLength() * Chunk::SIZE / 2.0f
	};
	for (int e = 0; e < world.getEdgeLength() * Chunk::SIZE; ++e)
	{
		for (int d = 0; d < world.getEdgeLength() * Chunk::SIZE; ++d)
		{
			for (int c = 0; c < world.getEdgeLength() * Chunk::SIZE; ++c)
			{
				for (int b = 0; b < world.getEdgeLength() * Chunk::SIZE; ++b)
				{
					for (int a = 0; a < 16; ++a)
					{
						if (a >= 15)
						{
							world.setBlock({ a,b,c,d,e }, Block::GRASS);
						}
						else if (a >= 12)
						{
							world.setBlock({ a,b,c,d,e }, Block::DIRT);
						}
						else if (a >= 0)
						{
							world.setBlock({ a,b,c,d,e }, Block::STONE);
						}
					}
				}
			}
		}
	}

	// init renderer buffers
	chunksBuffer = TextureBuffer{ 1, 1, 1, TextureFormat::RGBA32u };
	blockDataHandlesBuffer = GPUBuffer{ sizeof(uint64_t) };
	std::array<int, 10 * Block::COUNT> allTextures{ 0 };
	for (int i = 0; i < Block::COUNT; ++i)
	{
		std::memcpy(allTextures.data() + i * 10, Block::blocks[i].textures, sizeof(Block::blocks[i].textures));
	}
	blockTexturesBuffer = GPUBuffer{ sizeof(allTextures), allTextures.data() };

	// TODO: maybe handle this with a json file or something
	std::vector<uint64_t> textures{};
	Texture::load("assets/textures/blocks/dirt.ndtf", 4, false, "blocks.dirt", true); // 0
	Texture::load("assets/textures/blocks/grassSide.ndtf", 4, false, "blocks.grassSide", true); // 1
	Texture::load("assets/textures/blocks/grassTop.ndtf", 4, false, "blocks.grassTop", true); // 2
	Texture::load("assets/textures/blocks/stone.ndtf", 4, false, "blocks.stone", true); // 3
	Texture::load("assets/textures/blocks/woodSide.ndtf", 4, false, "blocks.woodSide", true); // 4
	Texture::load("assets/textures/blocks/woodTop.ndtf", 4, false, "blocks.woodTop", true); // 5
	Texture::load("assets/textures/blocks/glass.ndtf", 4, false, "blocks.glass", true); // 6
	Texture::load("assets/textures/blocks/bricks.ndtf", 4, false, "blocks.bricks", true); // 7
	textures.emplace_back(Texture::get("blocks.dirt")->getHandle());
	textures.emplace_back(Texture::get("blocks.grassSide")->getHandle());
	textures.emplace_back(Texture::get("blocks.grassTop")->getHandle());
	textures.emplace_back(Texture::get("blocks.stone")->getHandle());
	textures.emplace_back(Texture::get("blocks.woodSide")->getHandle());
	textures.emplace_back(Texture::get("blocks.woodTop")->getHandle());
	textures.emplace_back(Texture::get("blocks.glass")->getHandle());
	textures.emplace_back(Texture::get("blocks.bricks")->getHandle());
	tileTextureHandles = GPUBuffer{ sizeof(uint64_t) * textures.size(), textures.data() };

	updateRendererData();
}

void StateGame::close(StateManager& s)
{
	if (s.getUiPage() != nullptr)
	{
		enableCursor(s.getWindow());
		s.setUiPage(nullptr);
	}
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

	if (keys.space) cam.pos.a += 7.0f * dt;
	if (keys.shift) cam.pos.a -= 7.0f * dt;

	coordsText.setText(std::format(
		"Pos: A:{:+.1f} B:{:+.1f} C:{:+.1f} D:{:+.1f} E:{:+.1f}\n"
		"Up: A:{:+.1f} B:{:+.1f} C:{:+.1f} D:{:+.1f} E:{:+.1f}\n"
		"Forward: A:{:+.1f} B:{:+.1f} C:{:+.1f} D:{:+.1f} E:{:+.1f}\n"
		"Left: A:{:+.1f} B:{:+.1f} C:{:+.1f} D:{:+.1f} E:{:+.1f}\n"
		"Over: A:{:+.1f} B:{:+.1f} C:{:+.1f} D:{:+.1f} E:{:+.1f}\n"
		"Yonder: A:{:+.1f} B:{:+.1f} C:{:+.1f} D:{:+.1f} E:{:+.1f}\n",
		cam.pos.a, cam.pos.b, cam.pos.c, cam.pos.d, cam.pos.e,
		cam.up.a, cam.up.b, cam.up.c, cam.up.d, cam.up.e,
		cam.forward.a, cam.forward.b, cam.forward.c, cam.forward.d, cam.forward.e,
		cam.left.a, cam.left.b, cam.left.c, cam.left.d, cam.left.e,
		cam.over.a, cam.over.b, cam.over.c, cam.over.d, cam.over.e,
		cam.yonder.a, cam.yonder.b, cam.yonder.c, cam.yonder.d, cam.yonder.e
	));

	if (console.open)
	{
		if (glfwGetInputMode(s.getWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
		{
			enableCursor(s.getWindow());
		}
		if (s.getUiPage() != &console.ui)
		{
			s.setUiPage(&console.ui);
		}
	}
}

void StateGame::render(StateManager& s)
{
	int wW, wH;
	s.getSize(&wW, &wH);

	static double lastTime = glfwGetTime();
	double curTime = glfwGetTime();
	double dt = curTime - lastTime;
	lastTime = curTime;

	double fps = 1.0 / dt;
	fpsText.setText(std::format("FPS: {:.1f}", fps));

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);

	cameraBuf.uploadData<Camera>(&cam);

	rendererShader->use();
	blockDataHandlesBuffer.use(1);
	blockTexturesBuffer.use(2);
	tileTextureHandles.use(3);
	rendererShader->setUniform("shadows", shadows);
	rendererShader->setUniform("ambientOcclusion", ambientOcclusion);
	rendererShader->setUniform("lightDir", lightDir);
	rendererShader->setUniform("time", (float)glfwGetTime());
	QuadRendererBasic::render();

	glDisable(GL_DEPTH_TEST);
	ui.render();

	if (console.open)
	{
		console.qr.setMode(QuadRenderer::MODE_FILL);
		console.qr.setPos(0, 0, wW, console.height);
		console.qr.setColor({ 0.05f, 0.01f, 0.05f, 0.8f });
		console.qr.render();

		console.qr.setMode(QuadRenderer::MODE_LINES);
		console.qr.setPos(1, 1, wW - 1, console.height - 1);
		console.qr.setColor({ 1.0f, 1.0f, 1.0f, 0.8f });
		console.qr.render();

		int size = console.inputText.getSize();
		int x = 0, y = 0, w = 0, h = 0;
		console.inputText.getBounds(&console.ui, &x, &y, &w, &h);
		console.qr.setPos(x + 8 * (console.cursorPos + 2) * size, y, 7 * size, 8 * size);
		console.qr.render();

		x = 0; y = 0; w = 0; h = 0;
		console.logText.setWrapWidth(wW);
		console.logText.getBounds(&console.ui, &x, &y, &w, &h);
		console.logText.setOffsetY(console.height - 8 - 5 - h);
	}
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

void StateGame::charInput(StateManager& s, uint32_t codepoint)
{
	if (console.open)
	{
		if (console.blockCharInput)
		{
			console.blockCharInput = false;
			return;
		}

		if (codepoint >= 1 && codepoint <= 127)
		{
			console.input.insert(console.cursorPos++, 1, (char)codepoint);
		}

		updateConsoleInput();
	}

}
void StateGame::updateConsoleInput()
{
	console.inputText.setText("> " + (console.input.empty() ? "Enter a command. (\"help\" for a list of commands)" : console.input));
	console.inputText.setColor(console.input.empty() ? glm::vec4{ 0.65f, 0.65f, 0.65f, 1.0f } : glm::vec4{ 1.0f });
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
	if (console.open)
	{
		if (action == GLFW_RELEASE) return;

		switch (key)
		{
		case GLFW_KEY_BACKSPACE:
		{
			if (mods & GLFW_MOD_CONTROL)
			{
				size_t next = console.input.rfind(' ', console.cursorPos);
				if (next == std::string::npos) next = 0;
				console.input.erase(next, console.cursorPos - next);
				console.cursorPos = next;
			}
			else
			{
				console.cursorPos = glm::clamp(console.cursorPos - 1, 0, (int)console.input.size());
				console.input.erase(console.cursorPos, 1);
			}
		} break;
		case GLFW_KEY_DELETE:
		{
			if (mods & GLFW_MOD_CONTROL)
			{
				size_t next = console.input.find_first_of(' ', glm::clamp(console.cursorPos + 1, 0, (int)console.input.size()));
				if (next == std::string::npos) next = console.input.size();
				console.input.erase(console.cursorPos, next - console.cursorPos);
			}
			else
			{
				console.input.erase(console.cursorPos, 1);
			}
		} break;
		case GLFW_KEY_LEFT:
		{
			console.cursorPos = glm::clamp(console.cursorPos - 1, 0, (int)console.input.size());
			if (mods & GLFW_MOD_CONTROL)
			{
				size_t next = console.input.rfind(' ', console.cursorPos);
				if (next == std::string::npos) next = 0;
				console.cursorPos = next;
			}
		} break;
		case GLFW_KEY_RIGHT:
		{
			console.cursorPos = glm::clamp(console.cursorPos + 1, 0, (int)console.input.size());
			if (mods & GLFW_MOD_CONTROL)
			{
				size_t next = console.input.find_first_of(' ', console.cursorPos);
				if (next == std::string::npos) next = console.input.size();
				console.cursorPos = next;
			}
		} break;
		case GLFW_KEY_HOME:
		{
			console.cursorPos = 0;
		} break;
		case GLFW_KEY_END:
		{
			console.cursorPos = console.input.size();
		} break;
		case GLFW_KEY_ESCAPE:
		{
			if (action == GLFW_PRESS)
			{
				console.open = false;
				disableCursor(s.getWindow());
				s.setUiPage(nullptr);
			}
		} break;
		case GLFW_KEY_ENTER:
		{
			exec(console.input);
			console.input = "";
			console.cursorPos = 0;
		} break;
		case GLFW_KEY_UP:
		{
			console.input = console.history[console.historyIndex];
			console.cursorPos = console.input.size();

			console.historyIndex = (console.historyIndex + 1) % console.historyCount;
		} break;
		case GLFW_KEY_DOWN:
		{
			if (console.historyIndex > 0)
			{
				console.historyIndex = (console.historyIndex - 1) % console.historyCount;

				console.input = console.history[console.historyIndex];
				console.cursorPos = console.input.size();
			}
		} break;
		}
		updateConsoleInput();
		return;
	}

	if (action == GLFW_REPEAT) return;

	switch (key)
	{
	case GLFW_KEY_0:
	case GLFW_KEY_1:
	case GLFW_KEY_2:
	case GLFW_KEY_3:
	case GLFW_KEY_4:
	case GLFW_KEY_5:
	case GLFW_KEY_6:
	{
		if (action == GLFW_PRESS)
		{
			world.setBlock(cam.pos, key - GLFW_KEY_0);
			updateRendererData();
		}
	} break;
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
	case GLFW_KEY_ESCAPE:
	{
		if (action == GLFW_PRESS)
		{
			console.open = false;
			if (s.getUiPage() == nullptr)
			{
				enableCursor(s.getWindow());
				s.setUiPage(&pauseMenu);
			}
			else
			{
				disableCursor(s.getWindow());
				s.setUiPage(nullptr);
			}
		}
	} break;
	case GLFW_KEY_GRAVE_ACCENT:
	{
		if (action == GLFW_PRESS)
		{
			console.open = !console.open;

			if (console.open)
			{
				enableCursor(s.getWindow());
				s.setUiPage(&console.ui);
				console.blockCharInput = true;
			}
			else
			{
				disableCursor(s.getWindow());
				s.setUiPage(nullptr);
			}
		}
	} break;
	}
}
std::optional<m5::vec5> StateGame::positionArg(const std::vector<std::string>& args, int& cursor)
{
	if (cursor + 5 > args.size())
	{
		return std::nullopt;
	}

	m5::vec5 result{};

	try
	{
		for (int i = 0; i < 5; ++i)
		{
			const std::string& arg = args[cursor + i];
			if (arg[0] == '~')
			{
				result[i] = cam.pos[i];

				if (arg.size() > 2 && (arg[1] == '+' || arg[1] == '-'))
				{
					float x = std::stof(arg.substr(2));
					result[i] += x * (arg[1] == '+' ? 1 : -1);
				}
			}
			else
			{
				float x = std::stof(arg);
				result[i] = x;
			}
		}
	}
	catch (const std::exception&)
	{
		return std::nullopt;
	}

	cursor += 5;

	return result;
}
std::optional<int64_t> StateGame::intArg(const std::vector<std::string>& args, int& cursor, int64_t min, int64_t max)
{
	if (cursor + 1 > args.size())
	{
		return std::nullopt;
	}

	int64_t result;

	try
	{
		const std::string& arg = args[cursor];
		int64_t x = std::stoll(arg);
		result = x;
	}
	catch (const std::exception&)
	{
		return std::nullopt;
	}

	if (result < min || result > max)
	{
		return std::nullopt;
	}

	cursor += 1;

	return result;
}
void StateGame::exec(std::string_view cmd)
{
	std::string c = std::string{ cmd };
	utils::trim(c);

	if (c.empty())
	{
		return;
	}

	{
		auto history = console.history;
		std::move(history.begin(), history.end() - 1, console.history.begin() + 1);
	}

	console.history[0] = c;
	console.historyIndex = 0;
	console.historyCount = glm::min(console.historyCount + 1, (int)console.history.size());

	auto args = utils::splitStr(c, ' ');
	std::remove_if(args.begin(), args.end(), [](const std::string& str) { return str.empty(); });

	// switch(string) at home:
	std::unordered_map<std::string, std::function<bool()>> commands
	{
		{ "help", [&]()
		{
			print(
				"- help - Outputs this list.\n"
				"- clear - Clears the log.\n"
				"- setBlock <pos> <id> - Sets a block at a position.\n"
				"- fill <posA> <posB> <id> - Fills a region with a block."
			);
			return true;
		}},
		{ "clear", [&]()
		{
			console.log.clear();
			console.logText.setText(console.log);
			return true;
		}},
		{ "setBlock", [&]()
		{
			int cursor = 1;

			auto pos = positionArg(args, cursor);
			if (!pos.has_value())
			{
				return false;
			}
			auto id = intArg(args, cursor, 0, Block::COUNT - 1);
			if (!id.has_value())
			{
				return false;
			}

			world.setBlock(pos.value(), id.value());
			updateRendererData();

			return true;
		}},
		{ "fill", [&]()
		{
			int cursor = 1;

			auto posA = positionArg(args, cursor);
			if (!posA.has_value())
			{
				return false;
			}
			auto posB = positionArg(args, cursor);
			if (!posB.has_value())
			{
				return false;
			}
			auto id = intArg(args, cursor, 0, Block::COUNT - 1);
			if (!id.has_value())
			{
				return false;
			}

			int minA = glm::min(posA.value().a, posB.value().a); int maxA = glm::max(posA.value().a, posB.value().a);
			int minB = glm::min(posA.value().b, posB.value().b); int maxB = glm::max(posA.value().b, posB.value().b);
			int minC = glm::min(posA.value().c, posB.value().c); int maxC = glm::max(posA.value().c, posB.value().c);
			int minD = glm::min(posA.value().d, posB.value().d); int maxD = glm::max(posA.value().d, posB.value().d);
			int minE = glm::min(posA.value().e, posB.value().e); int maxE = glm::max(posA.value().e, posB.value().e);
			for (int e = minE; e <= maxE; ++e)
				for (int d = minD; d <= maxD; ++d)
					for (int c = minC; c <= maxC; ++c)
						for (int b = minB; b <= maxB; ++b)
							for (int a = minA; a <= maxA; ++a)
							{
								world.setBlock({ a,b,c,d,e }, id.value());
							}
			updateRendererData();

			return true;
		}},
	};

	if (commands.contains(args[0]))
	{
		if (!commands[args[0]]())
		{
			print(std::format("Failed to execute \"{}\".", cmd));
		}
		return;
	}

	print(std::format("Unknown command \"{}\"! Use \"help\" for a list of commands.", cmd));
}
void StateGame::print(std::string_view message)
{
	console.log += '\n';
	console.log += message;

	console.logText.setText(console.log);
}

void StateGame::windowResize(StateManager&, int width, int height)
{
	float w = width;
	float h = height;
	float invW = 1.0f / w;
	float invH = 1.0f / h;
	projection3D = glm::perspective(cam.vFov, w * invH, 0.03f, 1000.0f);

	rendererShader->setUniform("screenSize", w, h, invW, invH);
}

World* StateGame::createWorld(uint8_t edgeLength)
{
	world = World{ edgeLength };
	return &world;
}

void StateGame::enableCursor(GLFWwindow* window)
{
	glfwSetCursorPos(window, lastMousePos.x, lastMousePos.y);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void StateGame::disableCursor(GLFWwindow* window)
{
	glfwGetCursorPos(window, &lastMousePos.x, &lastMousePos.y);

	//glfwSetCursorPos(window, 0.0, 0.0);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

constexpr uint8_t SECTIONS = 4u; // RGBA channels
constexpr uint8_t SECTION_HEIGHT = Chunk::HEIGHT / SECTIONS;
constexpr uint8_t BLOCK_BITS = 4u;
constexpr uint8_t BLOCKS_VERTICAL = 32u / BLOCK_BITS;
constexpr uint32_t CHUNK_BLOCKS = SECTION_HEIGHT * Chunk::SIZE * Chunk::SIZE * Chunk::SIZE * Chunk::SIZE / BLOCKS_VERTICAL;
using BlockArray = std::array<uint32_t, CHUNK_BLOCKS>;

void StateGame::updateRendererData()
{
	glm::ivec4 minC{ 0 }, maxC{ (int)world.getEdgeLength() - 1 };
	glm::ivec4 size = maxC - minC + 1;

	uint32_t sectionSize = size.x * size.y * size.z * size.w;
	uint32_t* chunks = new uint32_t[sectionSize * SECTIONS](0);

	int tex = 0;
	for (int b = 0; b < size.x; ++b)
	for (int c = 0; c < size.y; ++c)
	for (int d = 0; d < size.z; ++d)
	for (int e = 0; e < size.w; ++e)
	{
		glm::ivec4 pos{ b,c,d,e };
		Chunk* chunk = world.getChunk(pos);

		if (!chunk)
		{
			continue;
		}

		glm::uvec4 uPos = pos - minC;

		uint32_t ind = uPos.x + uPos.y * size.x + uPos.z * size.x * size.y + uPos.w * size.x * size.y * size.z;

		for (int s = 0; s < SECTIONS; ++s)
		{
			BlockArray blocks{ 0 };

			bool empty = true;
			if (!chunk->shouldUpdateRenderer[s])
			{
				chunks[ind * SECTIONS + s] = chunk->rendererHandleIndices[s];
				continue;
			}
			for (int ae = 0; ae < Chunk::SIZE; ++ae)
				for (int ad = 0; ad < Chunk::SIZE; ++ad)
					for (int ac = 0; ac < Chunk::SIZE; ++ac)
						for (int ab = 0; ab < Chunk::SIZE; ++ab)
						{
							uint32_t packed[SECTIONS]{ 0 };
							bool colNonEmpty = false;

							for (int c = 0; c < SECTIONS; ++c)
							{
								uint32_t v = 0;

								for (int i = 0; i < BLOCKS_VERTICAL; ++i)
								{
									int aa = c * BLOCKS_VERTICAL + i;
									uint32_t blockID = chunk->getBlock({ aa + s * SECTION_HEIGHT, ab, ac, ad, ae });

									v |= (blockID & 0xFu) << (i * BLOCK_BITS);
								}

								packed[c] = v;
								colNonEmpty |= (v != 0);
							}

							if (colNonEmpty)
							{
								empty = false;

								uint32_t texX = ab;
								uint32_t texY = ac;
								uint32_t texZ = ad + ae * Chunk::SIZE;

								uint32_t indB = texX + texY * Chunk::SIZE + texZ * Chunk::SIZE * Chunk::SIZE;

								blocks[indB * 4 + 0] = packed[0];
								blocks[indB * 4 + 1] = packed[1];
								blocks[indB * 4 + 2] = packed[2];
								blocks[indB * 4 + 3] = packed[3];
							}
						}

			if (!empty)
			{
				if (chunk->rendererTexIndices[s] && chunk->rendererHandleIndices[s] && chunk->rendererTexIndices[s] <= blockDataBuffers.size() && chunk->rendererHandleIndices[s] <= blockDataHandles.size())
				{
					blockDataBuffers[chunk->rendererTexIndices[s] - 1].uploadData(Chunk::SIZE, Chunk::SIZE, Chunk::SIZE * Chunk::SIZE, blocks.data());
					blockDataHandles[chunk->rendererHandleIndices[s] - 1] = blockDataBuffers[chunk->rendererTexIndices[s] - 1].getHandle();
					//printf("updated %llu:%llu\n", chunk->rendererTexIndices[s] - 1, chunk->rendererHandleIndices[s] - 1);
				}
				else
				{
					blockDataBuffers.emplace_back(TextureBuffer{ Chunk::SIZE, Chunk::SIZE, Chunk::SIZE * Chunk::SIZE, TextureFormat::RGBA32u, blocks.data() });
					blockDataHandles.emplace_back(blockDataBuffers.back().getHandle());
					chunk->rendererTexIndices[s] = blockDataBuffers.size(); // index + 1
					chunk->rendererHandleIndices[s] = blockDataHandles.size(); // index + 1
					//printf("added %llu:%llu\n", chunk->rendererTexIndices[s] - 1, chunk->rendererHandleIndices[s] - 1);
				}
			}
			else
			{
				chunk->rendererTexIndices[s] = 0;
				chunk->rendererHandleIndices[s] = 0;
			}

			chunks[ind * SECTIONS + s] = chunk->rendererHandleIndices[s];
			chunk->shouldUpdateRenderer[s] = false;
		}
	}

	blockDataHandlesBuffer.uploadData(0, sizeof(uint64_t) * blockDataHandles.size(), blockDataHandles.data());
	chunksBuffer.uploadData(size.x, size.y, size.z * size.w, chunks);

	delete[] chunks;

	chunks = nullptr;

	{
		rendererShader->setUniform("chunksMinBound", minC);
		rendererShader->setUniform("chunksSize", size);
		rendererShader->setUniform("chunks", chunksBuffer.getHandle());
	}
}
