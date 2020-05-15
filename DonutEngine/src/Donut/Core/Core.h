#pragma once

#ifdef DN_PLATFORM_WINDOWS

#else
	#error Donut supports only Windows for now!

#endif

#ifdef DN_DEBUG
	#define DN_ENABLE_ASSERTS
#endif

#ifdef DN_ENABLE_ASSERTS
	#define DN_ASSERT(x, ...) { if(!(x)) { DN_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define DN_CORE_ASSERT(x, ...) { if(!(x)) { DN_CORE_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define DN_ASSERT(x, ...)
	#define DN_CORE_ASSERT(x, ...)
#endif

#define DN_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define BIT(x) (1 << x)

#define DN_COLOR_WHITE	glm::vec4(1.0f)
#define DN_COLOR_BLACK	glm::vec4(0.0f)
#define DN_COLOR_RED	{0.90f, 0.10f, 0.10f, 1.0f}
#define DN_COLOR_GREEN	{0.10f, 0.90f, 0.10f, 1.0f}
#define DN_COLOR_BLUE	{0.10f, 0.10f, 0.90f, 1.0f}
#define DN_COLOR_PURPLE	{0.20f, 0.10f, 0.35f, 1.0f}

namespace Donut {

	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}


	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}