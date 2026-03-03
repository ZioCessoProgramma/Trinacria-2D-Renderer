#pragma once

#include <Trinacria/Layer.h>
#include "DangerZoneEvent.h"
#include <cassert>
#include <glm/glm.hpp>
#include <Trinacria/InputEvents.h>

class GameLogicLayer : public Trinacria::Layer
{
public:
	virtual void OnUpdate(float deltaTime) override;

	virtual void OnAttach() override;
	virtual void OnDetach() override {	}

	float ZOffset = 10.f;

	Trinacria::EventDispatcher<DangerZoneEnteredEvent> DangerZoneEnteredDispatcher;
	Trinacria::EventDispatcher<DangerZoneExitedEvent> DangerZoneExitedDispatcher;

private:
	glm::vec3 _cameraPos{ 0.f };
	glm::vec2 _playerPos{ -1.5f };

	bool _wasInDangerZone = false;

	void keyBoardInput(const Trinacria::KeyboardEvent&);
};

