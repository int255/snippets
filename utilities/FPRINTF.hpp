#ifndef FPRINTF_H_INCLUDED
#define FPRINTF_H_INCLUDED

#include <cstdarg>

#define FPRINTF(...) f_printf(__VA_ARGS__)

namespace {
#if _WIN32
	static const char * kF = "c:\\Temp\\printf.log.txt";
#elif defined(__APPLE__)
	static const char * kF = "/tmp/fprintf.log.txt";
#else
	#error UNSUPPORTED PLATFORM, fprintf.log.txt not defined
#endif

	void f_printf(const char * format, ...)
	{
		char buf[1024];
		va_list args;
		va_start(args, format);
		vsprintf(buf, format, args);

		va_end(args);
		FILE * f = fopen(kF, "ab+");
		if (f)
		{
			fwrite(buf, 1, strlen(buf), f);
			fclose(f);
		}
	}
};

#endif //FPRINTF_H_INCLUDED
