#pragma once
#include "Block.h"
#include <array>
#include <cstdint>
#include <cassert>

struct Chunk
{
	static constexpr size_t SIZE = 8;
	static constexpr size_t HEIGHT = 128;

	struct BlockColumn
	{
		// maximum 4-bit value, 15
		static constexpr uint8_t BLOCK_ID_MAX = 0b00001111ui8;

		uint8_t blockData[HEIGHT / 2]{};

		uint8_t getBlock(size_t index);
		void setBlock(size_t index, uint8_t value);
	};

	using BlockArr = std::array<std::array<std::array<std::array<BlockColumn, SIZE>, SIZE>, SIZE>, SIZE>;
	BlockArr blocks;
};