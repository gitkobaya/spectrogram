#include "csv.h"

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////ファイル操作関数

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
 * 入力データ読み込み。
 * ver0.1 2010/12/31 新規作成
 * ver0.2 2011/01/14 データ型設定変数の入力場所を変更
 *</PRE>
 * @param strFileName    ファイル名
 * @param strMode        ファイル読み込み read ファイル書き込み write
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
		// ファイルオープン処理
		if( strMode == "read" )
		{
			fp = fopen( strFileName.c_str(), "rb" );
			if( fp == NULL )
			{
				csve.SetErrorInfo( CSV_FILE_OPEN_ERROR, "Open", "CCsv", "ファイル読み込みでエラー発生", __LINE__ );
				throw( csve );
			}
			// CSVファイル行数、列数を取得。
			iTotalDataNum = GetDataNum();
		}
		else if( strMode == "write" )
		{
			fp = fopen( strFileName.c_str(), "w+b" );
			if( fp == NULL )
			{
				csve.SetErrorInfo( CSV_FILE_OPEN_ERROR, "Open", "CCsv", "ファイル読み込みでエラー発生", __LINE__ );
				throw( csve );
			}
		}
		// ファイルのモード設定
		strFileMode = strMode;
	}
	catch( CCsvException e )
	{
		throw(e);
	}
	catch( ... )
	{
		csve.SetErrorInfo( CSV_ERROR, "Read", "CCsv", "致命的エラー", __LINE__ );
		throw( csve );
	}
}

/**
 *<PRE>
 * ファイルを閉じる。
 * ver0.1 2010/12/31 新規作成
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
		// ファイル終了処理
		if( fclose( fp ) )
		{
			csve.SetErrorInfo( CSV_FILE_CLOSE_ERROR, "Close", "CCsv", "ファイル終了処理でエラー発生", __LINE__ );
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
		csve.SetErrorInfo( CSV_ERROR, "Close", "CCsv", "致命的エラー発生", __LINE__ );
		throw( csve );
	}
}

/**
 *<PRE>
 * 入力データ読み込み。
 * ver0.1 2010/12/31 新規作成
 * ver0.2 2011/01/14 データ読み込み部をまとめて処理するように修正
 *</PRE>
 * @param pvData   読み込むデータ
 * @param iNum     読み込むデータ数(個数を指定)
 * @param srtType  読み込むデータの型
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

			// エラーがあるかどうか確認
			lRet = ferror(fp);
			if( lRet )
			{
				csve.SetErrorInfo( lRet, "Read", "CCsv", _sys_errlist[(int)lRet], __LINE__ );
				throw( csve );
			}			

		/* 列の読み込み */
			if( cBuf == ',' )
			{
				//データの読み込みをする。
				_DataRead( pvData, iDataNum, acBuf, strType );

				// 読み込むデータ数をカウントする。
				iDataNum++;

				// 初期化
				memset( acBuf, 0 , sizeof(acBuf) );
				iBufCount = 0;
			}

		/* 行が次に移動する場合の動作 */
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
					// Windowsの改行コードの場合はそのままデータを読み出す。
				}
				else
				{
					// Unixの改行コードならば、ファイルポインタを戻す。
					lRet = fseek( fp, SEEK_CUR, -1 );
					if( lRet )
					{
						csve.SetErrorInfo( lRet, "Read", "CCsv", _sys_errlist[(int)lRet], __LINE__ );
						throw( csve );
					}
				}
				//データの読み込みをする。
				_DataRead( pvData, iDataNum, acBuf, strType );

				// 読み込むデータ数をカウントする。
				iDataNum++;

				// 初期化
				memset( acBuf, 0 , sizeof(acBuf) );
				iBufCount = 0;
			}
			else if( cBuf == '\n' )
			{
				// Macの改行コード

				//データの読み込みをする。
				_DataRead( pvData, iDataNum, acBuf, strType );

				// 読み込むデータ数をカウントする。
				iDataNum++;

				// 初期化
				memset( acBuf, 0 , sizeof(acBuf) );
				iBufCount = 0;
			}
			// データの読み込み動作
			else
			{
				acBuf[iBufCount] = cBuf;
				iBufCount += 1;
			}
			if( iDataNum == iNum )
			{
				// 指定データ読み込んだので読み込み終了。
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
		csve.SetErrorInfo( CSV_ERROR, "Read", "CCsv", "致命的エラー", __LINE__ );
		throw( csve );
	}
	return ;
}

/**
 *<PRE>
 * 入力データ読み込み。行数×列数分読みだす。
 * ver0.1 2010/12/31 新規作成
 * ver0.2 2011/01/14 スリム化
 *</PRE>
 * @param pvData   読み込むデータの型
 * @param iColumn  読みだす行数
 * @param iRow     読みだす列数
 * @param srtType  読み込むデータの型
 * @throw CCsvException
 * @author kobayashi
 * @since 2010/12/31
 * @version 0.2
 */
void CCsv::Read( void *pvData, int iColumn, int iRow, std::string strType )
{
	int iNum = 0;

	iNum = iColumn*iRow;

	//データの読み込み開始
	Read( pvData, iNum, strType );

	return ;
}


/**
 *<PRE>
 * データの書き込み。行数×列数。
 * ver0.1 2010/12/31 新規作成
 * ver0.2 2011/01/14 書き込みデータ型引数追加
 *</PRE>
 * @param pvData   書き込むデータの型
 * @param iColumn  書き込む行数
 * @param iRow     書き込む列数
 * @param srtType  読み込むデータの型
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
				//データの読み込みをする。
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
					// 書き込みにエラー発生
					csve.SetErrorInfo( lRet, "Write", "CCsv", "ファイルに書き込み時にエラー発生", __LINE__ );
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
		csve.SetErrorInfo( CSV_ERROR, "Write", "CCsv", "致命的エラー", __LINE__ );
		throw( csve );
	}
	return ;
}

/**
 *<PRE>
 * CSVファイル書き込み。
 * ver0.1 2010/12/31 新規作成
 * ver0.2 2011/01/14 書き込みデータ型引数追加
 *</PRE>
 * @param pvData   書き込むデータの型
 * @param iNum     書き込みデータ
 * @param srtType  書き込みデータの型
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
			//データの読み込みをする。
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
				// 書き込みにエラー発生
				csve.SetErrorInfo( lRet, "Write", "CCsv", "ファイルに書き込み時にエラー発生", __LINE__ );
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
		csve.SetErrorInfo( CSV_ERROR, "Write", "CCsv", "致命的エラー", __LINE__ );
		throw( csve );
	}
	return ;
}

/**
 *<PRE>
 * CSV列終了書き込み。
 * 列の書き込みが終了したらこの関数を呼び出して、
 * 次の行に改行する。
 * ver0.1 2010/12/31 新規作成
 * ver0.2 2011/01/14 説明文の追加
 *</PRE>
 * @param strEndLine 改行コードの指定。
 *                   windows windows形式
 *                   mac     mac形式
 *                   unix    unix形式
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
		//データの読み込みをする。
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
			// 書き込みにエラー発生
			csve.SetErrorInfo( lRet, "WriteEndLine", "CCsv", "ファイルに書き込み時にエラー発生", __LINE__ );
			throw( csve );
		}
	}
	catch( CCsvException e )
	{
		throw(e);
	}
	catch( ... )
	{
		csve.SetErrorInfo( CSV_ERROR, "Write", "CCsv", "致命的エラー", __LINE__ );
		throw( csve );
	}
	return ;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////プライベート関数

/**
 *<PRE>
 * CSVファイルの行数、列数を取得する。
 * ver0.1 2010/12/31 新規作成
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

			// エラーがあるかどうか確認
			lRet = ferror(fp);
			if( lRet )
			{
				csve.SetErrorInfo( lRet, "GetDataNum", "CCsv", _sys_errlist[(int)lRet], __LINE__ );
				throw( csve );
			}			
			/* 列の読み込み */
			if( cBuf == ',' )
			{
				// 読み込むデータ数をカウントする。
				iTempRow++;
				if( iTempRow > iRow ) iRow = iTempRow;
				iTempRow = 0;
			}

		/* 行が次に移動する場合の動作 */
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
					// Windowsの改行コードの場合はそのままデータを読み出す。
				}
				else
				{
					// Unixの改行コードならば、ファイルポインタを戻す。
					lRet = fseek( fp, SEEK_CUR, -1 );
					if( lRet )
					{
						csve.SetErrorInfo( lRet, "GetDataNum", "CCsv", _sys_errlist[(int)lRet], __LINE__ );
						throw( csve );
					}
				}
				// 列数をカウントする。
				iTempRow++;
				if( iTempRow > iRow ) iRow = iTempRow;
				iTempRow = 0;

				// 行数をカウントする。
				iColumn++;
			}
			else if( cBuf == '\n' )
			{
				// Macの改行コード

				// 読み込むデータ数をカウントする。
				iTempRow++;
				if( iTempRow > iRow ) iRow = iTempRow;
				iTempRow = 0;

				// 行数をカウントする。
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
		csve.SetErrorInfo( CSV_ERROR, "GetDataNum", "CCsv", "致命的エラー", __LINE__ );
		throw( csve );
	}
	return iRow*iColumn;
}

/**
 *<PRE>
 * 入力データ読み込み。
 * ver0.1 2011/01/14 新規作成
 *</PRE>
 * @param pvData   読み込むデータ
 * @param iDataNum 読み込むデータ数(個数を指定)
 * @param srtType  読み込むデータの型
 * @author kobayashi
 * @since 2011/01/14
 * @version 0.1
 */
void CCsv::_DataRead( void *pvData, int iDataNum, const char *pccBuf, std::string strType )
{
	//データの読み込みをする。
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
////////////////CCsvExceptionクラス

CCsvException::CCsvException()
{
	iErrCode = 0;
	strMethodName = "";
	strClassName = "";
	strErrDetail = "";
}

/*
 * コンストラクタ(エラー情報を設定する)
 * @param iCode       エラーコード
 * @param sMethodName 関数名
 * @param sClassName  クラス名
 * @param sDetail     エラー詳細
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
 * コンストラクタ(エラー情報を設定する)
 * @param iCode       エラーコード
 * @param sMethodName 関数名
 * @param sClassName  クラス名
 * @param sDetail     エラー詳細
 * @param iLine       行数
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
 * コンストラクタ(エラー情報を設定する)
 * @param iCode       エラーコード
 * @param sMethodName 関数名
 * @param sClassName  クラス名
 * @param sDetail     エラー詳細
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
 * コンストラクタ(エラー情報を設定する)
 * @param iCode       エラーコード
 * @param sMethodName 関数名
 * @param sClassName  クラス名
 * @param sDetail     エラー詳細
 * @param iLine       行数
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
 * エラー番号を出力する
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
int CCsvException::iGetErrCode()
{
	return iErrCode;
}

/*
 * エラーを起こした行数を出力する
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
int CCsvException::iGetErrLine()
{
	return iErrLine;
}

/*
 * エラーを起こした関数名を出力する
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CCsvException::strGetMethodName()
{
	return strMethodName;
}

/*
 * エラーを起こしたクラスを出力する
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CCsvException::strGetClassName()
{
	return strClassName;
}

/*
 * エラーの詳細情報を出力する
 * @author kobayashi
 * @since 2009/6/14
 * @version 1.0
 */
std::string CCsvException::strGetErrDetail()
{
	return strErrDetail;
}

