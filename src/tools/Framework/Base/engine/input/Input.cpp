#include "Input.h"

void Input::set_cursor_lock_state(Window& window, CursorLockState state)
{
    if( get_instance()->m_primaryCursorLockState == state )
    {
        return;
    }
    get_instance()->m_primaryCursorLockState = state;
    window.set_cursor_lock_state(state);

    get_instance()->m_previousMouseX = window.poll_mouse_pos_x();
    get_instance()->m_previousMouseY = window.poll_mouse_pos_y();
}

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

double Input::get_mouse_move_horizontal()
{
    return get_instance()->m_previousMouseX - get_instance()->m_currentMouseX;
}

double Input::get_mouse_move_vertical()
{
    return get_instance()->m_previousMouseY - get_instance()->m_currentMouseY;
}

CursorLockState Input::get_cursor_lock_state()
{
    return get_instance()->m_primaryCursorLockState;
}

void Input::tick()
{
    get_instance()->m_previousMouseX = get_instance()->m_currentMouseX;
    get_instance()->m_previousMouseY = get_instance()->m_currentMouseY;

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
    get_instance()->m_currentMouseX = event.get_pos_x();
    get_instance()->m_currentMouseY = event.get_pos_y();
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