#pragma once
#include <memory>

#ifdef BLANCO_PLATFORM_WINDOWS
//define BL_API
#ifdef BLANCO_DYNAMIC_LIBRARY
  #ifdef BLANCO_BUILD_DLL
  #define BL_API  _declspec(dllexport)
  #else
  #define BL_API  _declspec(dllimport)
  #endif
#else
#define BL_API
#endif
#else
  #error Blanco Engine only for windows!
#endif

#ifdef BL_DEBUG
  #define BL_ASSERT_ENABLE
#endif

#ifdef BL_ASSERT_ENABLE
  #define BL_CORE_ASSERT(x,...) if(!(x)){BL_CORE_ERROR("Assert failed:{0}",__VA_ARGS__);__debugbreak();}
  #define BL_ASSERT(x,...) if(!(x)){BL_ERROR("Assert failed:{0}",__VA_ARGS__);__debugbreak();}
#else
  #define BL_CORE_ASSERT(x,...)
  #define BL_ASSERT(x,...)
#endif

#define BIT(x) 1<<x

#define BL_BIND_EVENT_FNC(x) std::bind(&x, this, std::placeholders::_1)

namespace Blanco 
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename... Args>
	constexpr Scope<T> CreateScope(Args&&... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename... Args>
	constexpr Ref<T> CreateRef(Args&&... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

