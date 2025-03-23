#ifndef CUSTOM_CLogger_H
#define CUSTOM_CLogger_H
#include <fstream>
#include <iostream>
#include <cstdarg>
#include <string>
using namespace std;

#define LOGERROR( msg, ... ) { CLogger::GetLogger()->Log( msg, ##__VA_ARGS__ ); }
#define WARN( msg, ... ) { CLogger::GetLogger()->Log( msg, ##__VA_ARGS__ ); }

#ifdef _DEBUG
#define DEBUG( msg, ... ) { CLogger::GetLogger()->Log( msg, ##__VA_ARGS__ ); }
#else
#define DEBUG( msg, ...) {}
#endif


#define ERROR( msg, ... ) Debug::GetInstance()->WriteLine( __FILENAME__, __LINE__, msg, ##__VA_ARGS__ )
#define DEBUGINFO( msg, ... ) Debug::GetInstance()->WriteLine( msg, ##__VA_ARGS__ )

#ifdef DEV_MODE
#define WRITEDBG( msg, ... ) { Debug::GetInstance()->WriteLine( __FILENAME__, __LINE__, msg, ##__VA_ARGS__ ); }
#else
#define WRITEDBG( msg, ...) {}
#endif

/**
*   Singleton Logger Class.
*/
class CLogger
{
	//Source: https://cppcodetips.wordpress.com/2014/01/02/a-simple-logger-class-in-c/

public:
	/**
	*   Logs a message
	*   @param sMessage message to be logged.
	*/
	void Log(const std::string& sMessage);
	void LogRaw(const std::string& sMessage);
	/**
	*   Variable Length Logger function
	*   @param format string for the message to be logged.
	*/
	void Log(const char* format, ...);

	void LogRaw(const char* format, va_list args);

	void Log(const char* format, va_list args);

	/**
	*   Flush the log file
	*/
	void Flush();


	void Close();
	/**
	*   << overloaded function to Logs a message
	*   @param sMessage message to be logged.
	*/

	CLogger& operator<<(const string& sMessage);
	/**
	*   Funtion to create the instance of logger class.
	*   @return singleton object of Clogger class..
	*/
	static CLogger* GetLogger();
	static CLogger* GetLogger(const string& sFileName);

private:
	/**
	*    Default constructor for the Logger class.
	*/
	CLogger();
	~CLogger();
	/**
	*   copy constructor for the Logger class.
	*/
	CLogger(const CLogger&) {};             // copy constructor is private
	/**
	*   assignment operator for the Logger class.
	*/
	CLogger& operator=(const CLogger&) { return *this; };  // assignment operator is private
	/**
	*   Log file name.
	**/
	static const std::string m_sFileName;
	static const std::string m_sFileNameOld;
	/**
	*   Singleton logger class object pointer.
	**/
	static CLogger* m_pThis;
	/**
	*   Log file stream object.
	**/
	static ofstream m_Logfile;

	std::mutex * cLogMutex;
};
#endif