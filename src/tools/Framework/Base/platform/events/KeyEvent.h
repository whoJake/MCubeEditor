#pragma once
#include "Event.h"
#include <sstream>
#include "glfw3.h"

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
        {KeyCode::Space,        "EVENT_KEY_SPACE" },
        {KeyCode::Apostrophe,   "EVENT_KEY_APOSTROPHE" },
        {KeyCode::Comma,        "EVENT_KEY_COMMA" },
        {KeyCode::Minus,        "EVENT_KEY_MINUS" },
        {KeyCode::Period,       "EVENT_KEY_PERIOD" },
        {KeyCode::Slash,        "EVENT_KEY_SLASH" },
        {KeyCode::_0,           "EVENT_KEY_0" },
        {KeyCode::_1,           "EVENT_KEY_1" },
        {KeyCode::_2,           "EVENT_KEY_2" },
        {KeyCode::_3,           "EVENT_KEY_3" },
        {KeyCode::_4,           "EVENT_KEY_4" },
        {KeyCode::_5,           "EVENT_KEY_5" },
        {KeyCode::_6,           "EVENT_KEY_6" },
        {KeyCode::_7,           "EVENT_KEY_7" },
        {KeyCode::_8,           "EVENT_KEY_8" },
        {KeyCode::_9,           "EVENT_KEY_9" },
        {KeyCode::Semicolon,    "EVENT_KEY_SEMICOLON" },
        {KeyCode::Equal,        "EVENT_KEY_EQUAL" },
        {KeyCode::A,            "EVENT_KEY_A" },
        {KeyCode::B,            "EVENT_KEY_B" },
        {KeyCode::C,            "EVENT_KEY_C" },
        {KeyCode::D,            "EVENT_KEY_D" },
        {KeyCode::E,            "EVENT_KEY_E" },
        {KeyCode::F,            "EVENT_KEY_F" },
        {KeyCode::G,            "EVENT_KEY_G" },
        {KeyCode::H,            "EVENT_KEY_H" },
        {KeyCode::I,            "EVENT_KEY_I" },
        {KeyCode::J,            "EVENT_KEY_J" },
        {KeyCode::K,            "EVENT_KEY_K" },
        {KeyCode::L,            "EVENT_KEY_L" },
        {KeyCode::M,            "EVENT_KEY_M" },
        {KeyCode::N,            "EVENT_KEY_N" },
        {KeyCode::O,            "EVENT_KEY_O" },
        {KeyCode::P,            "EVENT_KEY_P" },
        {KeyCode::Q,            "EVENT_KEY_Q" },
        {KeyCode::R,            "EVENT_KEY_R" },
        {KeyCode::S,            "EVENT_KEY_S" },
        {KeyCode::T,            "EVENT_KEY_T" },
        {KeyCode::U,            "EVENT_KEY_U" },
        {KeyCode::V,            "EVENT_KEY_V" },
        {KeyCode::W,            "EVENT_KEY_W" },
        {KeyCode::X,            "EVENT_KEY_X" },
        {KeyCode::Y,            "EVENT_KEY_Y" },
        {KeyCode::Z,            "EVENT_KEY_Z" },
        {KeyCode::LeftBracket,  "EVENT_KEY_LEFT_BRACKET" },
        {KeyCode::Backslash,    "EVENT_KEY_BACKSLASH" },
        {KeyCode::RightBracket, "EVENT_KEY_RIGHT_BRACKET" },
        {KeyCode::GraveAccent,  "EVENT_KEY_GRAVE_ACCENT" },
        {KeyCode::Escape,       "EVENT_KEY_ESCAPE" },
        {KeyCode::Enter,        "EVENT_KEY_ENTER" },
        {KeyCode::Tab,          "EVENT_KEY_TAB" },
        {KeyCode::Backspace,    "EVENT_KEY_BACKSPACE" },
        {KeyCode::Insert,       "EVENT_KEY_INSERT" },
        {KeyCode::DelKey,       "EVENT_KEY_DELETE" },
        {KeyCode::Right,        "EVENT_KEY_RIGHT" },
        {KeyCode::Left,         "EVENT_KEY_LEFT" },
        {KeyCode::Down,         "EVENT_KEY_DOWN" },
        {KeyCode::Up,           "EVENT_KEY_UP" },
        {KeyCode::PageUp,       "EVENT_KEY_PAGE_UP" },
        {KeyCode::PageDown,     "EVENT_KEY_PAGE_DOWN" },
        {KeyCode::Home,         "EVENT_KEY_HOME" },
        {KeyCode::End,          "EVENT_KEY_END" },
        {KeyCode::CapsLock,     "EVENT_KEY_CAPS_LOCK" },
        {KeyCode::ScrollLock,   "EVENT_KEY_SCROLL_LOCK" },
        {KeyCode::NumLock,      "EVENT_KEY_NUM_LOCK" },
        {KeyCode::PrintScreen,  "EVENT_KEY_PRINT_SCREEN" },
        {KeyCode::Pause,        "EVENT_KEY_PAUSE" },
        {KeyCode::F1,           "EVENT_KEY_F1"},
        {KeyCode::F2,           "EVENT_KEY_F2"},
        {KeyCode::F3,           "EVENT_KEY_F3"},
        {KeyCode::F4,           "EVENT_KEY_F4"},
        {KeyCode::F5,           "EVENT_KEY_F5"},
        {KeyCode::F6,           "EVENT_KEY_F6"},
        {KeyCode::F7,           "EVENT_KEY_F7"},
        {KeyCode::F8,           "EVENT_KEY_F8"},
        {KeyCode::F9,           "EVENT_KEY_F9"},
        {KeyCode::F10,          "EVENT_KEY_F10" },
        {KeyCode::F11,          "EVENT_KEY_F11" },
        {KeyCode::F12,          "EVENT_KEY_F12" },
        {KeyCode::KP_0,         "EVENT_KEY_KP_0" },
        {KeyCode::KP_1,         "EVENT_KEY_KP_1" },
        {KeyCode::KP_2,         "EVENT_KEY_KP_2" },
        {KeyCode::KP_3,         "EVENT_KEY_KP_3"},
        {KeyCode::KP_4,         "EVENT_KEY_KP_4" },
        {KeyCode::KP_5,         "EVENT_KEY_KP_5"},
        {KeyCode::KP_6,         "EVENT_KEY_KP_6" },
        {KeyCode::KP_7,         "EVENT_KEY_KP_7" },
        {KeyCode::KP_8,         "EVENT_KEY_KP_8"},
        {KeyCode::KP_9,         "EVENT_KEY_KP_9" },
        {KeyCode::KP_Decimal,   "EVENT_KEY_KP_DECIMAL" },
        {KeyCode::KP_Divide,    "EVENT_KEY_KP_DIVIDE" },
        {KeyCode::KP_Multiply,  "EVENT_KEY_KP_MULTIPLY" },
        {KeyCode::KP_Subtract,  "EVENT_KEY_KP_SUBTRACT" },
        {KeyCode::KP_Add,       "EVENT_KEY_KP_ADD" },
        {KeyCode::KP_Enter,     "EVENT_KEY_KP_ENTER" },
        {KeyCode::KP_Equal,     "EVENT_KEY_KP_EQUAL" },
        {KeyCode::LeftShift,    "EVENT_KEY_LEFT_SHIFT" },
        {KeyCode::LeftControl,  "EVENT_KEY_LEFT_CONTROL" },
        {KeyCode::LeftAlt,      "EVENT_KEY_LEFT_ALT" },
        {KeyCode::RightShift,   "EVENT_KEY_RIGHT_SHIFT" },
        {KeyCode::RightControl, "EVENT_KEY_RIGHT_CONTROL" },
        {KeyCode::RightAlt,     "EVENT_KEY_RIGHT_ALT" }
    };

    auto it = lookup.find(keycode);
    if (it == lookup.end())
        return "EVENT_KEY_UNKNOWN";
    return it->second;
}

class KeyEvent : public Event {
public:
    inline KeyCode GetKeyCode() const { return m_keyCode; }
protected:
    KeyEvent(int keycode) : 
        m_keyCode(TranslateKeyCode(keycode)) 
    { }
    KeyCode m_keyCode;
};

class KeyPressedEvent : public KeyEvent {
public:
    KeyPressedEvent(int keycode, int repeatCount) :
        KeyEvent(keycode),
        m_repeatCount(repeatCount)
    { }

    inline int GetRepeatCount() const { return m_repeatCount; }

    std::string to_str() const override {
        std::stringstream result;
        result << KeyCodeName(m_keyCode) << " x" << m_repeatCount << " PRESSED";
        return result.str();
    }

    EVENT_CLASS_TYPE(EVENT_KEY_PRESSED);
private:
    int m_repeatCount;
};

class KeyReleasedEvent : public KeyEvent {
public:
    KeyReleasedEvent(int keycode) :
        KeyEvent(keycode)
    { }

    EVENT_CLASS_TYPE(EVENT_KEY_RELEASED);

    std::string to_str() const override {
        std::stringstream result;
        result << KeyCodeName(m_keyCode) << " RELEASED";
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
        result << KeyCodeName(m_keyCode) << " TYPED";
        return result.str();
    }

    EVENT_CLASS_TYPE(EVENT_KEY_TYPED);
};
