#pragma once

#include "Trinacria/Macros.h"

namespace TRCN_CORE_NAMESPACE
{
	TRCN_ABSTRACT class Layer
	{
	public:
		virtual void OnUpdate(float deltaTime) = 0;

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;

		virtual ~Layer() = default;
	};
}
