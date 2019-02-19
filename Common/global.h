#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include<windows.h>

/*
redefinition of all basic types
*/

//! 8 bit unsigned variable.
/** This is a typedef for unsigned char, it ensures portability of the engine. */
#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef unsigned __int8		u8;
#else
typedef unsigned char		u8;
#endif

//! 8 bit signed variable.
/** This is a typedef for signed char, it ensures portability of the engine. */
#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef __int8			s8;
#else
typedef signed char		s8;
#endif

//! 8 bit character variable.
/** This is a typedef for char, it ensures portability of the engine. */
typedef char			c8;

//! 16 bit unsigned variable.
/** This is a typedef for unsigned short, it ensures portability of the engine. */
#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef unsigned __int16	u16;
#else
typedef unsigned short		u16;
#endif

//! 16 bit signed variable.
/** This is a typedef for signed short, it ensures portability of the engine. */
#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef __int16			s16;
#else
typedef signed short		s16;
#endif

//! 32 bit unsigned variable.
/** This is a typedef for unsigned int, it ensures portability of the engine. */
#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef unsigned __int32	u32;
#else
typedef unsigned int		u32;
#endif

//! 32 bit signed variable.
/** This is a typedef for signed int, it ensures portability of the engine. */
#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
typedef __int32			s32;
#else
typedef signed int		s32;
#endif

//! 32 bit floating point variable.
/** This is a typedef for float, it ensures portability of the engine. */
typedef float				f32;

typedef double				f64;

/*
interface macro
*/
#define __INTERFACE struct
/*
namespace macro to avoid multiple tabs
*/

#define _XE_BEGIN namespace xe{
#define _XE_END   }

#define _GRAPHIC_BEGIN namespace graphic{
#define _GRAPHIC_END   }

#define _MATH_BEGIN namespace math{
#define _MATH_END   }

#define _TOOL_BEGIN namespace tool{
#define _TOOL_END   }

#define _OS_BEGIN namespace os{
#define _OS_END   }

#define _D3D_BEGIN namespace d3d{
#define _D3D_END   }





