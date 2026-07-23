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

		~Texture() = default;

		virtual int GetId() { return _id; }

		virtual int GetWidth() { return _width; }
		virtual int GetHeight() { return _height; }
		virtual int GetChannels() { return _channels; }

		virtual void LoadTexture(const std::string& path, uint32_t filter = GL_LINEAR);

		static void TexImage(uint32_t internalFormat, uint32_t format, uint32_t width, uint32_t height, uint32_t type, void* data, uint32_t filter);
		void BoundTexImage(uint32_t
		                   internalFormat, uint32_t format, uint32_t width, uint32_t height, uint32_t type, void* data, uint32_t filter);

		virtual void Bind(uint32_t textureSlot);
		virtual void Bind(); // to first available texture slot
		virtual void Cleanup();

		static void ClearTextureSlots();

		uint32_t GetTextureChosenSlot() const {return _textureSlotChosen;}

		void Init();

		static constexpr Texture* NO_TEXTURE = nullptr;

	private:
		uint32_t _id;
		int _width, _height;
		int _channels;
		uint32_t _textureSlotChosen = -1; // value for not choosing any

		static bool _isTextureSlotOccupied[32];
	};
}