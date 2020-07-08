#include <Donut.h>
#include <Donut/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Donut {

	class Editor : public Application
	{
	public:
		Editor()
			: Application("Donut Editor")
		{
			PushLayer(new EditorLayer());
		}
	};

	Application* CreateApplication()
	{
		return new Editor();
	}

}