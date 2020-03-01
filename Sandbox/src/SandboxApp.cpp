#include <Donut.h>

class ExampleLayer : public Donut::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		if (Donut::Input::IsKeyPressed(DN_KEY_TAB))
			DN_TRACE("Tab key is pressed!");
	}

	void OnEvent(Donut::Event& e) override
	{
		DN_TRACE("{0}", e);
	}
};

class OpenGLExampleLayer : public Donut::Layer
{
public:
	OpenGLExampleLayer()
		: Layer("OpenGLExampleLayer"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		m_TriangleVA.reset(Donut::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f,  0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f,  0.0f, 0.2f, 0.2f, 0.8f, 1.0f,
			 0.0f,  0.5f,  0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Donut::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Donut::VertexBuffer::Create(sizeof(vertices), vertices));
		vertexBuffer->SetLayout({
			{ Donut::ShaderDataType::Float3, "a_Position" },
			{ Donut::ShaderDataType::Float4, "a_Color" },
			});
		m_TriangleVA->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Donut::IndexBuffer> indexBuffer;
		indexBuffer.reset(Donut::IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices));
		m_TriangleVA->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position  = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmantSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";
		m_RainbowShader.reset(new Donut::Shader(vertexSrc, fragmantSrc));

		m_SquareVA.reset(Donut::VertexArray::Create());
		float squareVertices[3 * 4] = {
			-0.75f, -0.75f,  0.0f,
			 0.75f, -0.75f,  0.0f,
			 0.75f,  0.75f,  0.0f,
			-0.75f,  0.75f,  0.0f,
		};
		std::shared_ptr<Donut::VertexBuffer> squareVertexBuffer;
		squareVertexBuffer.reset(Donut::VertexBuffer::Create(sizeof(squareVertices), squareVertices));
		squareVertexBuffer->SetLayout({
			{Donut::ShaderDataType::Float3, "a_Position"}
		});
		m_SquareVA->AddVertexBuffer(squareVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Donut::IndexBuffer> squareIndexBuffer;
		squareIndexBuffer.reset(Donut::IndexBuffer::Create(sizeof(squareIndices) / sizeof(uint32_t), squareIndices));
		m_SquareVA->SetIndexBuffer(squareIndexBuffer);

		std::string squareVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string squareFragmantSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2f, 0.2f, 0.8f, 1.0f);
			}
		)";
		m_BlueShader.reset(new Donut::Shader(squareVertexSrc, squareFragmantSrc));
	}

	void OnUpdate() override
	{
		Donut::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		Donut::RenderCommand::Clear();

		Donut::Renderer::BeginScene(m_Camera);

		CheckNewCameraPosition();
		Donut::Renderer::Submit(m_BlueShader, m_SquareVA);
		Donut::Renderer::Submit(m_RainbowShader, m_TriangleVA);

		Donut::Renderer::EndScene();
	}

	void CheckNewCameraPosition()
	{
		glm::vec3 newPosition = {
			m_Camera.GetPosition().x,
			m_Camera.GetPosition().y,
			m_Camera.GetPosition().z
		};

		if (Donut::Input::IsKeyPressed(DN_KEY_LEFT))
			newPosition.x += 0.01f;
		if (Donut::Input::IsKeyPressed(DN_KEY_RIGHT))
			newPosition.x -= 0.01f;
		if (Donut::Input::IsKeyPressed(DN_KEY_UP))
			newPosition.y -= 0.01f;
		if (Donut::Input::IsKeyPressed(DN_KEY_DOWN))
			newPosition.y += 0.01f;
		
		m_Camera.SetPosition(newPosition);
	}

	void OnEvent(Donut::Event& e) override
	{

	}

	private:
		Donut::OrthographicCamera m_Camera;

		std::shared_ptr<Donut::VertexArray> m_TriangleVA, m_SquareVA;
		std::shared_ptr<Donut::Shader> m_RainbowShader, m_BlueShader;
};

class Sandbox : public Donut::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushLayer(new OpenGLExampleLayer());
	}

	~Sandbox()
	{

	}
};

Donut::Application* Donut::CreateApplication()
{
	return new Sandbox();
}