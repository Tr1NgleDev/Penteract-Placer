#pragma once
#include "Chunk.h"
#include <memory>
#include <glm/vec4.hpp>

struct World
{
	World();
	World(uint8_t edgeLength);

	size_t getEdgeLength() const;

	// returns a pointer to the 4d array of chunks
	Chunk* getChunks();
	const Chunk* getChunks() const;

	Chunk* getChunk(glm::u8vec4 chunkPos);
	const Chunk* getChunk(glm::u8vec4 chunkPos) const;

private:

	uint8_t edgeLength;
	std::unique_ptr<Chunk[]> chunks;
};