#ifndef CUSTOM_CLogger_H
#define CUSTOM_CLogger_H
#include <fstream>
#include <iostream>
#include <cstdarg>
#include <string>
using namespace std;



/**
*   Singleton Logger Class.
*/
class CLogger
{
	//Source: https://cppcodetips.wordpress.com/2014/01/02/a-simple-logger-class-in-c/

public:

	/**
	*    Default constructor for the Logger class.
	*/
	CLogger(const string& logFileName);


#ifndef _GAME
	void AttachGMUser(User * pcUser);
	void DeattachGMUser();
#endif

	void Log(const std::string& sMessage);
	void LogInfo(const std::string& sMessage);
	void LogStatus ( const std::string & sMessage);
	void LogWarn(const std::string& sMessage );
	void LogError(const std::string& sMessage );
	void LogDebug(const std::string& sMessage);
	void LogDebugPacket(const std::string& sMessage);
	void LogDebugIf(BOOL condition, const std::string& sMessage);
	void WriteLine(const std::string& sMessage);

	void WriteLine(const char* format, ...);
	void Log(const char* format, ...);
	void LogDebugIf(BOOL condition, const char* format, ...);
	void LogDebug(const char* format, ...);
	void LogDebugPacket(const char* format, ...);
	void LogWarn(const char* format, ...);
	void LogError(const char* format, ...);
	void LogInfo(const char* format, ...);
	void LogStatus (const char* format, ...);

	int GetNumOfErrors () { return iErrorCount; }
	int GetNumOfWarnings () { return iWarningCount; }

	/// <summary>
	/// The main purpose of this is to prevent an infinite loop when adding error or warning to ServerLog table
	/// but the query execution resulted in a warning or error. For example, due to running out of space
	/// </summary>
	void SetAllowInsertIntoServerLogTable ( BOOL bAllow ) { bAllowInsertIntoServerLog = bAllow; }

	/**
	*   Flush the log file
	*/
	void Flush();

	/**
	*   Close the log file
	*/
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
	*   Singleton logger class object pointer.
	**/
	CLogger* m_pThis = NULL;
	/**
	*   Log file stream object.
	**/
	ofstream m_Logfile;

	//Console handle
	HANDLE hConsole = NULL;

	std::mutex * cLogMutex;

	UINT iErrorCount;
	UINT iWarningCount;

	BOOL bAllowInsertIntoServerLog = FALSE;

	User * pcGMUser = NULL;
};
#endif