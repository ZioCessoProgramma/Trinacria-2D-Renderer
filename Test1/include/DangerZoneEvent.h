#pragma once

#include <Trinacria/Event.h>
#include <glm/glm.hpp>

enum class PlayerEvents
{
	OnDied,
	DangerZoneEntered,
	DangerZoneExited
};

class DangerZoneEnteredEvent : public Trinacria::DSL::Event<PlayerEvents>
{
public:
	DangerZoneEnteredEvent(float damageToApply) : _damageToApply(damageToApply),
		Event(PlayerEvents::DangerZoneEntered) { }
	
	float GetDamageToApply() const { return _damageToApply; }


private:
	float _damageToApply = 0;
};

class DangerZoneExitedEvent : public Trinacria::DSL::Event<PlayerEvents>
{
public:
	DangerZoneExitedEvent(const glm::vec3& colorToRestore) : _colorToRestore(colorToRestore),
		Event(PlayerEvents::DangerZoneExited) { }

	glm::vec3 GetColorToRestore() const { return _colorToRestore; }

private:
	glm::vec3 _colorToRestore;
};
