#include "StateGame.h"
#include "StateTitleScreen.h"
#include "Shader.h"
#include "Texture.h"
#include "TextureBuffer.h"
#include "QuadRendererBasic.h"
#include "Directory.h"

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
	cam.up		= m5::vec5::up();
	cam.forward	= m5::vec5::forward();
	cam.left	= m5::vec5::left();
	cam.over	= m5::vec5::over();
	cam.yonder	= m5::vec5::yonder();
	cam.vFov = glm::radians(90.0f);
	orientation = {};

	disableCursor(s.getWindow());

	int wWidth, wHeight;
	glfwGetWindowSize(s.getWindow(), &wWidth, &wHeight);
	windowResize(s, wWidth, wHeight);

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

	pauseMenu.clear();
	pauseMenu.addElem(&pausedText);
	pauseMenu.addElem(&backToGameButton);
	pauseMenu.addElem(&quitToTitleButton);

	createWorld(8);

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
}

void StateGame::render(StateManager& s)
{
	glDisable(GL_DEPTH_TEST);
	
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	cameraBuf.uploadData<Camera>(&cam);

	rendererShader->use();
	blockDataHandlesBuffer.use(1);
	blockTexturesBuffer.use(2);
	tileTextureHandles.use(3);
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
			if (s.getUiPage() == &pauseMenu)
			{
				disableCursor(s.getWindow());
				s.setUiPage(nullptr);
			}
			else
			{
				enableCursor(s.getWindow());
				s.setUiPage(&pauseMenu);
			}
		}
		break;
	}
	}
}

void StateGame::windowResize(StateManager&, int width, int height)
{
	rendererShader->setUniform("screenSize", (float)width, (float)height, 1.0f / width, 1.0f / height);
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
constexpr uint32_t CHUNK_BLOCKS = Chunk::SIZE * SECTION_HEIGHT * Chunk::SIZE * Chunk::SIZE * Chunk::SIZE / BLOCKS_VERTICAL;
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
			for (int ab = 0; ab < Chunk::SIZE; ++ab)
			{
				for (int ac = 0; ac < Chunk::SIZE; ++ac)
				{
					for (int ad = 0; ad < Chunk::SIZE; ++ad)
					{
						for (int ae = 0; ae < Chunk::SIZE; ++ae)
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
					}
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
