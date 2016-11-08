#ifndef _CCMDCHECK_H_
#define _CCMDCHECK_H_

#include <string>

#define CCMD_SUCCESS							 0
#define CCMD_ERROR_INVALID_FORMAT				-1001
#define CCMD_ERROR_INVALID_DATA					-1002
#define CCMD_ERROR_MEMORY_ALLOCATE				-1003
#define CCMD_ERROR_MEMORY_RELEASE				-1004
#define CCMD_ERROR_EXTEND_STRING				-1005
#define CCMD_ERROR_COMMAND_NULL					-1006
#define CCMD_ERROR_INVALID_FRAME				-1007
#define CCMD_ERROR_INVALID_SAMPLING				-1008
#define CCMD_ERROR_INVALID_INTERVAL				-1009
#define CCMD_ERROR_INVALID_NORMALIZE			-1010
#define CCMD_ERROR_INVALID_WRITEMODE			-1011
#define CCMD_ERROR_INVALID_WINDOW				-1012

class CCmdCheck
{
public:
	CCmdCheck();
	virtual ~CCmdCheck();
	/**
	 * 入力されたコマンドを解析
	 * @author kobayashi
	 * @param argc
	 * @param argv
	 * @version 1.0
	 */
	void vCommandCheck( int argc, char* argv[] );

	/**
	 * 入力されたコマンドがフォーマットに適していない場合、エラーを返却
	 * @author kobayashi
	 * @param argc
	 * @param argv
	 * @version 1.0
	 */
	long lCommandErrorCheck( char* argv );

	/*
	 * 以下の部分はアプリケーションに応じて作成する部分
	 * おもにゲット関数を作成。
	 */
	int iGetFrame()			{ return iFrame; };
	int iGetSampling()		{ return iSampling; };
	double lfGetTimeInter() { return lfTimeInter; };
	int iGetNormalizeFlag() { return iNormalizeFlag; };
	int iGetWriteMode()		{ return iWriteMode;};
	int iGetWindowFlag()	{ return iWindowFlag;};
	double lfGetNoiseLevel() { return lfNoiseLevel; };
	double* lfGetParam()		{ return alfWindowPara;};
	const char* pcGetInFileName(){ return (const char*)accInFileName;};
	const char* pcGetOutFileName(){ return (const char*)accOutFileName;};
	const char* pcGetInDataType(){ return (const char*)accInDataType;};
	const char* pcGetOutDataType(){ return (const char*)accOutDataType;};

	void vGetWindowFlag( char** pcWindow, int* piCmdLoc );
	void vGetWindow( char* pcWindow );
	void vGetInFileName( char* pcInFileName );
	void vGetOutFileName( char* pcOutFileName );
	void vGetInDataType( char* pcDataType );
	void vGetOutDataType( char* pcDataType );

private:
	// 以下の部分はコマンドラインexe作成時に
	// アプリケーションに応じて作成する部分
#if 1
	int iFrame;
	int iSampling;
	int iWriteMode;
	int iWindowFlag;
	double lfTimeInter;
	double lfNoiseLevel;
	char accWindow[256];
	char accInFileName[256];
	char accOutFileName[256];
	char accInDataType[256];
	char accOutDataType[256];
	int iNormalizeFlag;
	double alfWindowPara[256];
#endif
};


class CCmdCheckException
{
public:
	CCmdCheckException();
	CCmdCheckException( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail );
	CCmdCheckException( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iErrorLine );
	~CCmdCheckException();
	void SetErrorInfo( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail );
	void SetErrorInfo( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iErrorLine );
	int iGetErrCode();
	int iGetErrLine();
	std::string strGetMethodName();
	std::string strGetClassName();
	std::string strGetErrDetail();
private:
	int iErrCode;
	int iErrLine;
	std::string strMethodName;
	std::string strClassName;
	std::string strModuleName;
	std::string strErrDetail;
};

#endif 