#pragma once
#include "Event.h"
#include <sstream>

class MouseEvent : public Event {
public:
	inline int GetButton() const { return m_Button; }

protected:
	MouseEvent(int button) :
		m_Button(button) {}

	int m_Button;
};

class MousePressedEvent : public MouseEvent {
public:
	MousePressedEvent(int button, int mods) :
		MouseEvent(button),
		m_Modifier(mods) {}

	inline int GetModifiers() const { return m_Modifier; }

	std::string to_str() const override {
		std::stringstream result;
		result << "MOUSE " << m_Button << " PRESSED ( " << m_Modifier << " )";
		return result.str();
	}

	EVENT_CLASS_TYPE(MOUSE_PRESSED);
private:
	int m_Modifier;
};

class MouseReleasedEvent : public MouseEvent {
public:
	MouseReleasedEvent(int button) :
		MouseEvent(button) {}

	std::string to_str() const override {
		std::stringstream result;
		result << "MOUSE " << m_Button << " RELEASED";
		return result.str();
	}

	EVENT_CLASS_TYPE(MOUSE_RELEASED);
};

class MouseScrolledEvent : public Event {
public:
	MouseScrolledEvent(double xOffset, double yOffset) :
		m_XOffset(xOffset),
		m_YOffset(yOffset) {}

	inline double GetXOffset() const { return m_XOffset; }
	inline double GetYOffset() const { return m_YOffset; }

	EVENT_CLASS_TYPE(MOUSE_SCROLLED);
private:
	double m_XOffset, m_YOffset;
};

class MouseMovedEvent : public Event {
public:
	MouseMovedEvent(double xpos, double ypos) :
		m_XPos(xpos),
		m_YPos(ypos) {}

	inline double GetXPos() const { return m_XPos; }
	inline double GetYPos() const { return m_YPos; }

	std::string to_str() const override {
		std::stringstream result;
		result << "MOUSE MOVED ( " << m_XPos << ", " << m_YPos << " )";
		return result.str();
	}

	EVENT_CLASS_TYPE(MOUSE_MOVED);
private:
	double m_XPos, m_YPos;
};