#pragma once
#include <string>
#include <functional>
#include "logging/jclog.h"

class Window {
public:
	struct Extent {
		uint32_t width;
		uint32_t height;
	};

	enum class Mode {
		Default,
		Fullscreen,
		FullscreenBorderless,
		FullscreenStretched,
		Windowed
	};

	enum class VSync {
		Default,
		On,
		Off
	};

	struct Properties {
		std::string title = "";
		Mode mode = Mode::Windowed;
		bool resizable = false;
		VSync vsync = VSync::Off;
		Extent extent = { 1200, 900 };
		//std::function<void(Event&)> eventfn = [](Event& e) {};
	};

public:
	Window(jclog::Log& log, const Properties& properties);
	virtual ~Window() = default;

	virtual void* get_native_handle() const = 0;

	// virtual VkSurfaceKHR create_surface(vulkan::Instance& instance) = 0;

	// virtual std::vector<const char*> get_required_surface_extensions() const = 0;

	virtual bool get_should_close() const = 0;

	virtual void close() = 0;

	virtual void process_events() = 0;

	virtual void set_title(const std::string& title) = 0;

	virtual Extent resize(const Extent& extent);

	virtual void set_mode(const Mode& mode);

	const Extent& get_extent() const;

	const Mode& get_window_mode() const;

	const Properties& get_properties() const;
protected:
    jclog::Log& m_log;
	Properties m_properties;
};