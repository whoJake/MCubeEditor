#pragma once

#include "platform/events/Event.h"
#include "platform/events/KeyEvent.h"
#include "platform/events/MouseEvent.h"

#include <unordered_map>
#include <array>

enum class KeyState
{
    NONE = 0,

    // Order is critical :3
    RELEASED,
    PRESSED,
    HELD,
};

class Input
{
public:
    static bool get_key_pressed(KeyCode key);
    static bool get_key_down(KeyCode key);
    static bool get_key_released(KeyCode key);

    static void tick();

    static bool register_event(Event& event);
private:
    static KeyState& get_key_state(KeyCode code);

    static bool register_key_press_event(KeyPressedEvent& event);

    static bool register_key_release_event(KeyReleasedEvent& event);

    static bool register_mouse_press_event(MousePressedEvent& event);

    static bool register_mouse_release_event(MouseReleasedEvent& event);

    static bool register_mouse_scroll_event(MouseScrolledEvent& event);

    static bool register_mouse_move_event(MouseMovedEvent& event);
private:
    std::array<KeyState, static_cast<size_t>(KeyCode::KEYCODE_COUNT)> m_keyStates;

private:
    Input();

    static Input* get_instance();

    static void shutdown();
private:
    inline static Input* s_instance = nullptr;

};