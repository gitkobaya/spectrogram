#include "csv.h"

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////�t�@�C������֐�

CCsv::CCsv()
{
	fp = NULL;
	strReadFileName = "";
	strWriteFileName = "";
	strFileMode = "";
	iRow = 0;
	iColumn = 0;
}

CCsv::~CCsv()
{
}

/**
 *<PRE>
 * ���̓f�[�^�ǂݍ��݁B
 * ver0.1 2010/12/31 �V�K�쐬
 * ver0.2 2011/01/14 �f�[�^�^�ݒ�ϐ��̓��͏ꏊ��ύX
 *</PRE>
 * @param strFileName    �t�@�C����
 * @param strMode        �t�@�C���ǂݍ��� read �t�@�C���������� write
 * @throw CCsvException
 * @author kobayashi
 * @since 2010/12/31
 * @version 0.2
 */
void CCsv::Open( std::string strFileName, std::string strMode )
{
	CCsvException csve;

	try
	{
		// �t�@�C���I�[�v������
		if( strMode == "read" )
		{
			fp = fopen( strFileName.c_str(), "rb" );
			if( fp == NULL )
			{
				csve.SetErrorInfo( CSV_FILE_OPEN_ERROR, "Open", "CCsv", "�t�@�C���ǂݍ��݂ŃG���[����", __LINE__ );
				throw( csve );
			}
			// CSV�t�@�C���s���A�񐔂��擾�B
			iTotalDataNum = GetDataNum();
		}
		else if( strMode == "write" )
		{
			fp = fopen( strFileName.c_str(), "w+b" );
			if( fp == NULL )
			{
				csve.SetErrorInfo( CSV_FILE_OPEN_ERROR, "Open", "CCsv", "�t�@�C���ǂݍ��݂ŃG���[����", __LINE__ );
				throw( csve );
			}
		}
		// �t�@�C���̃��[�h�ݒ�
		strFileMode = strMode;
	}
	catch( CCsvException e )
	{
		throw(e);
	}
	catch( ... )
	{
		csve.SetErrorInfo( CSV_ERROR, "Read", "CCsv", "�v���I�G���[", __LINE__ );
		throw( csve );
	}
}

/**
 *<PRE>
 * �t�@�C�������B
 * ver0.1 2010/12/31 �V�K�쐬
 *</PRE>
 * @throw CCsvException
 * @author kobayashi
 * @since 2010/12/31
 * @version 0.1
 */
void CCsv::Close()
{
	CCsvException csve;
	try
	{
		// �t�@�C���I������
		if( fclose( fp ) )
		{
			csve.SetErrorInfo( CSV_FILE_CLOSE_ERROR, "Close", "CCsv", "�t�@�C���I�������ŃG���[����", __LINE__ );
			throw( csve );
		}
		fp = NULL;
	}
	catch( CCsvException e )
	{
		throw(e);
	}
	catch( ... )
	{
		csve.SetErrorInfo( CSV_ERROR, "Close", "CCsv", "�v���I�G���[����", __LINE__ );
		throw( csve );
	}
}

/**
 *<PRE>
 * ���̓f�[�^�ǂݍ��݁B
 * ver0.1 2010/12/31 �V�K�쐬
 * ver0.2 2011/01/14 �f�[�^�ǂݍ��ݕ����܂Ƃ߂ď�������悤�ɏC��
 *</PRE>
 * @param pvData   �ǂݍ��ރf�[�^
 * @param iNum     �ǂݍ��ރf�[�^��(�����w��)
 * @param srtType  �ǂݍ��ރf�[�^�̌^
 * @throw CCsvException
 * @author kobayashi
 * @since 2010/12/31
 * @version 0.2
 */
void CCsv::Read( void *pvData, int iNum, std::string strType )
{
	int iBufCount = 0;
	int iDataNum = 0;
	char cBuf;
	char acBuf[256];
	long lRet = 0L;
	CCsvException csve;

	try
	{
		while( !feof(fp) )
		{
			lRet = fread( &cBuf, 1, 1, fp );
			if( lRet != 1 ) break;

			// �G���[�����邩�ǂ����m�F
			lRet = ferror(fp);
			if( lRet )
			{
				csve.SetErrorInfo( lRet, "Read", "CCsv", _sys_errlist[(int)lRet], __LINE__ );
				throw( csve );
			}			

		/* ��̓ǂݍ��� */
			if( cBuf == ',' )
			{
				//�f�[�^�̓ǂݍ��݂�����B
				_DataRead( pvData, iDataNum, acBuf, strType );

				// �ǂݍ��ރf�[�^�����J�E���g����B
				iDataNum++;

				// ������
				memset( acBuf, 0 , sizeof(acBuf) );
				iBufCount = 0;
			}

		/* �s�����Ɉړ�����ꍇ�̓��� */
			else if( cBuf == '\r' )
			{
				lRet = fread( &cBuf, 1, 1, fp );
				if( lRet != 1 ) break;
				lRet = ferror(fp);
				if( lRet )
				{
					csve.SetErrorInfo( lRet, "Read", "CCsv", _sys_errlist[(int)lRet], __LINE__ );
					throw( csve );
				}
				if( cBuf == '\n' ) 
				{
					// Windows�̉��s�R�[�h�̏ꍇ�͂��̂܂܃f�[�^��ǂݏo���B
				}
				else
				{
					// Unix�̉��s�R�[�h�Ȃ�΁A�t�@�C���|�C���^��߂��B
					lRet = fseek( fp, SEEK_CUR, -1 );
					if( lRet )
					{
						csve.SetErrorInfo( lRet, "Read", "CCsv", _sys_errlist[(int)lRet], __LINE__ );
						throw( csve );
					}
				}
				//�f�[�^�̓ǂݍ��݂�����B
				_DataRead( pvData, iDataNum, acBuf, strType );

				// �ǂݍ��ރf�[�^�����J�E���g����B
				iDataNum++;

				// ������
				memset( acBuf, 0 , sizeof(acBuf) );
				iBufCount = 0;
			}
			else if( cBuf == '\n' )
			{
				// Mac�̉��s�R�[�h

				//�f�[�^�̓ǂݍ��݂�����B
				_DataRead( pvData, iDataNum, acBuf, strType );

				// �ǂݍ��ރf�[�^�����J�E���g����B
				iDataNum++;

				// ������
				memset( acBuf, 0 , sizeof(acBuf) );
				iBufCount = 0;
			}
			// �f�[�^�̓ǂݍ��ݓ���
			else
			{
				acBuf[iBufCount] = cBuf;
				iBufCount += 1;
			}
			if( iDataNum == iNum )
			{
				// �w��f�[�^�ǂݍ��񂾂̂œǂݍ��ݏI���B
				break;
			}
		}
	}
	catch( CCsvException e )
	{
		throw(e);
	}
	catch( ... )
	{
		csve.SetErrorInfo( CSV_ERROR, "Read", "CCsv", "�v���I�G���[", __LINE__ );
		throw( csve );
	}
	return ;
}

/**
 *<PRE>
 * ���̓f�[�^�ǂݍ��݁B�s���~�񐔕��ǂ݂����B
 * ver0.1 2010/12/31 �V�K�쐬
 * ver0.2 2011/01/14 �X������
 *</PRE>
 * @param pvData   �ǂݍ��ރf�[�^�̌^
 * @param iColumn  �ǂ݂����s��
 * @param iRow     �ǂ݂�����
 * @param srtType  �ǂݍ��ރf�[�^�̌^
 * @throw CCsvException
 * @author kobayashi
 * @since 2010/12/31
 * @version 0.2
 */
void CCsv::Read( void *pvData, int iColumn, int iRow, std::string strType )
{
	int iNum = 0;

	iNum = iColumn*iRow;

	//�f�[�^�̓ǂݍ��݊J�n
	Read( pvData, iNum, strType );

	return ;
}


/**
 *<PRE>
 * �f�[�^�̏������݁B�s���~�񐔁B
 * ver0.1 2010/12/31 �V�K�쐬
 * ver0.2 2011/01/14 �������݃f�[�^�^�����ǉ�
 *</PRE>
 * @param pvData   �������ރf�[�^�̌^
 * @param iColumn  �������ލs��
 * @param iRow     �������ޗ�
 * @param srtType  �ǂݍ��ރf�[�^�̌^
 * @throw CCsvException
 * @author kobayashi
 * @since 2010/12/31
 * @version 0.2
 */
void CCsv::Write( void *pvData, int iColumn, int iRow, std::string strType )
{
	int i,j;
	long lRet;
	int iData = 0;
	short shData = 0;
	long lData = 0;
	float fData = 0.0;
	double lfData = 0.0;
	char cData = '\0';
	CCsvException csve;

	lRet = 0L;
	try
	{
		for( i = 0; i < iColumn; i++ )
		{
			for( j = 0; j < iRow; j++ )
			{
				//�f�[�^�̓ǂݍ��݂�����B
				if( strType == "double" )
				{
					lfData = *((double*)pvData+j+i*iRow);
					lRet = fprintf( fp, "%lf,", lfData );
				}
				else if( strType == "float" )
				{
					fData = *((float*)pvData+j+i*iRow);
					lRet = fprintf( fp, "%lf,", fData );
				}
				else if( strType == "int" )
				{
					iData = *((int*)pvData+j+i*iRow);
					lRet = fprintf( fp, "%d,", iData );
				}
				else if( strType == "short" )
				{
					shData = *((short*)pvData+j+i*iRow);
					lRet = fprintf( fp, "%d,", shData );
				}
				else if( strType == "long" )
				{
					lData = *((long*)pvData+j+i*iRow);
					lRet = fprintf( fp, "%l,", lData );
				}
				else if( strType == "char" )
				{
					cData = *((char*)pvData+j+i*iRow);
					lRet = fprintf( fp, "%d,", cData );
				}
				else
				{
					lRet = fprintf( fp, "%s,", (char*)((char*)pvData+j+i*iRow) );
				}
				if( lRet < 0 )
				{
					// �������݂ɃG���[����
					csve.SetErrorInfo( lRet, "Write", "CCsv", "�t�@�C���ɏ������ݎ��ɃG���[����", __LINE__ );
					throw( csve );
				}
			}
		}
	}
	catch( CCsvException e )
	{
		throw(e);
	}
	catch( ... )
	{
		csve.SetErrorInfo( CSV_ERROR, "Write", "CCsv", "�v���I�G���[", __LINE__ );
		throw( csve );
	}
	return ;
}

/**
 *<PRE>
 * CSV�t�@�C���������݁B
 * ver0.1 2010/12/31 �V�K�쐬
 * ver0.2 2011/01/14 �������݃f�[�^�^�����ǉ�
 *</PRE>
 * @param pvData   �������ރf�[�^�̌^
 * @param iNum     �������݃f�[�^
 * @param srtType  �������݃f�[�^�̌^
 * @throw CCsvException
 * @author kobayashi
 * @since 2010/12/31
 * @version 0.2
 */
void CCsv::Write( void *pvData, int iNum, std::string strType )
{
	int i;
	long lRet;
	int iData = 0;
	short shData = 0;
	long lData = 0;
	float fData = 0.0;
	double lfData = 0.0;
	char cData = '\0';
	CCsvException csve;

	lRet = 0L;
	try
	{
		for( i = 0; i < iNum; i++ )
		{
			//�f�[�^�̓ǂݍ��݂�����B
			if( strType == "double" )
			{
				lfData = *((double*)pvData+i);
				lRet = fprintf( fp, "%lf,", lfData );
			}
			else if( strType == "float" )
			{
				fData = *((float*)pvData+i);
				lRet = fprintf( fp, "%lf,", fData );
			}
			else if( strType == "int" )
			{
				iData = *((int*)pvData+i);
				lRet = fprintf( fp, "%d,", iData );
			}
			else if( strType == "short" )
			{
				shData = *((short*)pvData+i);
				lRet = fprintf( fp, "%d,", shData );
			}
			else if( strType == "long" )
			{
				lData = *((long*)pvData+i);
				lRet = fprintf( fp, "%l,", lData );
			}
			else if( strType == "char" )
			{
				cData = *((char*)pvData+i);
				lRet = fprintf( fp, "%d,", cData );
			}
			else
			{
				lRet = fprintf( fp, "%s,", (char*)((char*)pvData+i) );
			}
			if( lRet < 0 )
			{
				// �������݂ɃG���[����
				csve.SetErrorInfo( lRet, "Write", "CCsv", "�t�@�C���ɏ������ݎ��ɃG���[����", __LINE__ );
				throw( csve );
			}
		}
	}
	catch( CCsvException e )
	{
		throw(e);
	}
	catch( ... )
	{
		csve.SetErrorInfo( CSV_ERROR, "Write", "CCsv", "�v���I�G���[", __LINE__ );
		throw( csve );
	}
	return ;
}

/**
 *<PRE>
 * CSV��I���������݁B
 * ��̏������݂��I�������炱�̊֐����Ăяo���āA
 * ���̍s�ɉ��s����B
 * ver0.1 2010/12/31 �V�K�쐬
 * ver0.2 2011/01/14 �������̒ǉ�
 *</PRE>
 * @param strEndLine ���s�R�[�h�̎w��B
 *                   windows windows�`��
 *                   mac     mac�`��
 *                   unix    unix�`��
 * @throw CCsvException
 * @author kobayashi
 * @since 2010/12/31
 * @version 0.2
 */
void CCsv::WriteEndLine( std::string strEndLine )
{
	int i;
	long lRet;
	CCsvException csve;

	lRet = 0L;

	try
	{
		//�f�[�^�̓ǂݍ��݂�����B
		if( strEndLine == "windows" )
		{
			lRet = fprintf( fp, "\r\n" );
		}
		else if( strEndLine == "mac" )
		{
			lRet = fprintf( fp, "\r" );
		}
		else if( strEndLine == "unix" )
		{
			lRet = fprintf( fp, "\n" );
		}
		else
		{
			lRet = -1;
		}
		if( lRet < 0 )
		{
			// �������݂ɃG���[����
			csve.SetErrorInfo( lRet, "WriteEndLine", "CCsv", "�t�@�C���ɏ������ݎ��ɃG���[����", __LINE__ );
			throw( csve );
		}
	}
	catch( CCsvException e )
	{
		throw(e);
	}
	catch( ... )
	{
		csve.SetErrorInfo( CSV_ERROR, "Write", "CCsv", "�v���I�G���[", __LINE__ );
		throw( csve );
	}
	return ;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////�v���C�x�[�g�֐�

/**
 *<PRE>
 * CSV�t�@�C���̍s���A�񐔂��擾����B
 * ver0.1 2010/12/31 �V�K�쐬
 *</PRE>
 * @throw CCsvException
 * @author kobayashi
 * @since 2010/12/31
 * @version 0.1
 */
long CCsv::GetDataNum()
{
	int iBufCount = 0;
	int iDataNum = 0;
	int iTempRow = 0;
	char cBuf;
	long lRet = 0L;
	CCsvException csve;

	try
	{
		while( !feof(fp) )
		{
			lRet = fread( &cBuf, 1, 1, fp );
			if( lRet != 1 ) break;

			// �G���[�����邩�ǂ����m�F
			lRet = ferror(fp);
			if( lRet )
			{
				csve.SetErrorInfo( lRet, "GetDataNum", "CCsv", _sys_errlist[(int)lRet], __LINE__ );
				throw( csve );
			}			
			/* ��̓ǂݍ��� */
			if( cBuf == ',' )
			{
				// �ǂݍ��ރf�[�^�����J�E���g����B
				iTempRow++;
				if( iTempRow > iRow ) iRow = iTempRow;
				iTempRow = 0;
			}

		/* �s�����Ɉړ�����ꍇ�̓��� */
			else if( cBuf == '\r' )
			{
				lRet = fread( &cBuf, 1, 1, fp );
				if( lRet != 1 ) break;
				lRet = ferror(fp);
				if( lRet )
				{
					csve.SetErrorInfo( lRet, "GetDataNum", "CCsv", _sys_errlist[(int)lRet], __LINE__ );
					throw( csve );
				}
				if( cBuf == '\n' ) 
				{
					// Windows�̉��s�R�[�h�̏ꍇ�͂��̂܂܃f�[�^��ǂݏo���B
				}
				else
				{
					// Unix�̉��s�R�[�h�Ȃ�΁A�t�@�C���|�C���^��߂��B
					lRet = fseek( fp, SEEK_CUR, -1 );
					if( lRet )
					{
						csve.SetErrorInfo( lRet, "GetDataNum", "CCsv", _sys_errlist[(int)lRet], __LINE__ );
						throw( csve );
					}
				}
				// �񐔂��J�E���g����B
				iTempRow++;
				if( iTempRow > iRow ) iRow = iTempRow;
				iTempRow = 0;

				// �s�����J�E���g����B
				iColumn++;
			}
			else if( cBuf == '\n' )
			{
				// Mac�̉��s�R�[�h

				// �ǂݍ��ރf�[�^�����J�E���g����B
				iTempRow++;
				if( iTempRow > iRow ) iRow = iTempRow;
				iTempRow = 0;

				// �s�����J�E���g����B
				iColumn++;
			}
		}
		lRet = fseek( fp, SEEK_SET, 0 );
		if( lRet )
		{
			csve.SetErrorInfo( lRet, "GetDataNum", "CCsv", _sys_errlist[(int)lRet], __LINE__ );
			throw( csve );
		}
	}
	catch( ... )
	{
		csve.SetErrorInfo( CSV_ERROR, "GetDataNum", "CCsv", "�v���I�G���[", __LINE__ );
		throw( csve );
	}
	return iRow*iColumn;
}

/**
 *<PRE>
 * ���̓f�[�^�ǂݍ��݁B
 * ver0.1 2011/01/14 �V�K�쐬
 *</PRE>
 * @param pvData   �ǂݍ��ރf�[�^
 * @param iDataNum �ǂݍ��ރf�[�^��(�����w��)
 * @param srtType  �ǂݍ��ރf�[�^�̌^
 * @author kobayashi
 * @since 2011/01/14
 * @version 0.1
 */
void CCsv::_DataRead( void *pvData, int iDataNum, const char *pccBuf, std::string strType )
{
	//�f�[�^�̓ǂݍ��݂�����B
	if( strType == "double" )
	{
		*((double*)pvData+iDataNum) = atof( pccBuf );
	}
	else if( strType == "float" )
	{
		*((float*)pvData+iDataNum) = atof( pccBuf );
	}
	else if( strType == "int" )
	{
		*((int*)pvData+iDataNum) = (int)atof( pccBuf );
	}
	else if( strType == "short" )
	{
		*((short*)pvData+iDataNum) = (short)atof( pccBuf );
	}
	else if( strType == "long" )
	{
		*((long*)pvData+iDataNum) = (long)atof( pccBuf );
	}
	else if( strType == "char" )
	{
		strcpy( ((char*)pvData+iDataNum), pccBuf );
	}
	else
	{
		*((double*)pvData+iDataNum) = atof( pccBuf );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////CCsvException�N���X

CCsvException::CCsvException()
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
CCsvException::CCsvException(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail)
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
CCsvException::CCsvException(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iLine )
{
	iErrCode = iCode;
	iErrLine = iLine;
	strMethodName = sMethodName;
	strClassName = sClassName;
	strErrDetail = sDetail;
}


CCsvException::~CCsvException()
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
void CCsvException::SetErrorInfo(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail)
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
void CCsvException::SetErrorInfo(int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iLine)
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
int CCsvException::iGetErrCode()
{
	return iErrCode;
}

/*
 * �G���[���N�������s�����o�͂���
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
int CCsvException::iGetErrLine()
{
	return iErrLine;
}

/*
 * �G���[���N�������֐������o�͂���
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CCsvException::strGetMethodName()
{
	return strMethodName;
}

/*
 * �G���[���N�������N���X���o�͂���
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CCsvException::strGetClassName()
{
	return strClassName;
}

/*
 * �G���[�̏ڍ׏����o�͂���
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CCsvException::strGetErrDetail()
{
	return strErrDetail;
}

