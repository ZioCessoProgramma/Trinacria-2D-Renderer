#include "Trinacria/Texture.h"
#include <stb_image/stb_image.h>
#include <glad/glad.h>
#include <iostream>

TRCN_CORE_NAMESPACE::Texture::Texture(uint32_t id, int width, int height, int channels) : _id(id),
	_width(width), _height(height), _channels(channels)
{

}

void TRCN_CORE_NAMESPACE::Texture::LoadTexture(const std::string& path, uint32_t filter)
{
	uint8_t* data = stbi_load(path.c_str(), &_width, &_height, &_channels, 0);

	if (!data)
	{
		std::cout << "Path probabilmente invalido: Texture non trovata!\n";
		return;
	}

	glGenTextures(1, &_id);

	glBindTexture(GL_TEXTURE_2D, _id);

	uint32_t format = 0;

	switch (_channels)
	{
	case 1:
		format = GL_RED;
		break;
	case 2:
		format = GL_RG;
		break;
	case 3:
		format = GL_RGB;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		break;
	case 4:
		format = GL_RGBA;
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);
	

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  // Trilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	// Parametri di wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	stbi_image_free(data);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void TRCN_CORE_NAMESPACE::Texture::Bind(uint32_t textureSlot)
{
	glActiveTexture(textureSlot);
	glBindTexture(GL_TEXTURE_2D, _id);
}
