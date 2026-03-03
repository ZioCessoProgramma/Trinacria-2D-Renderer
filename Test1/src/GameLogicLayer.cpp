#include "GameLogicLayer.h"
#include "Application.h"
#include "RendererLayer.h"

void GameLogicLayer::OnUpdate(float deltaTime)
{
	RendererLayer* layer = Application::Get()->GetLayer<RendererLayer>();
	if (layer == nullptr) return;

	bool inDangerZone = _playerPos.x > 3 && _playerPos.x < 9 && _playerPos.y > 3 && _playerPos.y < 9;

	if (inDangerZone && !_wasInDangerZone)
	{
		DangerZoneEnteredDispatcher.Submit(this, DangerZoneEnteredEvent(1.f));
		_wasInDangerZone = true;
	}
	else if (!inDangerZone && _wasInDangerZone)
	{
		DangerZoneExitedDispatcher.Submit(this, DangerZoneExitedEvent(glm::vec3(1.f)));
		_wasInDangerZone = false;
	}

	layer->SetCameraData({ _cameraPos, ZOffset / 10 });
	layer->SetPlayerPos(_playerPos - glm::vec2(0.25f));
}

void GameLogicLayer::OnAttach()
{
	DangerZoneEnteredDispatcher.SetOwner(this);
	DangerZoneExitedDispatcher.SetOwner(this);

	Trinacria::InputPollerLayer* layer = Application::Get()->GetLayer<Trinacria::InputPollerLayer>();

	if (layer)
	{
		layer->KeyBoardDispatcher.RegistCallback(std::bind(&GameLogicLayer::keyBoardInput, this, std::placeholders::_1));
	}
}

void GameLogicLayer::keyBoardInput(const Trinacria::KeyboardEvent& event)
{
	const float speed = 3.f;

	float deltaTime = Application::Get()->GetDeltaTime();

	Trinacria::KeyValue key = event.GetKey();
	Trinacria::InputEventType type = event.GetType();

	using Trinacria::InputEventType;
	using Trinacria::KeyValue;
	
	if (key == KeyValue::Key_W && type == InputEventType::Pressed)
	{
		_cameraPos.y += speed * deltaTime;
	}

	if (key == KeyValue::Key_S && type == InputEventType::Pressed)
	{
		_cameraPos.y -= speed * deltaTime;
	}

	if (key == KeyValue::Key_A && type == InputEventType::Pressed)
	{
		_cameraPos.x -= speed * deltaTime;
	}

	if (key == KeyValue::Key_D && type == InputEventType::Pressed)
	{
		_cameraPos.x += speed * deltaTime;
	}

	if (key == KeyValue::Key_UP && type == InputEventType::Pressed)
	{
		_playerPos.y += speed * deltaTime;
	}

	if (key == KeyValue::Key_DOWN && type == InputEventType::Pressed)
	{
		_playerPos.y -= speed * deltaTime;
	}

	if (key == KeyValue::Key_LEFT && type == InputEventType::Pressed)
	{
		_playerPos.x -= speed * deltaTime;
	}

	if (key == KeyValue::Key_RIGHT && type == InputEventType::Pressed)
	{
		_playerPos.x += speed * deltaTime;
	}
	
}