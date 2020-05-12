#pragma once

#include "Donut\Renderer\Buffers.h"

namespace Donut {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;
		
		virtual void SetData(const void* data, uint32_t size);
		virtual void SetLayout(const BufferLayout& layout) { m_BufferLayout = layout; };
		virtual const BufferLayout& GetLayout() const { return m_BufferLayout; };
	private:
		uint32_t m_RendererID;
		BufferLayout m_BufferLayout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual inline uint32_t GetCount() const { return m_Count; }
	private:
		uint32_t m_RendererID, m_Count;
	};

}