#include "World.h"
#include <glm/common.hpp>

World::World() : edgeLength{ 0 }
{
}

World::World(uint8_t edgeLength) :
	edgeLength{ edgeLength },
	chunks{ std::make_unique<Chunk[]>(edgeLength * edgeLength * edgeLength * edgeLength) }
{
	assert(edgeLength > 0);
}

size_t World::getEdgeLength() const
{
	return edgeLength;
}

size_t World::getDataSize() const
{
	return sizeof(Chunk) * edgeLength * edgeLength * edgeLength * edgeLength;
}

Chunk* World::getChunks()
{
	return chunks.get();
}

const Chunk* World::getChunks() const
{
	return chunks.get();
}

Chunk* World::getChunk(glm::u8vec4 chunkPos)
{
	if (glm::max(chunkPos, glm::u8vec4{ (uint8_t)(edgeLength - 1) }) != glm::u8vec4{ (uint8_t)(edgeLength - 1) })
	{
		return nullptr;
	}

	size_t index =
		(size_t)chunkPos.x * (edgeLength * edgeLength * edgeLength) +
		(size_t)chunkPos.y * (edgeLength * edgeLength) +
		(size_t)chunkPos.z * (edgeLength) +
		(size_t)chunkPos.w;

	return &chunks[index];
}

const Chunk* World::getChunk(glm::u8vec4 chunkPos) const
{
	if (glm::max(chunkPos, glm::u8vec4{ (uint8_t)(edgeLength - 1) }) != glm::u8vec4{ (uint8_t)(edgeLength - 1) })
	{
		return nullptr;
	}

	size_t index =
		(size_t)chunkPos.x * (edgeLength * edgeLength * edgeLength) +
		(size_t)chunkPos.y * (edgeLength * edgeLength) +
		(size_t)chunkPos.z * (edgeLength)+
		(size_t)chunkPos.w;

	return &chunks[index];
}

Chunk* World::getChunkFromCoords(const glm::vec4& pos)
{
	glm::u8vec4 chunkPos = glm::floor(pos / (float)Chunk::SIZE);
	return getChunk(chunkPos);
}

const Chunk* World::getChunkFromCoords(const glm::vec4& pos) const
{
	glm::u8vec4 chunkPos = glm::floor(pos / (float)Chunk::SIZE);
	return getChunk(chunkPos);
}

uint8_t World::getBlock(const m5::uvec5& block) const
{
	if (m5::max(block, (getSize() - 1)) != (getSize() - 1))
	{
		return 0;
	}

	if (block.a >= Chunk::HEIGHT)
	{
		return 0;
	}

	const Chunk* c = getChunkFromCoords(block.b, block.c, block.d, block.e);
	if (c)
	{
		// coords must be floats
		m5::vec5 coords = block;

		m5::uvec5 b
		{
			coords.a,
			coords.b - (glm::floor(coords.b / Chunk::SIZE) * Chunk::SIZE),
			coords.c - (glm::floor(coords.c / Chunk::SIZE) * Chunk::SIZE),
			coords.d - (glm::floor(coords.d / Chunk::SIZE) * Chunk::SIZE),
			coords.e - (glm::floor(coords.e / Chunk::SIZE) * Chunk::SIZE),
		};

		return c->getBlock(b);
	}

	return Block::AIR;
}

void World::setBlock(const m5::uvec5& block, uint8_t value)
{
	if (m5::max(block, (getSize() - 1)) != (getSize() - 1))
	{
		return;
	}

	if (block.a >= Chunk::HEIGHT)
	{
		return;
	}

	Chunk* c = getChunkFromCoords(block.b, block.c, block.d, block.e);
	if (c)
	{
		// coords must be floats
		m5::vec5 coords = block;

		m5::uvec5 b
		{
			coords.a,
			coords.b - (glm::floor(coords.b / Chunk::SIZE) * Chunk::SIZE),
			coords.c - (glm::floor(coords.c / Chunk::SIZE) * Chunk::SIZE),
			coords.d - (glm::floor(coords.d / Chunk::SIZE) * Chunk::SIZE),
			coords.e - (glm::floor(coords.e / Chunk::SIZE) * Chunk::SIZE),
		};

		c->setBlock(b, value);
	}
}