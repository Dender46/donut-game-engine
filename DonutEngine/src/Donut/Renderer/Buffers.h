#pragma once

namespace Donut {

	enum class ShaderDataType
	{
		None = 0, Int, Int2, Int3, Int4, Float, Float2, Float3, Float4, Mat3, Mat4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case Donut::ShaderDataType::Int:	return 4;
			case Donut::ShaderDataType::Int2:	return 4 * 2;
			case Donut::ShaderDataType::Int3:	return 4 * 3;
			case Donut::ShaderDataType::Int4:	return 4 * 4;
			case Donut::ShaderDataType::Float:	return 4;
			case Donut::ShaderDataType::Float2:	return 4 * 2;
			case Donut::ShaderDataType::Float3:	return 4 * 3;
			case Donut::ShaderDataType::Float4:	return 4 * 4;
			case Donut::ShaderDataType::Mat3:	return 4 * 3 * 3;
			case Donut::ShaderDataType::Mat4:	return 4 * 4 * 4;
			case Donut::ShaderDataType::Bool:	return 1;
		}

		DN_CORE_ASSERT(false, "Uknown ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Offset;
		uint32_t Size;
		bool Normalized;

		BufferElement() {}

		BufferElement(ShaderDataType type, const std::string name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case Donut::ShaderDataType::Int:	return 1;
				case Donut::ShaderDataType::Int2:	return 2;
				case Donut::ShaderDataType::Int3:	return 3;
				case Donut::ShaderDataType::Int4:	return 4;
				case Donut::ShaderDataType::Float:	return 1;
				case Donut::ShaderDataType::Float2:	return 2;
				case Donut::ShaderDataType::Float3:	return 3;
				case Donut::ShaderDataType::Float4:	return 4;
				case Donut::ShaderDataType::Mat3:	return 3 * 3;
				case Donut::ShaderDataType::Mat4:	return 4 * 4;
				case Donut::ShaderDataType::Bool:	return 1;
			}

			DN_CORE_ASSERT(false, "Uknown ShaderDataType!");
			return 0;
		} 
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		inline const std::vector<BufferElement> GetElements() { return m_Elements; }
		inline uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateOffsetAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& el : m_Elements)
			{
				el.Offset = offset;
				offset += el.Size;
				m_Stride += el.Size;
			}
		}

		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static VertexBuffer* Create(uint32_t size, float* vertices);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual inline uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t count, uint32_t* indices);
	};
}
