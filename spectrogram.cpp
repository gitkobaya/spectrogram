#include <cstdio>
#include <string>
#include <iostream>
#include "fft.h"
#include "windowfunc.h"
#include "search_maxmin.h"
#include "cmd_check.h"
#include "csv.h"
#include "iir.hpp"

// �O���[�o���ϐ�


// �֐��v���g�^�C�v�錾
extern void vSpectrogram( CCmdCheck cmd );
extern void vSetWindowFunction( CCmdCheck cmd, double* plfWindowData );
extern void vSetFilterFunction(CCmdCheck cmd, double* plfFilterReData, double *plfFilterImgData);

int main( int argc, char* argv[] )
{
	long lRet = 0L;
	CCmdCheck cmd;
	CCmdCheckException ccmde;

	char acWindow[256];
	char acFilter[256];
	char acInFileName[256];
	char acOutFileName[256];
	char acInDataType[256];
	char acOutDataType[256];

	//������
	memset( acWindow, 0, sizeof( acWindow ) );
	memset( acFilter, 0, sizeof(acFilter));
	memset( acInFileName, 0, sizeof( acInFileName ) );
	memset( acOutFileName, 0, sizeof( acOutFileName ) );
	memset( acInDataType, 0, sizeof( acInDataType ) );
	memset( acOutDataType, 0, sizeof( acOutDataType ) );

	try
	{
		// �R�}���h���C�����
		cmd.vCommandCheck( argc, argv );

		// �R�}���h���C������擾�����l�̐ݒ�
		cmd.vGetWindow( acWindow );
		cmd.vGetFilter( acFilter );
		cmd.vGetInFileName( acInFileName );
		cmd.vGetOutFileName( acOutFileName );
		cmd.vGetInDataType( acInDataType );
		cmd.vGetOutDataType( acOutDataType );
	
		// �G���[�`�F�b�N
		if( cmd.iGetFrame() <= 0 )
		{
			ccmde.SetErrorInfo( CCMD_ERROR_INVALID_FRAME, "CommandCheck", "CCmdCheck", "�s���ȃt���[����", __LINE__ );
			throw( ccmde );
		}
		if( cmd.iGetSampling() <= 0 )
		{
			ccmde.SetErrorInfo( CCMD_ERROR_INVALID_SAMPLING, "CommandCheck", "CCmdCheck", "�s���ȃT���v�����O���g��", __LINE__ );
			throw( ccmde );
		}
		if( cmd.lfGetTimeInter() <= 0 || cmd.lfGetTimeInter() > cmd.iGetFrame() )
		{
			ccmde.SetErrorInfo( CCMD_ERROR_INVALID_INTERVAL, "CommandCheck", "CCmdCheck", "�s���Ȏ��ԊԊu�l", __LINE__ );
			throw( ccmde );
		}
		if( cmd.iGetNormalizeFlag() != 0 && cmd.iGetNormalizeFlag() != 1 )
		{
			ccmde.SetErrorInfo( CCMD_ERROR_INVALID_NORMALIZE, "CommandCheck", "CCmdCheck", "�s���ȃm�[�}���C�Y�l", __LINE__ );
			throw( ccmde );
		}
		if( cmd.iGetWriteMode() < 0 && cmd.iGetWriteMode() > 5 )
		{
			ccmde.SetErrorInfo( CCMD_ERROR_INVALID_WRITEMODE, "CommandCheck", "CCmdCheck", "�s���ȏ����o�����[�h�l", __LINE__ );
			throw( ccmde );
		}
	
		// �y�N�g���O�������s
		vSpectrogram( cmd );
	}
	catch( CCmdCheckException cce )
	{
		printf("�G���[�ԍ��F%d\n�G���[�N���X���F%s\n�G���[���\�b�h���F%s\n�G���[�s���F%d\n�G���[�ڍׁF%s\n",
			   cce.iGetErrCode(), 
			   cce.strGetClassName().c_str(), 
			   cce.strGetMethodName().c_str(), 
			   cce.iGetErrLine(), 
			   cce.strGetErrDetail().c_str() );
	}
	catch( CCsvException csve )
	{
		printf( "�G���[�ԍ��F%d\n�G���[�N���X���F%s\n�G���[���\�b�h���F%s\n�G���[�s���F%d\n�G���[�ڍׁF%s\n",
			   csve.iGetErrCode(), 
			   csve.strGetClassName().c_str(), 
			   csve.strGetMethodName().c_str(), 
			   csve.iGetErrLine(), 
			   csve.strGetErrDetail().c_str() );
	}
	catch( std::bad_alloc ba )
	{
		printf("�������m�ۃG���[�F%s\n",ba.what() );
	}
	catch( std::bad_exception be )
	{
		printf("��������@�G���[�F%s\n",be.what() );
	}
	return lRet;
}

void vSpectrogram( CCmdCheck cmd )
{
	int i,j;
	long lDataLen;
	long lTotalLength;
	double lfMax = 0.0;
	double lfMin = 0.0;
	double *plfFreqX, *plfFreqY;
	double *plfOriginData;
	double *plfSinTable;
	double *plfWindowTable;
	double *plfFilterRe;
	double *plfFilterImg;
	int *piBitTable;
	double lfData;
	double lfNoiseLevel = -100;
	double lfRe, lfImg;
	char str[100];

	CCsv CsvRead, CsvWrite;
	CCmdCheckException ccmde;

	// �t�@�C���I�[�v��
	CsvRead.Open( cmd.pcGetInFileName(), "read" );
	CsvWrite.Open( cmd.pcGetOutFileName(), "write" );

	// ����͋�Ԃ����߂�B
	lDataLen = CsvRead.GetColumn();
	lTotalLength = (double)lDataLen;

	if( cmd.iGetFrame() > lTotalLength )
	{
		ccmde.SetErrorInfo( CCMD_ERROR_INVALID_FRAME, "CommandCheck", "CCmdCheck", "�s���ȃt���[����", __LINE__ );
		throw( ccmde );
	}

	try
	{
		memset( str, '\0', sizeof(str));
		plfFreqX = new double[cmd.iGetFrame()];
		plfFreqY = new double[cmd.iGetFrame()];
		plfSinTable = new double[cmd.iGetFrame()];
		piBitTable = new int[cmd.iGetFrame()];
		plfOriginData = new double[cmd.iGetFrame()];
		plfWindowTable = new double[cmd.iGetFrame()];
		plfFilterRe = new double[cmd.iGetFrame()];
		plfFilterImg = new double[cmd.iGetFrame()];

		for( i = 0 ;i < cmd.iGetFrame(); i++ )
		{
			plfFreqX[i] = 0.0;
			plfFreqY[i] = 0.0;
			plfSinTable[i] = 0.0;
			plfSinTable[i] = 0.0;
			piBitTable[i] = 0.0;
			plfOriginData[i] = 0.0;
			plfWindowTable[i] = 0.0;
			plfFilterRe[i] = 0.0;
			plfFilterImg[i] = 0.0;
		}
	}
	catch( std::bad_alloc ba )
	{
		throw( ba );
	}
	// �m�C�Y���x���̎擾�B
	lfNoiseLevel = cmd.lfGetNoiseLevel();

	// sin�e�[�u���쐬
	make_fft_sintable( cmd.iGetFrame(), plfSinTable );
	// �Q�ƃe�[�u���쐬
	make_bittable( cmd.iGetFrame(), piBitTable );
	// ���֐��p�e�[�u���̍쐬
	vSetWindowFunction( cmd, plfWindowTable );
	// �t�B���^�p�̃e�[�u���쐬
	vSetFilterFunction(cmd, plfFilterRe, plfFilterImg );

/* �f�[�^��͎��s */

	CsvRead.Read( plfOriginData, cmd.iGetFrame()-1, cmd.pcGetInDataType() );
	for( i = 0 ;i < lTotalLength; i += (int)cmd.lfGetTimeInter() )
	{
		// �t�@�C���̓ǂݍ���
		CsvRead.Read( &plfOriginData[cmd.iGetFrame()-1], 1, cmd.pcGetInDataType() );
		
		for( j = 0;j < cmd.iGetFrame(); j++ )
		{
			if( lTotalLength > i+j )
			{
				plfFreqX[j] = plfOriginData[j]*plfWindowTable[j];
			}
			else
			{
				plfFreqX[j] = plfOriginData[j] = 0.0;
			}
		}
		
		// �t�[���G�ϊ����s
		fft( cmd.iGetFrame(), plfSinTable, piBitTable, plfFreqX, plfFreqY );

		// �t�B���^��������iLPF,HPF,BPF,BEF,APF,LowShelf,HighShelf,PeakingEQ,PEQ,GEQ�j
		// �����Ƌ����̐ς��Z�o����B
		for (j = 0; j < cmd.iGetFrame(); j++)
		{
			lfRe = plfFreqX[j]*plfFreqY[j] - plfFilterRe[j]*plfFilterImg[j];
			lfImg = plfFreqX[j]*plfFilterImg[j] + plfFilterRe[j]*plfFreqY[j];
			plfFreqX[j] = lfRe;
			plfFreqY[j] = lfImg;
		}
		// ��Βl�̍ő�l���擾�B
		if( cmd.iGetNormalizeFlag() == 1 )
		{
			lfMax = search_max( cmd.iGetFrame(), plfFreqX, plfFreqY );
			lfMax = 1.0/lfMax;
		}
		else if( cmd.iGetNormalizeFlag() == 2 )
		{
			lfMin = search_max( cmd.iGetFrame(), plfFreqX, plfFreqY );
			lfMin = 1.0/lfMin;
		}
		else
		{
			lfMax = 1.0;
		}
		
		// ���ʏo��
		if( cmd.iGetWriteMode() == 1 )
		{
			// ���f���o��
			for( j = 0 ; j < cmd.iGetFrame() ; j++ )
			{
				if( plfFreqY[j] >= 0.0 )
				{
					sprintf(str, "%lf+%lfi",plfFreqX[j],plfFreqY[j]);
				}
				else
				{
					sprintf(str, "%lf%lfi",plfFreqX[j],plfFreqY[j]);
				}
				CsvWrite.Write( str, 1, "string" );
			}
		}
		// ���f�U���o��
		else if( cmd.iGetWriteMode() == 2 )
		{
			for( j = 0 ; j < cmd.iGetFrame() ; j++ )
			{
				lfData = sqrt(plfFreqX[j]*plfFreqX[j]+plfFreqY[j]*plfFreqY[j]);
				CsvWrite.Write( &lfData, 1, cmd.pcGetOutDataType() );
			}
		}
		// �G�l���M�[�X�y�N�g�����x�o��
		else if( cmd.iGetWriteMode() == 3 )
		{
			for( j = 0 ; j < cmd.iGetFrame() ; j++ )
			{
				lfData = (plfFreqX[j]*plfFreqX[j]+plfFreqY[j]*plfFreqY[j]);
				CsvWrite.Write( &lfData, 1, cmd.pcGetOutDataType() );
			}
		}
		// �p���[�X�y�N�g�����x�o��
		else if( cmd.iGetWriteMode() == 4 )
		{
			for( j = 0 ; j < cmd.iGetFrame() ; j++ )
			{
				lfData = (plfFreqX[j]*plfFreqX[j]+plfFreqY[j]*plfFreqY[j]);
				lfData /= ((double)cmd.iGetFrame()*cmd.iGetFrame());
				CsvWrite.Write( &lfData, 1, cmd.pcGetOutDataType() );
			}
		}
		// �G�l���M�[�X�y�N�g�����x�o��(dB�o�́A�dB��0�Ƃ���B)
		else if (cmd.iGetWriteMode() == 5)
		{
			for (j = 0; j < cmd.iGetFrame(); j++)
			{
				lfData = (plfFreqX[j] * plfFreqX[j] + plfFreqY[j] * plfFreqY[j]);
				lfData = 10 * log10(lfData);
				CsvWrite.Write(&lfData, 1, cmd.pcGetOutDataType());
			}
		}
		// �p���[�X�y�N�g�����x�o��(dB�o�́A�dB��0�Ƃ���B)
		else if (cmd.iGetWriteMode() == 6)
		{
			for (j = 0; j < cmd.iGetFrame(); j++)
			{
				lfData = (plfFreqX[j] * plfFreqX[j] + plfFreqY[j] * plfFreqY[j]);
				lfData /= ((double)cmd.iGetFrame()*cmd.iGetFrame());
				lfData = 10 * log10(lfData);
				CsvWrite.Write(&lfData, 1, cmd.pcGetOutDataType());
			}
		}
		// �G�l���M�[�X�y�N�g�����x�o��(dB�o��)
		else if (cmd.iGetWriteMode() == 7)
		{
			for (j = 0; j < cmd.iGetFrame(); j++)
			{
				lfData = (plfFreqX[j] * plfFreqX[j] + plfFreqY[j] * plfFreqY[j]);
				lfData = 10 * log10(lfData) + lfNoiseLevel;
				CsvWrite.Write(&lfData, 1, cmd.pcGetOutDataType());
			}
		}
		// �p���[�X�y�N�g�����x�o��(dB�o��)
		else if (cmd.iGetWriteMode() == 8)
		{
			for (j = 0; j < cmd.iGetFrame(); j++)
			{
				lfData = (plfFreqX[j] * plfFreqX[j] + plfFreqY[j] * plfFreqY[j]);
				lfData /= ((double)cmd.iGetFrame()*cmd.iGetFrame());
				lfData = 10 * log10(lfData) + lfNoiseLevel;
				CsvWrite.Write(&lfData, 1, cmd.pcGetOutDataType());
			}
		}
		// ���s��������
		CsvWrite.WriteEndLine( "windows" );
		
		// ������
		for( j = 0 ;j < cmd.iGetFrame()-1; j++ )
		{
			plfOriginData[j] = plfOriginData[j+1];
			plfFreqX[j] = 0.0;
			plfFreqY[j] = 0.0;
		}
		plfFreqX[cmd.iGetFrame()-1] = 0.0;
		plfFreqY[cmd.iGetFrame()-1] = 0.0;
		memset( str, '\0', sizeof(str) );
	}
	CsvRead.Close();
	CsvWrite.Close();
	
	try
	{
		delete[] plfFreqX;
		delete[] plfFreqY;
		delete[] plfSinTable;
		delete[] piBitTable;
		delete[] plfOriginData;
		delete[] plfWindowTable;
	}
	catch( ... )
	{
		std::bad_exception be;
		throw(be);
	}
}


void vSetWindowFunction( CCmdCheck cmd, double* plfWindowData )
{
	using namespace std;
	int i;
	int iWindowFlag;
	int iFrame;
	double lfParam;

	iWindowFlag = cmd.iGetWindowFlag();
	iFrame = cmd.iGetFrame();
	lfParam = cmd.lfGetParam()[0];

	for( i = 0;i < iFrame; i++ )
	{
		if( iWindowFlag == 0 )
		{
			plfWindowData[i] = 1.0;
		}
		else if( iWindowFlag == 1 )
		{
			plfWindowData[i] = bartlett<double>( i, iFrame );
		}
		else if( iWindowFlag == 2 )
		{
			plfWindowData[i] = hanning<double>( i, iFrame );
		}
		else if( iWindowFlag == 3 )
		{
			plfWindowData[i] = hamming<double>( i, iFrame );
		}
		else if( iWindowFlag == 4 )
		{
			plfWindowData[i] = blackman<double>( i, iFrame );
		}
		else if( iWindowFlag == 5 )
		{
			plfWindowData[i] = blackman_hann<double>( i, iFrame );
		}
		else if( iWindowFlag == 6 )
		{
			plfWindowData[i] = general_hamming<double>( i, iFrame, lfParam );
		}
		else if( iWindowFlag == 7 )
		{
			plfWindowData[i] = kaiser<double>( i, iFrame, lfParam );
		}
		else if( iWindowFlag == 8 )
		{
			plfWindowData[i] = gaussian<double>( i, iFrame, lfParam );
		}
		else if( iWindowFlag == 9 )
		{
			plfWindowData[i] = exponential<double>( i, iFrame, lfParam );
		}
		else if( iWindowFlag == 10 )
		{
			plfWindowData[i] = flat_top<double>( i, iFrame );
		}
		else if( iWindowFlag == 11 )
		{
			plfWindowData[i] = blackman_nuttall<double>( i, iFrame );
		}
		else if( iWindowFlag == 12 )
		{
			plfWindowData[i] = parzen<double>( i, iFrame );
		}
		else if( iWindowFlag == 13 )
		{
			plfWindowData[i] = welch<double>( i, iFrame );
		}
		else if( iWindowFlag == 14 )
		{
			plfWindowData[i] = blackman_harris<double>( i, iFrame );
		}
		else if( iWindowFlag == 15 )
		{
			plfWindowData[i] = akaike<double>( i, iFrame );
		}
	}
}

void vSetFilterFunction(CCmdCheck cmd, double* plfFilterReData, double *plfFilterImgData )
{
	int i;
	int iFilterFlag;
	int iFrame;
	double lfParam;
	double lfSf, lfFp, lfFs;
	double lfFpl, lfFph, lfFsl, lfFsh;
	double lfAp, lfAs, lfAb;

	iFilterFlag = cmd.iGetFilterFlag();
	iFrame = cmd.iGetFrame();

	for (i = 0; i < iFrame; i++)
	{
		// �t�B���^�������Ȃ��B
		if (iFilterFlag == 0)
		{
			plfFilterReData[i] = 1.0;
			plfFilterImgData[i] = 0.0;
		}
		// ���[�p�X�t�B���^
		else if (iFilterFlag == 1)
		{
			lfSf = cmd.iGetSampling();	// �T���v�����O���g��
			lfFp = cmd.lfGetParam()[0];	// �ʉ߈���g���iLPF�̎��g���j
			lfFs = lfFp*1.5;			// �Ւf����g��
			lfAp = 0.1;					// �ʉ߈�U���l�i1-�̌`���j
			lfAs = 0.3;					// �Ւf��U���l (���̂܂�)
			lpf( eChebyshev, lfFp, lfFs, lfAp, lfAs, iFrame, lfSf, plfFilterReData, plfFilterImgData );
		}
		// �n�C�p�X�t�B���^
		else if (iFilterFlag == 2)
		{
			lfSf = cmd.iGetSampling();	// �T���v�����O���g��
			lfFs = cmd.lfGetParam()[0];	// �Ւf����g���iHPF�̎��g���j
			lfFp = lfFs*1.5;			// �ʉ߈���g��
			lfAp = 0.2;					// �ʉ߈�U���l�i1-�̌`���j
			lfAs = 0.1;					// �Ւf��U���l (���̂܂�)
			hpf(eChebyshev, lfFp, lfFs, lfAp, lfAs, iFrame, lfSf, plfFilterReData, plfFilterImgData);
		}
		// �o���h�p�X�t�B���^
		else if (iFilterFlag == 3)
		{
			lfSf = cmd.iGetSampling();	// �T���v�����O���g��
			lfFpl = cmd.lfGetParam()[0];// �ʉ߈���g���iBPF�̎n�ߎ��g���j
			lfFph = cmd.lfGetParam()[1];// �ʉ߈���g���iBPF�̏I�����g���j
			lfFsl = lfFsl*0.5;			// �Ւf����g�� 
			lfFsh = lfFsh*1.5;			// �Ւf����g��
			lfAp = 0.2;					// �ʉ߈�U���l�i1-�̌`���j
			lfAs = 0.1;					// �Ւf��U���l (���̂܂�)
			bpf(eChebyshev, lfFpl, lfFsl, lfFph, lfFsh, lfAp, lfAs, iFrame, lfSf, plfFilterReData, plfFilterImgData);
		}
		// �o���h�G���~�l�C�g�t�B���^(�m�b�`�t�B���^)
		else if (iFilterFlag == 4)
		{
			lfSf = cmd.iGetSampling();	// �T���v�����O���g��
			lfFsl = cmd.lfGetParam()[0];// �Ւf����g���iBEF�̎n�ߎ��g���j
			lfFsh = cmd.lfGetParam()[1];// �Ւf����g���iBEF�̏I�����g���j
			lfFpl = lfFsl*0.5;			// �ʉ߈���g�� 
			lfFph = lfFsh*1.5;			// �ʉ߈���g��
			lfAp = 0.2;					// �ʉ߈�U���l�i1-�̌`���j
			lfAs = 0.1;					// �Ւf��U���l (���̂܂�)
			bef(eChebyshev, lfFpl, lfFsl, lfFph, lfFsh, lfAp, lfAs, iFrame, lfSf, plfFilterReData, plfFilterImgData);
		}
		// �I�[���p�X�t�B���^
		else if (iFilterFlag == 5)
		{
//			plfFilterData[i] = <double>(i, iFrame);
		}
#if 0
		// ���[�V�F���t
		else if (iFilterFlag == 6)
		{
			plfFilterData[i] = lowshelf<double>(i, iFrame, lfParam);
		}
		// �n�C�V�F���t
		else if (iFilterFlag == 7)
		{
			plfFilterData[i] = kaiser<double>(i, iFrame, lfParam);
		}
		// �s�[�L���OEQ
		else if (iFilterFlag == 8)
		{
			plfFilterData[i] = gaussian<double>(i, iFrame, lfParam);
		}
		// �p�����g���b�N�C�R���C�U
		else if (iFilterFlag == 9)
		{
			plfFilterData[i] = exponential<double>(i, iFrame, lfParam);
		}
		// GEQ
		else if (iFilterFlag == 10)
		{
			plfFilterData[i] = flat_top<double>(i, iFrame);
		}
#endif
	}
}