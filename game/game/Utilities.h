#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <string>
#include <time.h>

//Source: https://cppcodetips.wordpress.com/2014/01/02/a-simple-logger-class-in-c/
namespace Util
{
	// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
	const std::string CurrentDateTime()
	{
		time_t     now = time(NULL);
		struct tm  tstruct;
		char       buf[80];
		localtime_s(&tstruct, &now);
		strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
		return buf;
	}
}
#endif