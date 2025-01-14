#pragma once

#include "Event.h"
#include "Input/InputCodes.h"

namespace Behemoth
{
	using namespace Events;

	class KeyboardEvent : public Event
	{
	public:
		KeyboardEvent(KeyCode keyCode) :keyCode(keyCode) {}

		virtual EventType GetEventType() = 0;
		virtual uint16_t GetEventFlags() = 0;

		const KeyCode GetKeyCode() const { return keyCode; }

	private:
		KeyCode keyCode;
	};


	class KeyDownEvent : public KeyboardEvent
	{
	public:
		KeyDownEvent(KeyCode keyCode) : KeyboardEvent(keyCode) {}
		virtual EventType GetEventType()   override { return EventType::KeyPressed; }
		virtual uint16_t GetEventFlags()   override { return EventFlags::Input | EventFlags::Keyboard; }
	};

	class KeyReleasedEvent : public KeyboardEvent
	{
	public:
		KeyReleasedEvent(KeyCode keyCode) : KeyboardEvent(keyCode) {}

		virtual EventType GetEventType()   override { return EventType::KeyReleased; }
		virtual uint16_t GetEventFlags()   override { return EventFlags::Input | EventFlags::Keyboard; }
	};
}