#pragma once
#include <string>
#include <vector>
#include <functional>

enum class EventType{
	NONE = 0,
	KEY_PRESSED, KEY_RELEASED, KEY_TYPED,
	MOUSE_PRESSED, MOUSE_RELEASED, MOUSE_SCROLLED, MOUSE_MOVED,
	WINDOW_CLOSED, WINDOW_RESIZE, WINDOW_FOCUSED, WINDOW_UNFOCUSED
};

#define EVENT_CLASS_TYPE(type) static EventType get_static_type() { return EventType::##type; }\
							   virtual EventType get_type() const override { return get_static_type(); }\
							   virtual const char* name() const override { return #type; }

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

class Event {
	friend class EventDispatcher;
public:
	virtual EventType get_type() const = 0;
	virtual const char* name() const = 0;
	virtual std::string to_str() const { return name(); }

	inline bool handled() const { return m_handled; }
private:
	bool m_handled = false;
};

class EventDispatcher {
public:
	template<typename T>
	using EventFunc = std::function<bool(T&)>;

	EventDispatcher(Event& e) : m_event(e) {}

	template<typename T>
	void dispatch(EventFunc<T> callback) {
		if ( m_event.get_type() == T::get_static_type() ) {
            m_event.m_handled = callback(*(T*)&m_event); // Convert m_Event to specific EventType class
		}
	}
private:
	Event& m_event;
};
