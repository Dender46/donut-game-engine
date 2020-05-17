#pragma once

#include <chrono>
#include <random>

namespace Donut {

	class Random
	{
	public:
		static void Init()
		{
			s_RandomEngine.seed((uint32_t)std::chrono::steady_clock::now().time_since_epoch().count());
		}

		static float Float()
		{
			return (float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max();
		}
	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};

}
