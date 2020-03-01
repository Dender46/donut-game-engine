#pragma once

#include "Donut/Timestep.h"
#include "Donut/Core.h"
#include "Donut/Events/Event.h"

namespace Donut {

	class  Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}

		inline const std::string& GetName() { return m_DebugName; }
	private:
		std::string m_DebugName;
	};

}
