#pragma once

#include <stdint.h>
#include <string>

#include <glad/glad.h>
#include "Trinacria/Macros.h"

namespace TRCN_CORE_NAMESPACE
{
	class Texture
	{
	public:
		Texture(uint32_t id, int width, int height, int channels);
		Texture() = default;

		virtual int GetId() { return _id; }

		virtual int GetWidth() { return _width; }
		virtual int GetHeight() { return _height; }
		virtual int GetChannels() { return _channels; }

		virtual void LoadTexture(const std::string& path, uint32_t filter = GL_LINEAR);

		virtual void Bind(uint32_t textureSlot);

		static constexpr Texture* NO_TEXTURE = nullptr;

	private:
		uint32_t _id;
		int _width, _height;
		int _channels;
	};
}