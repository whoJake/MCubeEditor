#include "Input.h"

bool Input::get_key_pressed(KeyCode key)
{
    return get_key_state(key) == KeyState::PRESSED;
}

bool Input::get_key_down(KeyCode key)
{
    return get_key_state(key) >= KeyState::PRESSED;
}

bool Input::get_key_released(KeyCode key)
{
    return get_key_state(key) == KeyState::RELEASED;
}

void Input::tick()
{
    for( KeyState& state : get_instance()->m_keyStates )
    {
        switch( state )
        {
        case KeyState::RELEASED:
            state = KeyState::NONE;
            break;
        case KeyState::PRESSED:
            state = KeyState::HELD;
            break;
        default:
            break;
        }
    }
}

bool Input::register_event(Event& event)
{
    EventDispatcher dispatcher(event);

    dispatcher.dispatch<KeyPressedEvent>(Input::register_key_press_event);
    dispatcher.dispatch<KeyReleasedEvent>(Input::register_key_release_event);
    dispatcher.dispatch<MousePressedEvent>(Input::register_mouse_press_event);
    dispatcher.dispatch<MouseReleasedEvent>(Input::register_mouse_release_event);
    dispatcher.dispatch<MouseScrolledEvent>(Input::register_mouse_scroll_event);
    dispatcher.dispatch<MouseMovedEvent>(Input::register_mouse_move_event);

    return false;
}

KeyState& Input::get_key_state(KeyCode code)
{
    return get_instance()->m_keyStates[static_cast<size_t>(code)];
}

bool Input::register_key_press_event(KeyPressedEvent& event)
{
    KeyState& currentState = get_key_state(event.get_key_code());
    if( currentState != KeyState::HELD )
    {
        currentState = KeyState::PRESSED;
    }

    return false;
}

bool Input::register_key_release_event(KeyReleasedEvent& event)
{
    get_key_state(event.get_key_code()) = KeyState::RELEASED;
    return false;
}

bool Input::register_mouse_press_event(MousePressedEvent& event)
{
    return false;
}

bool Input::register_mouse_release_event(MouseReleasedEvent& event)
{
    return false;
}

bool Input::register_mouse_scroll_event(MouseScrolledEvent& event)
{
    return false;
}

bool Input::register_mouse_move_event(MouseMovedEvent& event)
{
    return false;
}

Input::Input() :
    m_keyStates()
{ }

Input* Input::get_instance()
{
    if( !s_instance )
    {
        s_instance = new Input();
    }

    return s_instance;
}

void Input::shutdown()
{
    delete s_instance;
}