#pragma once

#include "dnpch.h"

#include "Donut/Core/Core.h"
#include "Donut/Events/Event.h"

namespace Donut {

	struct WindowProps
	{
		std::string Title = "Window Name";
		unsigned int Width = 1280;
		unsigned int Height = 720;
	};

	class  Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth()  const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};


}