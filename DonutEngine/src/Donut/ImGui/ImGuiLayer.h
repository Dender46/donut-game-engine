#pragma once

#include "Donut/Core/Layer.h"

namespace Donut {
	
	class  ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		inline void BlockEvents(bool val) { m_BlockEvents = val; }

		void Begin();
		void End();
	private:
		bool m_BlockEvents = true;
	};

}