#pragma once
#include "Event.h"
#include <sstream>

class WindowResizeEvent : public Event {
public:
public:
	WindowResizeEvent(int width, int height) :
		m_Width(width),
		m_Height(height) {}

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

	std::string to_str() const override {
		std::stringstream result;
		result << "( " << m_Width << ", " << m_Height << " ) RESIZE";
		return result.str();
	}

	EVENT_CLASS_TYPE(WINDOW_RESIZE);
private:
	int m_Width, m_Height;
};

class WindowClosedEvent : public Event {
public:
	WindowClosedEvent() {}

	EVENT_CLASS_TYPE(WINDOW_CLOSED);
};

class WindowFocusedEvent : public Event {
public:
	WindowFocusedEvent() {}

	EVENT_CLASS_TYPE(WINDOW_FOCUSED);
};

class WindowUnFocusedEvent : public Event {
public:
	WindowUnFocusedEvent() {}

	EVENT_CLASS_TYPE(WINDOW_UNFOCUSED);
};
