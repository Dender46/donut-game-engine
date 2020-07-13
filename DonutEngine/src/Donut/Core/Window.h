#pragma once

#include "dnpch.h"

#include "Donut/Core/Core.h"
#include "Donut/Events/Event.h"

namespace Donut {

	struct WindowProps
	{
		std::string Title = "Window Name";
		uint32_t Width = 1280;
		uint32_t Height = 720;
	};

	class  Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth()  const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};


}