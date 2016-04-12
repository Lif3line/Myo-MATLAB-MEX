#pragma once

#include <inttypes.h>

#ifdef MYOACQDLL_EXPORTS
#define MYOACQDLL_API __declspec(dllexport) // Enabled as "export" while compiling the dll project
#else
#define MYOACQDLL_API __declspec(dllimport) // For using already created dll file
#endif

namespace MyoAcqSpace {
	class MyoAcq {
	public:
		static MYOACQDLL_API void init(void);

		static MYOACQDLL_API void getEMG(double *out);

		static MYOACQDLL_API void testFunct(int8_t in);
	};
}