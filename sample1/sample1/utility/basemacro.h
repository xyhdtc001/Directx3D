#pragma once
#ifndef _BASEMACRO_H
#define _BASEMACRO_H
#include <malloc.h>
#include <string>
#include <iostream>

namespace bx {
	inline constexpr bool ignoreC4127(bool _x)
	{
		return _x;
	}

    static void outputdebuginfo(char *szFormat, ...)
	{
		char temp[8192];
		char* out = temp;
		va_list argList;
		va_start(argList, szFormat);
		int len = vsnprintf(out, sizeof(temp), szFormat, argList);
		if ((int)sizeof(temp) < len)
		{
			out = (char*)alloca(len + 1);
			len = vsnprintf(out, len, szFormat, argList);
			out[len] = '\0';
			std::cout << out << std::endl;
			OutputDebugStringA(out);
		}
		else
		{
			std::cout << "error,too long info...." << std::endl;
			OutputDebugStringA("error,too long info....");
		}
		va_end(argList);
	}

}

#define  D3DSAMPLE_CHECKPTR(ptr, returnvalue) if (ptr == NULL) return returnvalue;

#define BX_MACRO_BLOCK_BEGIN for(;;) {
#define BX_MACRO_BLOCK_END break; }

#define  BX_IGNORE_C4127(_condition) bx::ignoreC4127(!!(_condition))

#define BX_STRINGIZE_(_x) #_x
#define BX_STRINGIZE(_x) BX_STRINGIZE_(_x)


#if 0
#	define DX_CHECK_EXTRA_F " (%s): %s"
#	define DX_CHECK_EXTRA_ARGS , DXGetErrorString(__hr__), DXGetErrorDescription(__hr__)
#else
#	define DX_CHECK_EXTRA_F ""
#	define DX_CHECK_EXTRA_ARGS 
#endif

#define _BX_CHECK(_condition, _format, ...)                        \
					if (!BX_IGNORE_C4127(_condition) )             \
					{                                              \
						bx::outputdebuginfo(__FILE__ "(" BX_STRINGIZE(__LINE__) "): BX " _format "\n", ##__VA_ARGS__); \
					}                                              \

#define _DX_CHECK(_call) \
				BX_MACRO_BLOCK_BEGIN \
				HRESULT __hr__ = _call; \
				_BX_CHECK(SUCCEEDED(__hr__), #_call " FAILED 0x%08x" DX_CHECK_EXTRA_F "\n" \
					, (uint32_t)__hr__ \
					DX_CHECK_EXTRA_ARGS \
					); \
				BX_MACRO_BLOCK_END
#endif