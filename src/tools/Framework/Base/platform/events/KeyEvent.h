#pragma once
#include "Event.h"
#include <sstream>
#include "GLFW/glfw3.h"

enum class KeyCode {
	Unknown,
	Space,
	Apostrophe, /* ' */
	Comma,      /* , */
	Minus,      /* - */
	Period,     /* . */
	Slash,      /* / */
	_0,
	_1,
	_2,
	_3,
	_4,
	_5,
	_6,
	_7,
	_8,
	_9,
	Semicolon, /* ; */
	Equal,     /* = */
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	LeftBracket,  /* [ */
	Backslash,    /* \ */
	RightBracket, /* ] */
	GraveAccent,  /* ` */
	Escape,
	Enter,
	Tab,
	Backspace,
	LeftShift,
	LeftControl,
	LeftAlt,
	RightShift,
	RightControl,
	RightAlt,
	Insert,
	DelKey,
	Right,
	Left,
	Down,
	Up,
	PageUp,
	PageDown,
	Home,
	End,
	Back,
	CapsLock,
	ScrollLock,
	NumLock,
	PrintScreen,
	Pause,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
	KP_0,
	KP_1,
	KP_2,
	KP_3,
	KP_4,
	KP_5,
	KP_6,
	KP_7,
	KP_8,
	KP_9,
	KP_Decimal,
	KP_Divide,
	KP_Multiply,
	KP_Subtract,
	KP_Add,
	KP_Enter,
	KP_Equal
};

inline KeyCode TranslateKeyCode(int key) {
	static const std::unordered_map<int, KeyCode> lookup = {
		{GLFW_KEY_SPACE, KeyCode::Space},
		{GLFW_KEY_APOSTROPHE, KeyCode::Apostrophe},
		{GLFW_KEY_COMMA, KeyCode::Comma},
		{GLFW_KEY_MINUS, KeyCode::Minus},
		{GLFW_KEY_PERIOD, KeyCode::Period},
		{GLFW_KEY_SLASH, KeyCode::Slash},
		{GLFW_KEY_0, KeyCode::_0},
		{GLFW_KEY_1, KeyCode::_1},
		{GLFW_KEY_2, KeyCode::_2},
		{GLFW_KEY_3, KeyCode::_3},
		{GLFW_KEY_4, KeyCode::_4},
		{GLFW_KEY_5, KeyCode::_5},
		{GLFW_KEY_6, KeyCode::_6},
		{GLFW_KEY_7, KeyCode::_7},
		{GLFW_KEY_8, KeyCode::_8},
		{GLFW_KEY_9, KeyCode::_9},
		{GLFW_KEY_SEMICOLON, KeyCode::Semicolon},
		{GLFW_KEY_EQUAL, KeyCode::Equal},
		{GLFW_KEY_A, KeyCode::A},
		{GLFW_KEY_B, KeyCode::B},
		{GLFW_KEY_C, KeyCode::C},
		{GLFW_KEY_D, KeyCode::D},
		{GLFW_KEY_E, KeyCode::E},
		{GLFW_KEY_F, KeyCode::F},
		{GLFW_KEY_G, KeyCode::G},
		{GLFW_KEY_H, KeyCode::H},
		{GLFW_KEY_I, KeyCode::I},
		{GLFW_KEY_J, KeyCode::J},
		{GLFW_KEY_K, KeyCode::K},
		{GLFW_KEY_L, KeyCode::L},
		{GLFW_KEY_M, KeyCode::M},
		{GLFW_KEY_N, KeyCode::N},
		{GLFW_KEY_O, KeyCode::O},
		{GLFW_KEY_P, KeyCode::P},
		{GLFW_KEY_Q, KeyCode::Q},
		{GLFW_KEY_R, KeyCode::R},
		{GLFW_KEY_S, KeyCode::S},
		{GLFW_KEY_T, KeyCode::T},
		{GLFW_KEY_U, KeyCode::U},
		{GLFW_KEY_V, KeyCode::V},
		{GLFW_KEY_W, KeyCode::W},
		{GLFW_KEY_X, KeyCode::X},
		{GLFW_KEY_Y, KeyCode::Y},
		{GLFW_KEY_Z, KeyCode::Z},
		{GLFW_KEY_LEFT_BRACKET, KeyCode::LeftBracket},
		{GLFW_KEY_BACKSLASH, KeyCode::Backslash},
		{GLFW_KEY_RIGHT_BRACKET, KeyCode::RightBracket},
		{GLFW_KEY_GRAVE_ACCENT, KeyCode::GraveAccent},
		{GLFW_KEY_ESCAPE, KeyCode::Escape},
		{GLFW_KEY_ENTER, KeyCode::Enter},
		{GLFW_KEY_TAB, KeyCode::Tab},
		{GLFW_KEY_BACKSPACE, KeyCode::Backspace},
		{GLFW_KEY_INSERT, KeyCode::Insert},
		{GLFW_KEY_DELETE, KeyCode::DelKey},
		{GLFW_KEY_RIGHT, KeyCode::Right},
		{GLFW_KEY_LEFT, KeyCode::Left},
		{GLFW_KEY_DOWN, KeyCode::Down},
		{GLFW_KEY_UP, KeyCode::Up},
		{GLFW_KEY_PAGE_UP, KeyCode::PageUp},
		{GLFW_KEY_PAGE_DOWN, KeyCode::PageDown},
		{GLFW_KEY_HOME, KeyCode::Home},
		{GLFW_KEY_END, KeyCode::End},
		{GLFW_KEY_CAPS_LOCK, KeyCode::CapsLock},
		{GLFW_KEY_SCROLL_LOCK, KeyCode::ScrollLock},
		{GLFW_KEY_NUM_LOCK, KeyCode::NumLock},
		{GLFW_KEY_PRINT_SCREEN, KeyCode::PrintScreen},
		{GLFW_KEY_PAUSE, KeyCode::Pause},
		{GLFW_KEY_F1, KeyCode::F1},
		{GLFW_KEY_F2, KeyCode::F2},
		{GLFW_KEY_F3, KeyCode::F3},
		{GLFW_KEY_F4, KeyCode::F4},
		{GLFW_KEY_F5, KeyCode::F5},
		{GLFW_KEY_F6, KeyCode::F6},
		{GLFW_KEY_F7, KeyCode::F7},
		{GLFW_KEY_F8, KeyCode::F8},
		{GLFW_KEY_F9, KeyCode::F9},
		{GLFW_KEY_F10, KeyCode::F10},
		{GLFW_KEY_F11, KeyCode::F11},
		{GLFW_KEY_F12, KeyCode::F12},
		{GLFW_KEY_KP_0, KeyCode::KP_0},
		{GLFW_KEY_KP_1, KeyCode::KP_1},
		{GLFW_KEY_KP_2, KeyCode::KP_2},
		{GLFW_KEY_KP_3, KeyCode::KP_3},
		{GLFW_KEY_KP_4, KeyCode::KP_4},
		{GLFW_KEY_KP_5, KeyCode::KP_5},
		{GLFW_KEY_KP_6, KeyCode::KP_6},
		{GLFW_KEY_KP_7, KeyCode::KP_7},
		{GLFW_KEY_KP_8, KeyCode::KP_8},
		{GLFW_KEY_KP_9, KeyCode::KP_9},
		{GLFW_KEY_KP_DECIMAL, KeyCode::KP_Decimal},
		{GLFW_KEY_KP_DIVIDE, KeyCode::KP_Divide},
		{GLFW_KEY_KP_MULTIPLY, KeyCode::KP_Multiply},
		{GLFW_KEY_KP_SUBTRACT, KeyCode::KP_Subtract},
		{GLFW_KEY_KP_ADD, KeyCode::KP_Add},
		{GLFW_KEY_KP_ENTER, KeyCode::KP_Enter},
		{GLFW_KEY_KP_EQUAL, KeyCode::KP_Equal},
		{GLFW_KEY_LEFT_SHIFT, KeyCode::LeftShift},
		{GLFW_KEY_LEFT_CONTROL, KeyCode::LeftControl},
		{GLFW_KEY_LEFT_ALT, KeyCode::LeftAlt},
		{GLFW_KEY_RIGHT_SHIFT, KeyCode::RightShift},
		{GLFW_KEY_RIGHT_CONTROL, KeyCode::RightControl},
		{GLFW_KEY_RIGHT_ALT, KeyCode::RightAlt},
	};

	auto it = lookup.find(key);
	if (it == lookup.end())
		return KeyCode::Unknown;

	return it->second;
}

inline std::string KeyCodeName(KeyCode keycode) {
	static const std::unordered_map<KeyCode, std::string> lookup = {
		{KeyCode::Space,		"KEY_SPACE" },
		{KeyCode::Apostrophe, 	"KEY_APOSTROPHE" },
		{KeyCode::Comma, 		"KEY_COMMA" },
		{KeyCode::Minus, 		"KEY_MINUS" },
		{KeyCode::Period, 		"KEY_PERIOD" },
		{KeyCode::Slash,		"KEY_SLASH" },
		{KeyCode::_0, 			"KEY_0" },
		{KeyCode::_1, 			"KEY_1" },
		{KeyCode::_2, 			"KEY_2" },
		{KeyCode::_3, 			"KEY_3" },
		{KeyCode::_4, 			"KEY_4" },
		{KeyCode::_5, 			"KEY_5" },
		{KeyCode::_6, 			"KEY_6" },
		{KeyCode::_7, 			"KEY_7" },
		{KeyCode::_8, 			"KEY_8" },
		{KeyCode::_9, 			"KEY_9" },
		{KeyCode::Semicolon, 	"KEY_SEMICOLON" },
		{KeyCode::Equal, 		"KEY_EQUAL" },
		{KeyCode::A, 			"KEY_A" },
		{KeyCode::B, 			"KEY_B" },
		{KeyCode::C, 			"KEY_C" },
		{KeyCode::D, 			"KEY_D" },
		{KeyCode::E, 			"KEY_E" },
		{KeyCode::F, 			"KEY_F" },
		{KeyCode::G, 			"KEY_G" },
		{KeyCode::H, 			"KEY_H" },
		{KeyCode::I, 			"KEY_I" },
		{KeyCode::J, 			"KEY_J" },
		{KeyCode::K, 			"KEY_K" },
		{KeyCode::L, 			"KEY_L" },
		{KeyCode::M, 			"KEY_M" },
		{KeyCode::N, 			"KEY_N" },
		{KeyCode::O, 			"KEY_O" },
		{KeyCode::P, 			"KEY_P" },
		{KeyCode::Q, 			"KEY_Q" },
		{KeyCode::R, 			"KEY_R" },
		{KeyCode::S, 			"KEY_S" },
		{KeyCode::T, 			"KEY_T" },
		{KeyCode::U, 			"KEY_U" },
		{KeyCode::V, 			"KEY_V" },
		{KeyCode::W, 			"KEY_W" },
		{KeyCode::X, 			"KEY_X" },
		{KeyCode::Y, 			"KEY_Y" },
		{KeyCode::Z, 			"KEY_Z" },
		{KeyCode::LeftBracket, 	"KEY_LEFT_BRACKET" },
		{KeyCode::Backslash, 	"KEY_BACKSLASH" },
		{KeyCode::RightBracket, "KEY_RIGHT_BRACKET" },
		{KeyCode::GraveAccent, 	"KEY_GRAVE_ACCENT" },
		{KeyCode::Escape, 		"KEY_ESCAPE" },
		{KeyCode::Enter, 		"KEY_ENTER" },
		{KeyCode::Tab, 			"KEY_TAB" },
		{KeyCode::Backspace, 	"KEY_BACKSPACE" },
		{KeyCode::Insert, 		"KEY_INSERT" },
		{KeyCode::DelKey, 		"KEY_DELETE" },
		{KeyCode::Right, 		"KEY_RIGHT" },
		{KeyCode::Left, 		"KEY_LEFT" },
		{KeyCode::Down, 		"KEY_DOWN" },
		{KeyCode::Up, 			"KEY_UP" },
		{KeyCode::PageUp, 		"KEY_PAGE_UP" },
		{KeyCode::PageDown, 	"KEY_PAGE_DOWN" },
		{KeyCode::Home, 		"KEY_HOME" },
		{KeyCode::End, 			"KEY_END" },
		{KeyCode::CapsLock, 	"KEY_CAPS_LOCK" },
		{KeyCode::ScrollLock, 	"KEY_SCROLL_LOCK" },
		{KeyCode::NumLock, 		"KEY_NUM_LOCK" },
		{KeyCode::PrintScreen, 	"KEY_PRINT_SCREEN" },
		{KeyCode::Pause, 		"KEY_PAUSE" },
		{KeyCode::F1, 			"KEY_F1"},
		{KeyCode::F2, 			"KEY_F2"},
		{KeyCode::F3, 			"KEY_F3"},
		{KeyCode::F4, 			"KEY_F4"},
		{KeyCode::F5, 			"KEY_F5"},
		{KeyCode::F6, 			"KEY_F6"},
		{KeyCode::F7, 			"KEY_F7"},
		{KeyCode::F8, 			"KEY_F8"},
		{KeyCode::F9, 			"KEY_F9"},
		{KeyCode::F10, 			"KEY_F10" },
		{KeyCode::F11, 			"KEY_F11" },
		{KeyCode::F12, 			"KEY_F12" },
		{KeyCode::KP_0, 		"KEY_KP_0" },
		{KeyCode::KP_1, 		"KEY_KP_1" },
		{KeyCode::KP_2, 		"KEY_KP_2" },
		{KeyCode::KP_3, 		"KEY_KP_3"},
		{KeyCode::KP_4, 		"KEY_KP_4" },
		{KeyCode::KP_5, 		"KEY_KP_5"},
		{KeyCode::KP_6, 		"KEY_KP_6" },
		{KeyCode::KP_7, 		"KEY_KP_7" },
		{KeyCode::KP_8, 		"KEY_KP_8"},
		{KeyCode::KP_9, 		"KEY_KP_9" },
		{KeyCode::KP_Decimal, 	"KEY_KP_DECIMAL" },
		{KeyCode::KP_Divide, 	"KEY_KP_DIVIDE" },
		{KeyCode::KP_Multiply, 	"KEY_KP_MULTIPLY" },
		{KeyCode::KP_Subtract, 	"KEY_KP_SUBTRACT" },
		{KeyCode::KP_Add, 		"KEY_KP_ADD" },
		{KeyCode::KP_Enter, 	"KEY_KP_ENTER" },
		{KeyCode::KP_Equal, 	"KEY_KP_EQUAL" },
		{KeyCode::LeftShift, 	"KEY_LEFT_SHIFT" },
		{KeyCode::LeftControl, 	"KEY_LEFT_CONTROL" },
		{KeyCode::LeftAlt, 		"KEY_LEFT_ALT" },
		{KeyCode::RightShift, 	"KEY_RIGHT_SHIFT" },
		{KeyCode::RightControl, "KEY_RIGHT_CONTROL" },
		{KeyCode::RightAlt, 	"KEY_RIGHT_ALT" }
	};

	auto it = lookup.find(keycode);
	if (it == lookup.end())
		return "KEY_UNKNOWN";
	return it->second;
}

class KeyEvent : public Event {
public:
	inline KeyCode GetKeyCode() const { return m_KeyCode; }
protected:
	KeyEvent(int keycode) : 
		m_KeyCode(TranslateKeyCode(keycode)) 
	{ }
	KeyCode m_KeyCode;
};

class KeyPressedEvent : public KeyEvent {
public:
	KeyPressedEvent(int keycode, int repeatCount) :
		KeyEvent(keycode),
		m_RepeatCount(repeatCount)
	{ }

	inline int GetRepeatCount() const { return m_RepeatCount; }

	std::string to_str() const override {
		std::stringstream result;
		result << KeyCodeName(m_KeyCode) << " x" << m_RepeatCount << " PRESSED";
		return result.str();
	}

	EVENT_CLASS_TYPE(KEY_PRESSED);
private:
	int m_RepeatCount;
};

class KeyReleasedEvent : public KeyEvent {
public:
	KeyReleasedEvent(int keycode) :
		KeyEvent(keycode)
	{ }

	EVENT_CLASS_TYPE(KEY_RELEASED);

	std::string to_str() const override {
		std::stringstream result;
		result << KeyCodeName(m_KeyCode) << " RELEASED";
		return result.str();
	}
};

class KeyTypedEvent : public KeyEvent {
public:
	KeyTypedEvent(int keycode) :
		KeyEvent(keycode)
	{ }

	std::string to_str() const override {
		std::stringstream result;
		result << KeyCodeName(m_KeyCode) << " TYPED";
		return result.str();
	}

	EVENT_CLASS_TYPE(KEY_TYPED);
};
