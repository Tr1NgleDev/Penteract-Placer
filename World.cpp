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
	assert(glm::max(chunkPos, glm::u8vec4{ (uint8_t)(edgeLength - 1) }) == glm::u8vec4{ (uint8_t)(edgeLength - 1) });

	size_t index =
		(size_t)chunkPos.x + (edgeLength * edgeLength * edgeLength) +
		(size_t)chunkPos.y + (edgeLength * edgeLength) +
		(size_t)chunkPos.z + (edgeLength) +
		(size_t)chunkPos.w;

	return &chunks[index];
}

const Chunk* World::getChunk(glm::u8vec4 chunkPos) const
{
	assert(glm::max(chunkPos, glm::u8vec4{ (uint8_t)(edgeLength - 1) }) == glm::u8vec4{ (uint8_t)(edgeLength - 1) });

	size_t index =
		(size_t)chunkPos.x + (edgeLength * edgeLength * edgeLength) +
		(size_t)chunkPos.y + (edgeLength * edgeLength) +
		(size_t)chunkPos.z + (edgeLength)+
		(size_t)chunkPos.w;

	return &chunks[index];
}