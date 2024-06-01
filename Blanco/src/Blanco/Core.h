#pragma once

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
  #define BL_CORE_ASSERT(x,...) if(!x){BL_CORE_ERROR("Assert failed:{0}",__VA_ARGS__);__debugbreak();}
  #define BL_ASSERT(x,...) if(!x){BL_ERROR("Assert failed:{0}",__VA_ARGS__);__debugbreak();}
#else
  #define BL_CORE_ASSERT(x,...)
  #define BL_ASSERT(x,...)
#endif

#define BIT(x) 1<<x

#define BL_BIND_EVENT_FNC(x) std::bind(&x, this, std::placeholders::_1)