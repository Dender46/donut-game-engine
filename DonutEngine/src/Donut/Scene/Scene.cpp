#include "dnpch.h"
#include "Donut/Scene/Scene.h"
#include "Donut/Scene/Components.h"
#include "Donut/Scene/Entity.h"

#include "Donut/Renderer/Renderer2D.h"

namespace Donut {

	void Scene::AddSystem(BaseECSSystem& system)
	{
		bool isSystemValid = m_SystemList.AddSystem(system);
		DN_CORE_ASSERT(isSystemValid, "System is not valid!");
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity newEntity = { this, m_ECS.MakeEntity() };
		newEntity.AddComponent(NameComponent(name.empty() ? "Unnamed Entity" : name));
		newEntity.AddComponent(TransformComponent());
		return newEntity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		// Rendering part
		Camera* primaryCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		auto cameraComponents = m_ECS.GetComponentsOfType<CameraComponent>();

		// Get primary camera that should be taken for drawing scene
		for (auto camera : cameraComponents)
			if (camera->IsPrimary)
			{
				primaryCamera = &camera->Camera;
				cameraTransform = &m_ECS.GetComponent<TransformComponent>(camera->Entity)->Transform;
				break;
			}

		if (primaryCamera)
		{
			Renderer2D::BeginScene(primaryCamera->GetProjection(), *cameraTransform, false);

			m_ECS.UpdateSystems(m_SystemList, ts);

			Renderer2D::EndScene();
		}
	}

}