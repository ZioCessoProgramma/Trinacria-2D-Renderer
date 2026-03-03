#pragma once

#include <Trinacria/Layer.h>

class ApplicationLayer : public Trinacria::Layer
{
public:
	virtual void OnAttach() override;
	virtual void OnDetach() override {	}

	virtual void OnUpdate(float deltaTime) override {	}
};
