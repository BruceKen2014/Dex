

#ifndef _DEX_COMPILER_H
#define _DEX_COMPILER_H

#if defined(_WIN32) || defined(WIN32)
#define _DEX_PLATFORM_WIN32
#endif

#if defined(_WIN64) || defined(WIN64)
#define _DEX_PLATFORM_WIN64
#endif

#if defined(_DEX_PLATFORM_WIN32) || defined(_DEX_PLATFORM_WIN64) 
#define _DEX_PLATFORM_WINDOWS
#define _DEX_USE_DEVICE_DIRECTX9
#define _DEX_USE_DEVICE_OPENGL
#endif

#endif 