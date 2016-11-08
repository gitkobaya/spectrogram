#ifndef _CSV_H_
#define _CSV_H_

#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<string>

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
////////////�t�@�C������֐�

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
	void Open( std::string strFileName, std::string strMode );
	
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
	void Close();

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
	void Read( void *pvData, int iNum, std::string strType );

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
	void Read( void *pvData, int iColumn, int iRow, std::string strType );

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
	void Write( void *pvData, int iColumn, int iRow, std::string styType );

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
	void Write( void *pvData, int iNum, std::string strType );

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
	void WriteEndLine( std::string strEndLine );

	long GetColumn(){return iColumn;};
	long GetRow(){return iRow;};

private:
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
	long GetDataNum();

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