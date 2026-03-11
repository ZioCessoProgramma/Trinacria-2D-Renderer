#pragma once

#include "Texture.h"
#include "Renderer.h"
//#include "Assert.h"

namespace TRCN_CORE_NAMESPACE
{
	class SpriteSheet : public Texture
	{
	public:
		SpriteSheet(float cellWidth, float cellHeight) : CellWidth(cellWidth), CellHeight(cellHeight) { }

		QuadTexCoords GetCell(uint32_t xIndex, uint32_t yIndex);

		float CellWidth, CellHeight;
	};

	class Sprite
	{
	public:
		Sprite(SpriteSheet* parent, uint32_t xIndex, uint32_t yIndex, uint32_t xExtent = 1, uint32_t yExtent = 1) :
			_parent(parent), _xIndex(xIndex), _yIndex(yIndex), _xExtent(xExtent), _yExtent(yExtent)
		{
			_initialized = true;
		};

		Sprite() = default;

		void Init(SpriteSheet* parent, uint32_t xIndex, uint32_t yIndex, uint32_t xExtent = 1, uint32_t yExtent = 1);

		QuadTexCoords GetTexCoords();
		TriangleTexCoords GetTriangleTexCoords();
		SpriteSheet* GetParent();

	private:
		SpriteSheet* _parent;

		uint32_t _xIndex, _yIndex;
		uint32_t _xExtent, _yExtent;

		bool _initialized = false;
	};
}
