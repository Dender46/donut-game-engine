#pragma once

#include "Donut\Core.h"
#include "Donut\Events\Event.h"

namespace Donut {

	class DONUT_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach();
		virtual void OnDetach();
		virtual void OnUpdate();
		virtual void OnEvent(Event& e);

		inline const std::string& GetName() { return m_DebugName; }
	private:
		std::string m_DebugName;
	};

}
