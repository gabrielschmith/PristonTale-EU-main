#include "stdafx.h"
#include "Logger.h"
#include "Logger.h"
#include "Utilities.h"

//Source: https://cppcodetips.wordpress.com/2014/01/02/a-simple-logger-class-in-c/


#define CONSOLE_BLACK			0
#define CONSOLE_BLUE			1
#define CONSOLE_GREEN			2
#define CONSOLE_CYAN			3
#define CONSOLE_RED				4
#define CONSOLE_MAGENTA			5
#define CONSOLE_BROWN			6
#define CONSOLE_LIGHTGRAY		7
#define CONSOLE_DARKGRAY		8
#define CONSOLE_LIGHTBLUE		9
#define CONSOLE_LIGHTGREEN		10
#define CONSOLE_LIGHTCYAN		11
#define CONSOLE_LIGHTRED		12
#define CONSOLE_LIGHTMAGENTA	13
#define CONSOLE_YELLOW			14
#define CONSOLE_WHITE			15

CLogger::CLogger ()
{
	cLogMutex = new std::mutex ();
	iErrorCount = 0;
	iWarningCount = 0;
}

CLogger::~CLogger ()
{
	SAFE_DELETE ( cLogMutex );
}


CLogger::CLogger ( const string & logFileName ) : CLogger ()
{
	hConsole = GetStdHandle ( STD_OUTPUT_HANDLE );

	m_pThis = new CLogger ();
	m_Logfile.open ( logFileName.c_str (), ios::out | ios::app );
}

void CLogger::AttachGMUser( User * pcUser )
{
	pcGMUser = pcUser;
}

void CLogger::DeattachGMUser( )
{
	pcGMUser = NULL;
}

void CLogger::WriteLine ( const char * format, ... )
{
	char * sMessage = NULL;

	try
	{
		int nLength = 0;
		va_list args;
		va_start ( args, format );
		//  Return the number of characters in the string referenced the list of arguments.
		// _vscprintf doesn't count terminating '\0' (that's why +1)
		nLength = _vscprintf ( format, args ) + 1;
		sMessage = new char[nLength];
		vsprintf_s ( sMessage, nLength, format, args );
		va_end ( args );
	}
	catch ( ... )
	{
		WARN ( std::string ( FormatString ( "Log format is incorrect: %s", format ) ) );
		return;
	}

	//vsprintf(sMessage, format, args);
	cLogMutex->lock ();
	m_Logfile << sMessage << "\n";
	cLogMutex->unlock ();

	delete[] sMessage;
}


void CLogger::Log ( const char * format, ... )
{
	char * sMessage = NULL;

	try
	{
		int nLength = 0;
		va_list args;
		va_start ( args, format );
		//  Return the number of characters in the string referenced the list of arguments.
		// _vscprintf doesn't count terminating '\0' (that's why +1)
		nLength = _vscprintf ( format, args ) + 1;
		sMessage = new char[nLength];
		vsprintf_s ( sMessage, nLength, format, args );
		va_end ( args );
	}
	catch ( ... )
	{
		WARN ( std::string ( FormatString ( "Log format is incorrect: %s", format ) ) );
		return;
	}

	//vsprintf(sMessage, format, args);
	cLogMutex->lock ();
	m_Logfile << Util::CurrentDateTime () << ":\t";
	m_Logfile << sMessage << "\n";
	cLogMutex->unlock ();

	delete[] sMessage;
}

void CLogger::LogError ( const char * format, ... )
{
	char * sMessage = NULL;

	try
	{
		int nLength = 0;
		va_list args;
		va_start ( args, format );
		//  Return the number of characters in the string referenced the list of arguments.
		// _vscprintf doesn't count terminating '\0' (that's why +1)
		nLength = _vscprintf ( format, args ) + 1;
		sMessage = new char[nLength];
		vsprintf_s ( sMessage, nLength, format, args );
		//vsprintf(sMessage, format, args);
		va_end ( args );
	}
	catch ( ... )
	{
		WARN ( std::string ( FormatString ( "Log format is incorrect: %s", format ) ) );
		return;
	}

	cLogMutex->lock ();
	m_Logfile << Util::CurrentDateTime () << ":\t";
	m_Logfile << "ERROR: " << sMessage << '\n'; //note - use \n instead of std::endl to prevent console from being flushed twice

	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_LIGHTRED );
	std::cout << "[ERROR] " << sMessage << '\n'; //note - use \n instead of std::endl to prevent console from being flushed twice
	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_WHITE );
	cLogMutex->unlock ();

	if ( bAllowInsertIntoServerLog ) LOGSERVER->OnServerLog ( sMessage, EServerLogType::Error );

	if ( pcGMUser && !pcGMUser->bDisableWarnAndErrorRelaysToClient )
	{
		CHATSERVER->SendChatEx( pcGMUser, EChatColor::CHATCOLOR_Error, "Error: %s", sMessage);
	}

	iErrorCount++;

	SetConsoleTextAttribute ( hConsole, CONSOLE_WHITE );

	delete[] sMessage;
}

void CLogger::LogDebugIf ( BOOL condition, const char * format, ... )
{

	char * sMessage = NULL;

#ifdef DEV_MODE

	if ( condition )
	{
		try
		{
			int nLength = 0;
			va_list args;
			va_start ( args, format );
			//  Return the number of characters in the string referenced the list of arguments.
			// _vscprintf doesn't count terminating '\0' (that's why +1)
			nLength = _vscprintf ( format, args ) + 1;
			sMessage = new char[nLength];
			vsprintf_s ( sMessage, nLength, format, args );
			//vsprintf(sMessage, format, args);
			va_end ( args );
		}
		catch ( ... )
		{
			WARN ( std::string ( FormatString ( "Log format is incorrect: %s", format ) ) );
			return;
		}

		cLogMutex->lock ();
		m_Logfile << Util::CurrentDateTime () << ":\t";
		m_Logfile << "DEBUG: " << sMessage << "\n";

		if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_DARKGRAY );
		std::cout << "[DEBUG] " << sMessage << '\n'; //note - use \n instead of std::endl to prevent console from being flushed twice
		if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_WHITE );
		cLogMutex->unlock ();

		SetConsoleTextAttribute ( hConsole, CONSOLE_WHITE );

		delete[] sMessage;
	}

#endif //  DEV_MODE
}

void CLogger::LogDebug ( const char * format, ... )
{
#ifdef DEV_MODE

	char * sMessage = NULL;

	try
	{
		int nLength = 0;
		va_list args;
		va_start ( args, format );
		//  Return the number of characters in the string referenced the list of arguments.
		// _vscprintf doesn't count terminating '\0' (that's why +1)
		nLength = _vscprintf ( format, args ) + 1;
		sMessage = new char[nLength];
		vsprintf_s ( sMessage, nLength, format, args );
		//vsprintf(sMessage, format, args);
		va_end ( args );
	}
	catch ( ... )
	{
		WARN ( std::string ( FormatString ( "Log format is incorrect: %s", format ) ) );
		return;
	}

	cLogMutex->lock ();
	m_Logfile << Util::CurrentDateTime () << ":\t";
	m_Logfile << "DEBUG: " << sMessage << "\n";
	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_BROWN );
	std::cout << "[DEBUG] " << sMessage << '\n'; //note - use \n instead of std::endl to prevent console from being flushed twice
	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_WHITE );
	cLogMutex->unlock ();

	delete[] sMessage;

#endif //  DEV_MODE
}

void CLogger::LogDebugPacket ( const char * format, ... )
{
#ifdef DEV_MODE

	char * sMessage = NULL;

	try
	{
		int nLength = 0;
		va_list args;
		va_start ( args, format );
		//  Return the number of characters in the string referenced the list of arguments.
		// _vscprintf doesn't count terminating '\0' (that's why +1)
		nLength = _vscprintf ( format, args ) + 1;
		sMessage = new char[nLength];
		vsprintf_s ( sMessage, nLength, format, args );
		//vsprintf(sMessage, format, args);
		va_end ( args );
	}
	catch ( ... )
	{
		WARN ( std::string ( FormatString ( "Log format is incorrect: %s", format ) ) );
		return;
	}

	cLogMutex->lock ();
	m_Logfile << Util::CurrentDateTime () << ":\t";
	m_Logfile << "PACKET: " << sMessage << "\n";
	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_DARKGRAY );
	std::cout << "[PACKET] " << sMessage << '\n'; //note - use \n instead of std::endl to prevent console from being flushed twice
	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_WHITE );
	cLogMutex->unlock ();

	delete[] sMessage;

#endif //  DEV_MODE
}


void CLogger::LogWarn ( const char * format, ... )
{
	char * sMessage = NULL;

	try
	{
		int nLength = 0;
		va_list args;
		va_start ( args, format );
		//  Return the number of characters in the string referenced the list of arguments.
		// _vscprintf doesn't count terminating '\0' (that's why +1)
		nLength = _vscprintf ( format, args ) + 1;
		sMessage = new char[nLength];
		vsprintf_s ( sMessage, nLength, format, args );
		//vsprintf(sMessage, format, args);
		va_end ( args );
	}
	catch (...)
	{
		WARN ( std::string ( FormatString ( "Log format is incorrect: %s", format ) ) );
		return;
	}

	cLogMutex->lock ();
	m_Logfile << Util::CurrentDateTime () << ":\t";
	m_Logfile << "WARN: " << sMessage << "\n";

	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_RED );
	std::cout << "[WARN] " << sMessage << '\n'; //note - use \n instead of std::endl to prevent console from being flushed twice
	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_WHITE );
	cLogMutex->unlock ();

	if ( bAllowInsertIntoServerLog ) LOGSERVER->OnServerLog ( sMessage, EServerLogType::Warn );

	if ( pcGMUser && !pcGMUser->bDisableWarnAndErrorRelaysToClient )
	{
		CHATSERVER->SendChatEx( pcGMUser, EChatColor::CHATCOLOR_Error, "Warn: %s", sMessage);
	}

	iWarningCount++;

	delete[] sMessage;
}


void CLogger::LogInfo ( const char * format, ... )
{
	char * sMessage = NULL;

	try
	{
		int nLength = 0;
		va_list args;
		va_start ( args, format );
		//  Return the number of characters in the string referenced the list of arguments.
		// _vscprintf doesn't count terminating '\0' (that's why +1)
		nLength = _vscprintf ( format, args ) + 1;
		sMessage = new char[nLength];
		vsprintf_s ( sMessage, nLength, format, args );
		//vsprintf(sMessage, format, args);
		va_end ( args );
	}
	catch ( ... )
	{
		WARN ( std::string ( FormatString ( "Log format is incorrect: %s", format ) ) );
		return;
	}

	cLogMutex->lock ();
	m_Logfile << Util::CurrentDateTime () << ":\t";
	m_Logfile << "INFO: " << sMessage << "\n";

	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_LIGHTGRAY );
	std::cout << "[INFO] " << sMessage << '\n'; //note - use \n instead of std::endl to prevent console from being flushed twice
	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_WHITE );
	cLogMutex->unlock ();

	delete[] sMessage;
}



void CLogger::LogStatus ( const char * format, ... )
{
	char * sMessage = NULL;

	try
	{
		int nLength = 0;
		va_list args;
		va_start ( args, format );
		//  Return the number of characters in the string referenced the list of arguments.
		// _vscprintf doesn't count terminating '\0' (that's why +1)
		nLength = _vscprintf ( format, args ) + 1;
		sMessage = new char[nLength];
		vsprintf_s ( sMessage, nLength, format, args );
		//vsprintf(sMessage, format, args);
		va_end ( args );
	}
	catch ( ... )
	{
		WARN ( std::string ( FormatString ( "Log format is incorrect: %s", format ) ) );
		return;
	}

	cLogMutex->lock ();
	m_Logfile << Util::CurrentDateTime () << ":\t";
	m_Logfile << "STATUS: " << sMessage << "\n";

	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_LIGHTGREEN );
	std::cout << "[STATUS] " << sMessage << '\n'; //note - use \n instead of std::endl to prevent console from being flushed twice
	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_WHITE );
	cLogMutex->unlock ();


	if ( bAllowInsertIntoServerLog ) LOGSERVER->OnServerLog ( sMessage, EServerLogType::Status );

	delete[] sMessage;
}

void CLogger::Flush ()
{
	if ( m_Logfile )
		m_Logfile.flush ();
}


void CLogger::Close ()
{
	if ( m_Logfile )
		m_Logfile.close ();
}

void CLogger::WriteLine ( const string & sMessage )
{
	m_Logfile << sMessage << "\n";
}

void CLogger::Log ( const string & sMessage )
{
	cLogMutex->lock ();
	m_Logfile << Util::CurrentDateTime () << ":\t";
	m_Logfile << sMessage << "\n";
	cLogMutex->unlock ();
}

void CLogger::LogInfo ( const string & sMessage )
{
	cLogMutex->lock ();
	m_Logfile << Util::CurrentDateTime () << ":\t";
	m_Logfile << "[INFO] " << sMessage << "\n";

	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_LIGHTGRAY );
	std::cout << "[INFO] " << sMessage << '\n'; //note - use \n instead of std::endl to prevent console from being flushed twice
	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_WHITE );
	cLogMutex->unlock ();
}

void CLogger::LogStatus ( const string & sMessage )
{
	cLogMutex->lock ();
	m_Logfile << Util::CurrentDateTime () << ":\t";
	m_Logfile << "[STATUS] " << sMessage << "\n";

	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_LIGHTGREEN );
	std::cout << "[STATUS] " << sMessage << '\n'; //note - use \n instead of std::endl to prevent console from being flushed twice
	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_WHITE );
	cLogMutex->unlock ();

	if ( bAllowInsertIntoServerLog ) LOGSERVER->OnServerLog ( sMessage, EServerLogType::Status );
}

void CLogger::LogWarn ( const string & sMessage )
{
	cLogMutex->lock ();
	m_Logfile << Util::CurrentDateTime () << ":\t";
	m_Logfile << "INFO: " << sMessage << "\n";

	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_RED );
	std::cout << "[WARN] " << sMessage << '\n'; //note - use \n instead of std::endl to prevent console from being flushed twice
	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_WHITE );
	cLogMutex->unlock ();

	if ( bAllowInsertIntoServerLog ) LOGSERVER->OnServerLog ( sMessage, EServerLogType::Warn );

	if ( pcGMUser && !pcGMUser->bDisableWarnAndErrorRelaysToClient )
	{
		CHATSERVER->SendChatEx( pcGMUser, EChatColor::CHATCOLOR_Error, "Warn: %s", sMessage);
	}

	iWarningCount++;
}

void CLogger::LogError ( const string & sMessage )
{
	cLogMutex->lock ();
	m_Logfile << Util::CurrentDateTime () << ":\t";
	m_Logfile << "ERROR: " << sMessage << "\n";

	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_LIGHTRED );
	std::cout << "[ERROR] " << sMessage << '\n'; //note - use \n instead of std::endl to prevent console from being flushed twice
	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_WHITE );
	cLogMutex->unlock ();

	if (bAllowInsertIntoServerLog) LOGSERVER->OnServerLog ( sMessage, EServerLogType::Error );

	if ( pcGMUser && !pcGMUser->bDisableWarnAndErrorRelaysToClient )
	{
		CHATSERVER->SendChatEx( pcGMUser, EChatColor::CHATCOLOR_Error, "Error: %s", sMessage);
	}

	iErrorCount++;
}

void CLogger::LogDebugIf ( BOOL condition, const string & sMessage )
{
#ifdef DEV_MODE
	if ( condition )
	{
		cLogMutex->lock ();
		m_Logfile << Util::CurrentDateTime () << ":\t";
		m_Logfile << "DEBUG: " << sMessage << "\n";

		if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_DARKGRAY );
		std::cout << "[DEBUG] " << sMessage << '\n'; //note - use \n instead of std::endl to prevent console from being flushed twice
		if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_WHITE );
		cLogMutex->unlock ();
	}
#endif // DEV_MODE
}

void CLogger::LogDebug ( const string & sMessage )
{
#ifdef DEV_MODE

	cLogMutex->lock ();
	m_Logfile << Util::CurrentDateTime () << ":\t";
	m_Logfile << "DEBUG: " << sMessage << "\n";

	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_BROWN );
	std::cout << "[DEBUG] " << sMessage << '\n'; //note - use \n instead of std::endl to prevent console from being flushed twice
	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_WHITE );
	cLogMutex->unlock ();

#endif // DEV_MODE
}

void CLogger::LogDebugPacket ( const string & sMessage )
{
#ifdef DEV_MODE

	cLogMutex->lock ();
	m_Logfile << Util::CurrentDateTime () << ":\t";
	m_Logfile << "PACKET: " << sMessage << "\n";

	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_DARKGRAY );
	std::cout << "[PACKET] " << sMessage << '\n'; //note - use \n instead of std::endl to prevent console from being flushed twice
	if ( hConsole ) SetConsoleTextAttribute ( hConsole, CONSOLE_WHITE );
	cLogMutex->unlock ();

#endif // DEV_MODE
}

CLogger & CLogger::operator<<( const string & sMessage )
{
	cLogMutex->lock ();
	m_Logfile << "\n" << Util::CurrentDateTime () << ":\t";
	m_Logfile << sMessage << "\n";
	cLogMutex->unlock ();

	return *this;
}