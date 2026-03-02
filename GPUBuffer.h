#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdint>

class GPUBuffer
{
public:
	enum AccessMode
	{
		READ_ONLY,
		WRITE_ONLY,
		READ_WRITE
	};

	enum BufferType
	{
		SHADER_STORAGE_BUFFER, // SSBO
		UNIFORM_BUFFER, // UBO
	};

private:
	uint32_t ID = NULL;
	size_t size = 0;

	void init(size_t size, const void* data = nullptr);

	static GLenum glAccess(AccessMode access);
	static GLenum glBufferType(BufferType type);

public:
	BufferType type = SHADER_STORAGE_BUFFER;

	GPUBuffer() {}
	GPUBuffer(size_t size, const void* data = nullptr);
	template<typename T>
	GPUBuffer(const T* data = nullptr) : GPUBuffer(sizeof(T), data) {}

	void uploadData(size_t offset, size_t size, const void* data);
	void uploadData(size_t size, const void* data)
	{
		uploadData(0, size, data);
	}
	template<typename T>
	void uploadData(size_t offset, const T* data)
	{
		uploadData(offset, sizeof(T), (const void*)data);
	}
	template<typename T>
	void uploadData(const T* data)
	{
		uploadData(sizeof(T), (const void*)data);
	}

	// might clear the data! use with caution!
	void fit(size_t size);

	// might clear the data! use with caution!
	void resize(size_t size);

	void cleanup();

	uint32_t id() const
	{
		return ID;
	}

	void use(uint32_t index) const
	{
		if (ID)
		{
			glBindBufferBase(glBufferType(type), index, ID);
		}
	}

	size_t getSize() const
	{
		return size;
	}

	GPUBuffer(GPUBuffer&& other) noexcept;
	GPUBuffer& operator=(GPUBuffer&& other) noexcept;

	~GPUBuffer();

	void* map(AccessMode access);
	void* map(size_t offset, size_t size, AccessMode access);

	template<typename T>
	T* map(AccessMode access)
	{
		return (T*)map(access);
	}
	template<typename T>
	T* map(size_t offset, AccessMode access)
	{
		return (T*)map(offset, sizeof(T), access);
	}

	void unmap();
};
