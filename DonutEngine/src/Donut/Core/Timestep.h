#pragma once

namespace Donut {

	class Timestep
	{
	public:
		Timestep(float time) 
			: m_Time(time)
		{}

		operator float() { return m_Time; }

		const float GetSeconds() const { return m_Time; }
		const float GetMiliseconds() const { return m_Time * 1000.0f; }
	private:
		float m_Time;
	};

}
