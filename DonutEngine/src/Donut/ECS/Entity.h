#pragma once

namespace Donut {

	class Entity
	{
	public:
		Entity(const std::string name = "Entity");
		
		template <typename T>
		void AddComponent(T component);

		operator uint32_t& () { return m_ID; }
		operator const uint32_t& () const { return m_ID; }
	private:
		uint32_t m_ID;
		uint32_t m_SceneID;
		std::string m_Name;

		friend class Scene;
	};

	

}
