#pragma once

#include "Sprite.h"
#include "Assert.h"
#include "Renderer.h"
#include "ShapesData.h"

#include "Trinacria/Macros.h"
#include <cstring>

namespace TRCN_CORE_NAMESPACE
{
	template<size_t SpriteNum>
	class Animation
	{
	public:
		void Play(float deltaTime, const glm::vec2& position, const glm::vec3& color = 1.f, int materialIndex = 0)
		{
			_accumulatedDeltaTime += deltaTime;

			if (_accumulatedDeltaTime >= 1.f / Fps)
			{
				_currentSpriteIndex++;

				if (_currentSpriteIndex > SpriteNum - 1)
				{
					_currentSpriteIndex = 0;
				}

				_accumulatedDeltaTime = 0.f;
			}

			Transform transform(position, glm::vec2(0.5f));
			QuadData data(transform, &_sprites[_currentSpriteIndex], materialIndex, color);

			Renderer::CreateQuad(data);
			
		}

		void Init(Sprite* array, size_t count, uint32_t fps)
		{
			if (!_initialized)
			{
				memcpy(_sprites, array, count * sizeof(Sprite));

				Fps = fps;
				_initialized = true;
			}
			else
			{
				TRCN_ASSERT(false, "You cannot initialize an asset more than one time");
			}

		}

		uint32_t Fps = 0;

	private:
		Sprite _sprites[SpriteNum];

		bool _initialized = false;

		float _accumulatedDeltaTime = 0.f;
		uint32_t _currentSpriteIndex = 0;
	};
}


