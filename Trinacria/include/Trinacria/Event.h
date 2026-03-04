#pragma once

#include "Trinacria/Macros.h"
#include "Log.h"
#include "Layer.h"
#include <functional>
#include <vector>

namespace TRCN_CORE_NAMESPACE
{
	template<typename Category>
	TRCN_ABSTRACT class Event
	{
	public:
		Event(Category type) : _type(type) { }

		Category GetType() const { return _type; }

	protected:
		Category _type;
	};

	template<typename EventType>
	class EventDispatcher
	{
		using CallbackType = std::function<void(const EventType& event)>;

	public:
		void RegistCallback(CallbackType callBack)
		{
			_callBacks.push_back(callBack);
		}

		void SetOwner(Layer* owner)
		{
			if (!_hasOwnerBeenSet)
			{
				_owner = owner;
				_hasOwnerBeenSet = true;
			}
			else
			{
				TRCN_LOG("Owner of this event has been set already");
			}
		}

		void Submit(Layer* context, const EventType& event)
		{
			if (context != _owner) return;

			for (CallbackType& callBack : _callBacks)
			{
				callBack(event);
			}
		}

	private:
		std::vector<CallbackType> _callBacks;

		Layer* _owner = nullptr;
		bool _hasOwnerBeenSet = false;
	};
}