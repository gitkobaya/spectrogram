
#include <iostream>
#include <cstdio>
#include <cstdarg>
#include "cmd_check.h"

CCmdCheck::CCmdCheck()
{
	iFrame = 128;
	iSampling = 1000;
	lfTimeInter = 1;
	iNormalizeFlag = 0;
	iWriteMode = 1;
	iWindowFlag = 0;
	lfNoiseLevel = -100;
	memset( accWindow, '\0', sizeof(accWindow) );
	memset( accInFileName, '\0', sizeof(accInFileName) );
	memset( accOutFileName, '\0', sizeof(accOutFileName) );
	memset( accInDataType, '\0', sizeof(accInDataType) );
	strcpy( accInDataType, "" );
	memset( accOutDataType, '\0', sizeof(accOutDataType) );
	strcpy( accOutDataType, "" );
	strcpy( accWindow, "rectangle" );
	memset( alfWindowPara, 0.0, sizeof(alfWindowPara) );
}

CCmdCheck::~CCmdCheck()
{
	iFrame = 0;
	iSampling = 0;
	lfTimeInter = 0;
	iNormalizeFlag = 0;
	iWindowFlag = 0;
	memset( accWindow, '\0', sizeof(accWindow) );
	memset( accInFileName, '\0', sizeof(accInFileName) );
	memset( accOutFileName, '\0', sizeof(accOutFileName) );
	memset( accInDataType, '\0', sizeof(accInDataType) );
	memset( accOutDataType, '\0', sizeof(accOutDataType) );
	memset( alfWindowPara, 0.0, sizeof(alfWindowPara) );
}

#if 1
/**
 * ���͂��ꂽ�R�}���h���`�F�b�N
 * @author kobayashi
 * @param argc
 * @param argv
 * @version 1.0
 **/
void CCmdCheck::vCommandCheck( int argc, char* argv[] )
{
	int i;
	long lRet = CCMD_SUCCESS;
	CCmdCheckException ccmde;

	/* �R�}���h�̃`�F�b�N */
	if( argc <= 1 )
	{
		ccmde.SetErrorInfo( CCMD_ERROR_INVALID_FORMAT, "CommandCheck", "CCmdCheck", "�R�}���h�t�H�[�}�b�g�G���[", __LINE__ );
		throw( ccmde );
	}
	if( argv == NULL )
	{
		ccmde.SetErrorInfo( CCMD_ERROR_COMMAND_NULL, "CommandCheck", "CCmdCheck", "�R�}���h�t�H�[�}�b�g�G���[", __LINE__ );
		throw( ccmde );
	}
	for( i=1; i<argc ;i++ )
	{
		/* ��͂���t���[�������w�� */
		if( strcmp( argv[i], "-frame" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i+1] );
			if( lRet == 0 )
			{
				ccmde.SetErrorInfo( CCMD_ERROR_INVALID_FORMAT, "CommandCheck", "CCmdCheck", "�R�}���h�t�H�[�}�b�g�G���[", __LINE__ );
				throw( ccmde );
			}
			iFrame = atoi( argv[i+1] );
			i++;
		}
		/* �T���v�����O���g�� */
		else if( strcmp( argv[i], "-sampling" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i+1] );
			if( lRet == 0 )
			{
				ccmde.SetErrorInfo( CCMD_ERROR_INVALID_FORMAT, "CommandCheck", "CCmdCheck", "�R�}���h�t�H�[�}�b�g�G���[", __LINE__ );
				throw( ccmde );
			}
			iSampling = atoi( argv[i+1] );
			i++;
		}
		/* ��͊Ԋu */
		else if( strcmp( argv[i], "-tinter" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i+1] );
			if( lRet == 0 )
			{
				ccmde.SetErrorInfo( CCMD_ERROR_INVALID_FORMAT, "CommandCheck", "CCmdCheck", "�R�}���h�t�H�[�}�b�g�G���[", __LINE__ );
				throw( ccmde );
			}
			lfTimeInter = atof( argv[i+1] );
			i++;
		}
		/* �m�C�Y���x�� */
		else if (strcmp(argv[i], "-noiselevel") == 0)
		{
			lRet = lCommandErrorCheck(argv[i + 1]);
			if (lRet == 0)
			{
				ccmde.SetErrorInfo(CCMD_ERROR_INVALID_FORMAT, "CommandCheck", "CCmdCheck", "�R�}���h�t�H�[�}�b�g�G���[", __LINE__);
				throw(ccmde);
			}
			lfNoiseLevel = atof(argv[i + 1]);
			i++;
		}
		/* ���֐� */
		else if( strcmp( argv[i], "-window" ) == 0 )
		{
			vGetWindowFlag( argv, &i );
		}
		/* ���̓t�@�C�� */
		else if( strcmp( argv[i], "-in" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i+1] );
			if( lRet == 0 )
			{
				ccmde.SetErrorInfo( CCMD_ERROR_INVALID_FORMAT, "CommandCheck", "CCmdCheck", "�R�}���h�t�H�[�}�b�g�G���[", __LINE__ );
				throw( ccmde );
			}
			strcpy( accInFileName, argv[i+1] );
			// �f�[�^�^�C�v�̓ǂݍ���
			strcpy( accInDataType, "double" );
			i++;
		}
		/* �o�̓t�@�C�� */
		else if( strcmp( argv[i], "-out" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i+1] );
			if( lRet == 0 )
			{
				ccmde.SetErrorInfo( CCMD_ERROR_INVALID_FORMAT, "CommandCheck", "CCmdCheck", "�R�}���h�t�H�[�}�b�g�G���[", __LINE__ );
				throw( ccmde );
			}
			if( strlen(argv[i+1]) == 1 )
			{
				if( strcmp( argv[i+1] , "1") == 0  || strcmp( argv[i+1], "2" ) || strcmp(argv[i + 1], "3") || strcmp(argv[i + 1], "4") == 0 )
				{
					iWriteMode = atoi( argv[i+1] );
					lRet = lCommandErrorCheck( argv[i+1] );
					if( lRet == 0 )
					{
						ccmde.SetErrorInfo( CCMD_ERROR_INVALID_FORMAT, "CommandCheck", "CCmdCheck", "�R�}���h�t�H�[�}�b�g�G���[", __LINE__ );
						throw( ccmde );
					}
					strcpy( accOutFileName, argv[i+2] );
					i+=2;
				}
				else
				{
					strcpy( accOutFileName, argv[i+1] );
					i++;
				}
			}
			else
			{
				strcpy( accOutFileName, argv[i+1] );
				i++;
			}
			// �f�[�^�^�C�v�̓ǂݍ���
			strcpy( accOutDataType, "double" );
		}
		/* �o�̓t�@�C�� */
		else if( strcmp( argv[i], "-normalize" ) == 0 )
		{
			lRet = lCommandErrorCheck( argv[i+1] );
			if( lRet == 0 )
			{
				ccmde.SetErrorInfo( CCMD_ERROR_INVALID_FORMAT, "CommandCheck", "CCmdCheck", "�R�}���h�t�H�[�}�b�g�G���[", __LINE__ );
				throw( ccmde );
			}
			iNormalizeFlag = atoi( argv[i+1] );
			i++;
		}
		else
		{
			ccmde.SetErrorInfo( CCMD_ERROR_INVALID_DATA, "CommandCheck", "CCmdCheck", "�R�}���h�̒l���s��", __LINE__ );
			throw( ccmde );
		}
	}
}

long CCmdCheck::lCommandErrorCheck( char* argv )
{
	long lRet = 0;

	if( argv == NULL ) return lRet;
	/* ��͂���t���[�������w�� */
	if( ( strcmp( argv, "-frame" ) == 0 )			||
		( strcmp( argv, "-sampling" ) == 0 )		||
		( strcmp( argv, "-tinter" ) == 0 )			||
		( strcmp( argv, "-window" ) == 0 )			||
		( strcmp( argv, "-in" ) == 0 )				||
		( strcmp( argv, "-out" ) == 0 )				||
		( strcmp( argv, "-normalize" ) == 0 )		||
		( strcmp( argv, "-noiselevel") == 0 ))
	{
		lRet = 0;
	}
	else
	{
		lRet = -1;
	}
	return lRet;
}
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////���̈ʒu����A�v���P�[�V�����ɉ����č쐬���镔��
#if 1

void CCmdCheck::vGetWindowFlag( char** pcWindow, int* piCmdLoc )
{
	long lRet = CCMD_SUCCESS;
	CCmdCheckException ccmde;

	lRet = lCommandErrorCheck( pcWindow[*piCmdLoc+1] );
	if( lRet == 0 )
	{
		ccmde.SetErrorInfo( CCMD_ERROR_INVALID_FORMAT, "CommandCheck", "CCmdCheck", "�R�}���h�t�H�[�}�b�g�G���[", __LINE__ );
		throw( ccmde );
	}

	if( strcmp( "rectangle", pcWindow[*piCmdLoc+1] ) == 0 )
	{
		iWindowFlag = 0;
		*piCmdLoc = *piCmdLoc+1;
	}
	else if( strcmp( "bartlett", pcWindow[*piCmdLoc+1] ) == 0 )
	{
		iWindowFlag = 1;
		*piCmdLoc = *piCmdLoc+1;
	}
	else if( strcmp( "hanning", pcWindow[*piCmdLoc+1] ) == 0 )
	{
		iWindowFlag = 2;
		*piCmdLoc = *piCmdLoc+1;
	}
	else if( strcmp( "hamming", pcWindow[*piCmdLoc+1] ) == 0 )
	{
		iWindowFlag = 3;
		*piCmdLoc = *piCmdLoc+1;
	}
	else if( strcmp( "blackman", pcWindow[*piCmdLoc+1] ) == 0 )
	{
		iWindowFlag = 4;
		*piCmdLoc = *piCmdLoc+1;
	}
	else if( strcmp( "blackmanhann", pcWindow[*piCmdLoc+1] ) == 0 )
	{
		iWindowFlag = 5;
		*piCmdLoc = *piCmdLoc+1;
	}
	else if( strcmp( "generalhamming", pcWindow[*piCmdLoc+1] ) == 0 )
	{
		iWindowFlag = 6;
		lRet = lCommandErrorCheck( pcWindow[*piCmdLoc+2] );
		if( lRet == 0 )
		{
			ccmde.SetErrorInfo( CCMD_ERROR_INVALID_FORMAT, "CommandCheck", "CCmdCheck", "�R�}���h�t�H�[�}�b�g�G���[", __LINE__ );
			throw( ccmde );
		}
		alfWindowPara[0] = atof( pcWindow[*piCmdLoc+2] );
		*piCmdLoc = *piCmdLoc+2;
	}
	else if( strcmp( "kaiser", pcWindow[*piCmdLoc+1] ) == 0 )
	{
		iWindowFlag = 7;
		lRet = lCommandErrorCheck( pcWindow[*piCmdLoc+2] );
		if( lRet == 0 )
		{
			ccmde.SetErrorInfo( CCMD_ERROR_INVALID_FORMAT, "CommandCheck", "CCmdCheck", "�R�}���h�t�H�[�}�b�g�G���[", __LINE__ );
			throw( ccmde );
		}
		alfWindowPara[0] = atof( pcWindow[*piCmdLoc+2] );
		*piCmdLoc = *piCmdLoc+2;
	}
	else if( strcmp( "gaussian", pcWindow[*piCmdLoc+1] ) == 0 )
	{
		iWindowFlag = 8;
		lRet = lCommandErrorCheck( pcWindow[*piCmdLoc+2] );
		if( lRet == 0 )
		{
			ccmde.SetErrorInfo( CCMD_ERROR_INVALID_FORMAT, "CommandCheck", "CCmdCheck", "�R�}���h�t�H�[�}�b�g�G���[", __LINE__ );
			throw( ccmde );
		}
		alfWindowPara[0] = atof( pcWindow[*piCmdLoc+2] );
		*piCmdLoc = *piCmdLoc+2;
	}
	else if( strcmp( "exponential", pcWindow[*piCmdLoc+1] ) == 0 )
	{
		iWindowFlag = 9;
		lRet = lCommandErrorCheck( pcWindow[*piCmdLoc+2] );
		if( lRet == 0 )
		{
			ccmde.SetErrorInfo( CCMD_ERROR_INVALID_FORMAT, "CommandCheck", "CCmdCheck", "�R�}���h�t�H�[�}�b�g�G���[", __LINE__ );
			throw( ccmde );
		}
		alfWindowPara[0] = atof( pcWindow[*piCmdLoc+2] );
		*piCmdLoc = *piCmdLoc+2;
	}
	else if( strcmp( "flattop", pcWindow[*piCmdLoc+1] ) == 0 )
	{
		iWindowFlag = 10;
		*piCmdLoc = *piCmdLoc+1;
	}
	else if( strcmp( "blackmannuttall", pcWindow[*piCmdLoc+1] ) == 0 )
	{
		iWindowFlag = 11;
		*piCmdLoc = *piCmdLoc+1;
	}
	else if( strcmp( "parzen", pcWindow[*piCmdLoc+1] ) == 0 )
	{
		iWindowFlag = 12;
		*piCmdLoc = *piCmdLoc+1;
	}
	else if( strcmp( "welch", pcWindow[*piCmdLoc+1] ) == 0 )
	{
		iWindowFlag = 13;
		*piCmdLoc = *piCmdLoc+1;
	}
	else if( strcmp( "blackmanharris", pcWindow[*piCmdLoc+1] ) == 0 )
	{
		iWindowFlag = 14;
		*piCmdLoc = *piCmdLoc+1;
	}
	else if( strcmp( "akaike", pcWindow[*piCmdLoc+1] ) == 0 )
	{
		iWindowFlag = 15;
		*piCmdLoc = *piCmdLoc+1;
	}
	else
	{
		ccmde.SetErrorInfo( CCMD_ERROR_INVALID_WINDOW, "CommandCheck", "CCmdCheck", "�s���ȑ��֐��w��", __LINE__ );
		throw( ccmde );
	}
}

void CCmdCheck::vGetWindow( char* pcWindow )
{
	CCmdCheckException ccmde;
	if( pcWindow == NULL )
	{
		ccmde.SetErrorInfo( CCMD_ERROR_INVALID_DATA, "CommandCheck", "vGetWindow", "������擾�p�ϐ�NULL", __LINE__ );
		throw( ccmde );
	}
	try
	{
		strcpy( pcWindow, accWindow );
	}
	catch( ... )
	{
		ccmde.SetErrorInfo( CCMD_ERROR_EXTEND_STRING, "CommandCheck", "vGetWindow", "�z��T�C�Y�����ĎQ��", __LINE__ );
		throw( ccmde );
	}
}

void CCmdCheck::vGetInFileName( char* pcInFileName )
{
	CCmdCheckException ccmde;
	if( pcInFileName == NULL )
	{
		ccmde.SetErrorInfo( CCMD_ERROR_INVALID_DATA, "CommandCheck", "vGetInFileName", "������擾�p�ϐ�NULL", __LINE__ );
		throw( ccmde );
	}
	try
	{
		strcpy( pcInFileName, accInFileName );
	}
	catch( ... )
	{
		ccmde.SetErrorInfo( CCMD_ERROR_EXTEND_STRING, "CommandCheck", "vGetInFileName", "�z��T�C�Y�����ĎQ��", __LINE__ );
		throw( ccmde );
	}
}

void CCmdCheck::vGetOutFileName( char* pcOutFileName )
{
	CCmdCheckException ccmde;
	if( pcOutFileName == NULL )
	{
		ccmde.SetErrorInfo( CCMD_ERROR_INVALID_DATA, "CommandCheck", "vGetOutFileName", "������擾�p�ϐ�NULL", __LINE__ );
		throw( ccmde );
	}
	try
	{
		strcpy( pcOutFileName, accOutFileName );
	}
	catch( ... )
	{
		ccmde.SetErrorInfo( CCMD_ERROR_EXTEND_STRING, "CommandCheck", "vGetOutFileName", "�z��T�C�Y�����ĎQ��", __LINE__ );
		throw( ccmde );
	}
}

void CCmdCheck::vGetInDataType( char* pcDataType )
{
	CCmdCheckException ccmde;
	if( pcDataType == NULL )
	{
		ccmde.SetErrorInfo( CCMD_ERROR_INVALID_DATA, "CommandCheck", "vGetInDataType", "������擾�p�ϐ�NULL", __LINE__ );
		throw( ccmde );
	}
	try
	{
		strcpy( pcDataType, accInDataType );
	}
	catch( ... )
	{
		ccmde.SetErrorInfo( CCMD_ERROR_EXTEND_STRING, "CommandCheck", "vGetInDataType", "�z��T�C�Y�����ĎQ��", __LINE__ );
		throw( ccmde );
	}
}

void CCmdCheck::vGetOutDataType( char* pcDataType )
{
	CCmdCheckException ccmde;
	if( pcDataType == NULL )
	{
		ccmde.SetErrorInfo( CCMD_ERROR_INVALID_DATA, "CommandCheck", "vGetOutDataType", "������擾�p�ϐ�NULL", __LINE__ );
		throw( ccmde );
	}
	try
	{
		strcpy( pcDataType, accOutDataType );
	}
	catch( ... )
	{
		ccmde.SetErrorInfo( CCMD_ERROR_EXTEND_STRING, "CommandCheck", "vGetOutDataType", "�z��T�C�Y�����ĎQ��", __LINE__ );
		throw( ccmde );
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////CCmdCheckException�N���X

CCmdCheckException::CCmdCheckException()
{
	iErrCode = 0;
	strMethodName = "";
	strClassName = "";
	strErrDetail = "";
}

/*
 * �R���X�g���N�^(�G���[����ݒ肷��)
 * @param iCode       �G���[�R�[�h
 * @param sMethodName �֐���
 * @param sClassName  �N���X��
 * @param sDetail     �G���[�ڍ�
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
CCmdCheckException::CCmdCheckException(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail)
{
	iErrCode = iCode;
	strMethodName = sMethodName;
	strClassName = sClassName;
	strErrDetail = sDetail;
}

/*
 * �R���X�g���N�^(�G���[����ݒ肷��)
 * @param iCode       �G���[�R�[�h
 * @param sMethodName �֐���
 * @param sClassName  �N���X��
 * @param sDetail     �G���[�ڍ�
 * @param iLine       �s��
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
CCmdCheckException::CCmdCheckException(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iLine)
{
	iErrCode = iCode;
	iErrLine = iLine;
	strMethodName = sMethodName;
	strClassName = sClassName;
	strErrDetail = sDetail;
}


CCmdCheckException::~CCmdCheckException()
{
	iErrCode = 0;
	strMethodName = "";
	strClassName = "";
	strErrDetail = "";
}

/*
 * �R���X�g���N�^(�G���[����ݒ肷��)
 * @param iCode       �G���[�R�[�h
 * @param sMethodName �֐���
 * @param sClassName  �N���X��
 * @param sDetail     �G���[�ڍ�
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
void CCmdCheckException::SetErrorInfo(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail)
{
	iErrCode = iCode;
	strMethodName = sMethodName;
	strClassName = sClassName;
	strErrDetail = sDetail;
}

/*
 * �R���X�g���N�^(�G���[����ݒ肷��)
 * @param iCode       �G���[�R�[�h
 * @param sMethodName �֐���
 * @param sClassName  �N���X��
 * @param sDetail     �G���[�ڍ�
 * @param iLine       �s��
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
void CCmdCheckException::SetErrorInfo(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iLine)
{
	iErrCode = iCode;
	iErrLine = iLine;
	strMethodName = sMethodName;
	strClassName = sClassName;
	strErrDetail = sDetail;
}

/*
 * �G���[�ԍ����o�͂���
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
int CCmdCheckException::iGetErrCode()
{
	return iErrCode;
}

/*
 * �G���[���N�������s�����o�͂���
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
int CCmdCheckException::iGetErrLine()
{
	return iErrLine;
}

/*
 * �G���[���N�������֐������o�͂���
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CCmdCheckException::strGetMethodName()
{
	return strMethodName;
}

/*
 * �G���[���N�������N���X���o�͂���
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CCmdCheckException::strGetClassName()
{
	return strClassName;
}

/*
 * �G���[�̏ڍ׏����o�͂���
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CCmdCheckException::strGetErrDetail()
{
	return strErrDetail;
}
