#pragma once

#if defined(_MSC_VER) && defined(_DEBUG)
#	define _CRTDBG_MAP_ALLOC
#	ifdef _malloca
#		undef _malloca
#	endif
#	include <crtdbg.h>

#	define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

#endif