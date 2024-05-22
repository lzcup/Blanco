#pragma once

#ifdef BLANCO_PLATFORM_WINDOWS
//define BL_API
  #ifdef BLANCO_BUILD_DLL
  #define  BL_API  _declspec(dllexport)
  #else
  #define  BL_API  _declspec(dllimport)
  #endif
#else
  #error Blanco Engine only for windows!
#endif
