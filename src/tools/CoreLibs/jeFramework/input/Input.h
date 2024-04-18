#pragma once

#include "platform/events/Event.h"
#include "platform/events/KeyEvent.h"
#include "platform/events/MouseEvent.h"

#include "platform/Window.h"

enum class KeyState
{
    NONE = 0,

    // Order is critical :3
    RELEASED,
    PRESSED,
    HELD,
};

#define INPUT_MAX_MOUSE_BUTTONS 16

class Input
{
public:
    static void set_cursor_lock_state(Window& window, CursorLockState state);

    static bool get_key_pressed(KeyCode key);
    static bool get_key_down(KeyCode key);
    static bool get_key_released(KeyCode key);

    static bool get_mouse_button_pressed(uint8_t button);
    static bool get_mouse_button_down(uint8_t button);
    static bool get_mouse_button_released(uint8_t button);

    static double get_mouse_move_horizontal();
    static double get_mouse_move_vertical();

    static CursorLockState get_cursor_lock_state();

    static void tick();

    static bool register_event(Event& event);

private:
    static KeyState& get_key_state(KeyCode code);

    static KeyState& get_mouse_key_state(uint8_t button);

    static bool register_key_press_event(KeyPressedEvent& event);

    static bool register_key_release_event(KeyReleasedEvent& event);

    static bool register_mouse_press_event(MousePressedEvent& event);

    static bool register_mouse_release_event(MouseReleasedEvent& event);

    static bool register_mouse_scroll_event(MouseScrolledEvent& event);

    static bool register_mouse_move_event(MouseMovedEvent& event);
private:
    std::array<KeyState, static_cast<size_t>(KeyCode::KEYCODE_COUNT)> m_keyStates{ };
    std::array<KeyState, INPUT_MAX_MOUSE_BUTTONS> m_mouseKeyStates{ };

    CursorLockState m_primaryCursorLockState{ CursorLockState::NONE };

    double m_currentMouseX{ 0.0 };
    double m_currentMouseY{ 0.0 };

    double m_previousMouseX{ 0.0 };
    double m_previousMouseY{ 0.0 };

private:
    Input();

    static Input* get_instance();

    static void shutdown();
private:
    inline static Input* s_instance = nullptr;

};