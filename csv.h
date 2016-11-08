#ifndef _CSV_H_
#define _CSV_H_

#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<string>

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////ファイル操作関数

#define CSV_SUCCESS					0
#define CSV_ERROR				-2000
#define CSV_FILE_OPEN_ERROR		-2001
#define CSV_FILE_CLOSE_ERROR	-2002
#define CSV_FILE_READ_ERROR		-2003
#define CSV_FILE_WRITE_ERROR	-2004

class CCsv 
{
public:
	CCsv();
	~CCsv();

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
	void Open( std::string strFileName, std::string strMode );
	
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
	void Close();

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
	void Read( void *pvData, int iNum, std::string strType );

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
	void Read( void *pvData, int iColumn, int iRow, std::string strType );

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
	void Write( void *pvData, int iColumn, int iRow, std::string styType );

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
	void Write( void *pvData, int iNum, std::string strType );

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
	void WriteEndLine( std::string strEndLine );

	long GetColumn(){return iColumn;};
	long GetRow(){return iRow;};

private:
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
	long GetDataNum();

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
	void _DataRead( void *pvData, int iDataNum, const char *pccBuf, std::string strType );

private:
	FILE *fp;
	std::string strReadFileName;
	std::string strWriteFileName;
	std::string strFileMode;

	int iRow;
	int iColumn;
	int iTotalDataNum;
};

class CCsvException
{
public:
	CCsvException();
	CCsvException( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail );
	CCsvException( int iCode, std::string sMethodName, std::string sClassName, std::string sDetail, int iErrorLine );
	~CCsvException();
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