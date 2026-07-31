#pragma once

#include <vector>
#include <Trinacria/Layer.h>
#include <glm/glm.hpp>

#include "Trinacria/Animation.h"


class Application
{
public:
	~Application() = default;

	static Application* Get();

	template<typename T>
	void PushLayer()
	{
		T* layer = new T;
		_layerStack.emplace(_layerStack.size() / 2 + _layerStack.begin(), layer);

		layer->OnAttach();
	}


	template<typename T>
	void PushOverlayLayer()
	{
		T* layer = new T;
		_layerStack.emplace_back(layer);

		layer->OnAttach();
	}

	void PushLayer(Trinacria::DSL::Layer* lay);
	void PushOverlayLayer(Trinacria::DSL::Layer* lay);

	template<typename T>
	void DeleteLayer()
	{
		for (int i = 0; i < _layerStack.size(); i++)
		{
			if (dynamic_cast<T*>(_layerStack[i]))
			{
				_layerStack[i]->OnDetach();
				delete _layerStack[i];

				_layerStack.erase(_layerStack.begin() + i);
			}
		}
	}

	template<typename T, typename U>
	void TransitionTo()
	{
		for (int i = 0; i < _layerStack.size(); i++)
		{
			if (dynamic_cast<T*>(_layerStack[i]))
			{
				_layerStack[i]->OnDetach();
				delete _layerStack[i];

				_layerStack[i] = new U;
			}
		}
	}

	template<typename T>
	void TransitionTo(Trinacria::DSL::Layer* newLayer)
	{
		for (int i = 0; i < _layerStack.size(); i++)
		{
			if (dynamic_cast<T>(_layerStack[i]))
			{
				_layerStack[i]->OnDetach();
				delete _layerStack[i];

				_layerStack[i] = newLayer;
			}
		}
	}

	template<typename T>
	T* GetLayer()
	{
		for (int i = 0; i < _layerStack.size(); i++)
		{
			if (T* layer = dynamic_cast<T*>(_layerStack[i]))
			{
				return dynamic_cast<T*>(_layerStack[i]);
			}
		}

		return nullptr;
	}

	float GetDeltaTime() { return _deltaTime; }

	glm::vec2 GetWindowDimensions();

	void Init();
	void Run();
	void CleanUp();

	struct GLFWwindow* GetWindow();

	float zOffset = 10.f;
	uint32_t _windowWidth = 800, _windowHeight = 600;


private:
	Application() = default;

	std::vector<Trinacria::DSL::Layer*> _layerStack;

	GLFWwindow* _window;

	float _deltaTime = 0.f;
};
