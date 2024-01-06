#include "Window.h"

Window::Window(jclog::Log& log, const Properties& properties) :
    m_log(log),
	m_properties(properties)
{ }

Window::Extent Window::resize(const Extent& extent) {
	if (m_properties.resizable) {
		m_properties.extent.width = extent.width;
		m_properties.extent.height = extent.height;
	}

	return m_properties.extent;
}

void Window::set_mode(const Mode& mode) {
	m_properties.mode = mode;
}

const Window::Extent& Window::get_extent() const {
	return m_properties.extent;
}

const Window::Mode& Window::get_window_mode() const {
	return m_properties.mode;
}

const Window::Properties& Window::get_properties() const {
	return m_properties;
}