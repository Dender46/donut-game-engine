#pragma once

#include "Donut\Layer.h"

#include "Donut\Events\KeyEvent.h"
#include "Donut\Events\MouseEvent.h"
#include "Donut\Events\ApplicationEvent.h"

namespace Donut {
	
	class  ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};

}