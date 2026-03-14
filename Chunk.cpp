#include "Chunk.h"

uint8_t Chunk::BlockColumn::getBlock(size_t index)
{
	assert(index < Chunk::HEIGHT);

	int shift = 4 * (index & 1);

	return (blockData[index >> 1] >> shift) & BLOCK_ID_MAX;
}

void Chunk::BlockColumn::setBlock(size_t index, uint8_t value)
{
	assert(index < Chunk::HEIGHT);
	assert(value <= BLOCK_ID_MAX);

	int shift = 4 * (index & 1);

	uint8_t& dst = blockData[index >> 1];
	// clear the destination bits
	dst &= 0b11110000ui8 >> shift;
	// apply the block-value bits
	dst |= value << shift;
}