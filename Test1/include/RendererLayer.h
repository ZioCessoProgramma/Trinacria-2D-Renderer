#pragma once

#include <Trinacria/Layer.h>
#include <Trinacria/Renderer.h>
#include <Trinacria/Sprite.h>
#include <Trinacria/Animation.h>

#include "DangerZoneEvent.h"

#define QUAD_COUNT 145
#define VERTICES_COUNT QUAD_COUNT * 4
#define INDICES_COUNT QUAD_COUNT * 6

struct CameraData
{
	glm::vec3 CameraPos;
	float Zoom;
};

class RendererLayer : public Trinacria::Layer
{
public:
	virtual void OnUpdate(float deltaTime);

	virtual void OnAttach();
	virtual void OnDetach();

	void SetCameraData(const CameraData& cameraData);
	void SetPlayerPos(glm::vec2 playerPos);

	static void OnEnteredDangerZone(const DangerZoneEnteredEvent& event);

private:
	glm::vec2 _playerPos{ 0.f };

	Trinacria::Texture _grey;
	Trinacria::Texture _purple;
	Trinacria::Texture _wood;

	Trinacria::QuadTexCoords _lastCoords{ glm::vec2(0.f, 2.f / 3), glm::vec2(1.f / 3, 2.f / 3), glm::vec2(1.f / 3, 1.f), glm::vec2(0.f, 1.f) };

	Trinacria::SpriteSheet _playerAnimationSpriteSheet{ 0.f, 0.f };

	Trinacria::SpriteSheet _tinyVillage{ 0.f, 0.f };

	Trinacria::Sprite _animation[3] = { Trinacria::Sprite(&_playerAnimationSpriteSheet, 0, 2), Trinacria::Sprite(&_playerAnimationSpriteSheet, 1, 2),
		Trinacria::Sprite(&_playerAnimationSpriteSheet, 2, 2) };

	Trinacria::Animation<3> _playerAnimation;

	Trinacria::Sprite _tree{ &_tinyVillage, 3, 9, 1, 2 };

	uint32_t _currentSpriteIndex = 0;

	CameraData _cameraData;
};

