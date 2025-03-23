#include "stdafx.h"
#include "Logger.h"
#include "Utilities.h"

const string CLogger::m_sFileName = "Log.txt";
const string CLogger::m_sFileNameOld = "Log_old.txt";
CLogger* CLogger::m_pThis = NULL;
ofstream CLogger::m_Logfile;


CLogger::CLogger()
{
	cLogMutex = new std::mutex ();

	//Source: https://cppcodetips.wordpress.com/2014/01/02/a-simple-logger-class-in-c/
}

CLogger::~CLogger ()
{
	SAFE_DELETE ( cLogMutex );
}

CLogger* CLogger::GetLogger() {
	if (m_pThis == NULL)
	{
		m_pThis = new CLogger();
		
		remove ( m_sFileNameOld.c_str () );
		rename ( m_sFileName.c_str (), m_sFileNameOld.c_str () );
		remove ( m_sFileName.c_str () );
		
		m_Logfile.open(m_sFileName.c_str(), ios::out | ios::app);
		m_Logfile << "" << "\n\n\n";
		m_Logfile << "START OF LOG: " << Util::CurrentDateTime () << '\n';
		m_Logfile << "-------------------------------------------------------------" << '\n';
	}
	return m_pThis;
}

CLogger* CLogger::GetLogger(const string& logFileName) {
	
	if (logFileName.empty())
		return nullptr;

	m_pThis = new CLogger();
	m_Logfile.open(logFileName.c_str(), ios::out | ios::app);
	return m_pThis;	
}

void CLogger::Log(const char* format, va_list args)
{
	char* sMessage = NULL;
	int nLength = 0;
	nLength = _vscprintf(format, args) + 1;
	sMessage = new char[nLength];
	vsprintf_s(sMessage, nLength, format, args);

	cLogMutex->lock ();
	m_Logfile << Util::CurrentDateTime() << ":\t";
	m_Logfile << sMessage << "\n";
	cLogMutex->unlock ();
	delete[] sMessage;
}

void CLogger::LogRaw ( const char * format, va_list args )
{
	char * sMessage = NULL;
	int nLength = 0;
	nLength = _vscprintf ( format, args ) + 1;
	sMessage = new char[nLength];
	vsprintf_s ( sMessage, nLength, format, args );
	cLogMutex->lock ();
	m_Logfile << sMessage;
	cLogMutex->unlock ();
	delete[] sMessage;
}

void CLogger::Log(const char* format, ...)
{
	char* sMessage = NULL;
	int nLength = 0;
	va_list args;
	va_start(args, format);
	//  Return the number of characters in the string referenced the list of arguments.
	// _vscprintf doesn't count terminating '\0' (that's why +1)
	nLength = _vscprintf(format, args) + 1;
	sMessage = new char[nLength];
	vsprintf_s(sMessage, nLength, format, args);
	//vsprintf(sMessage, format, args);
	cLogMutex->lock ();
	m_Logfile << Util::CurrentDateTime() << ":\t";
	m_Logfile << sMessage << "\n";
	cLogMutex->unlock ();
	va_end(args);

	delete[] sMessage;
}

void CLogger::Flush()
{
	cLogMutex->lock ();
	m_Logfile.flush();
	cLogMutex->unlock ();
}

void CLogger::Close()
{
	cLogMutex->lock ();
	m_Logfile.flush();
	m_Logfile.close();
	cLogMutex->unlock ();
}

void CLogger::Log(const string& sMessage)
{

	cLogMutex->lock ();
	m_Logfile << Util::CurrentDateTime() << ":\t";
	m_Logfile << sMessage << "\n";
	cLogMutex->unlock ();
}

void CLogger::LogRaw ( const string & sMessage )
{
	cLogMutex->lock ();
	m_Logfile << sMessage;
	cLogMutex->unlock ();
}

CLogger& CLogger::operator<<(const string& sMessage)
{
	cLogMutex->lock ();
	m_Logfile << "\n" << Util::CurrentDateTime() << ":\t";
	m_Logfile << sMessage << "\n";
	cLogMutex->unlock ();
	return *this;
}