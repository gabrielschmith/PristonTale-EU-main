#include "stdafx.h"
#include "Utilities.h"

namespace Util
{
	const std::string Util::CurrentDateTime()
	{
		time_t     now = time(NULL);
		struct tm  tstruct;
		char       buf[80];
		localtime_s(&tstruct, &now);
		strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
		return buf;
	}

	const std::string Util::CurrentDate()
	{
		time_t     now = time(NULL);
		struct tm  tstruct;
		char       buf[20];
		localtime_s(&tstruct, &now);
		strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
		return buf;
	}
}