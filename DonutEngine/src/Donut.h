#pragma once

// ----- Renderer -----
#include "Donut/Core/Application.h"
#include "Donut/Core/Layer.h"
#include "Donut/Core/Log.h"
#include "Donut/ImGui/ImGuiLayer.h"

#include "Donut/Core/Timestep.h"
#include "Donut/Core/Random.h"

#include "Donut/Core/MouseButtonCode.h"
#include "Donut/Core/KeyCodes.h"
#include "Donut/Core/Input.h"

// ----- Renderer -----
#include "Donut/Renderer/Renderer.h"
#include "Donut/Renderer/Renderer2D.h"
#include "Donut/Renderer/RenderCommand.h"

#include "Donut/Renderer/Buffers.h"
#include "Donut/Renderer/Shader.h"
#include "Donut/Renderer/VertexArray.h"
#include "Donut/Renderer/Framebuffer.h"

#include "Donut/Renderer/Texture.h"
#include "Donut/Renderer/SubTexture2D.h"

// ----- Cameras -----
#include "Donut/Renderer/Camera.h"
#include "Donut/Renderer/OrthographicCameraController.h"

// ----- Physics -----
#include "Donut/Physics/World.h"
#include "Donut/Physics/Box.h"

// ----- Systems -----
#include "Donut/Systems/Font.h"

// ----- ECS -----
#include "Donut/ECS/ECS.h"
#include "Donut/ECS/Components/ECSComponents.h"
#include "Donut/ECS/Systems/ECSSystems.h"
#include "Donut/ECS/Scene.h"