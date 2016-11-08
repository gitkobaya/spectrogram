#include <cstdio>
#include <string>
#include <iostream>
#include "fft.h"
#include "windowfunc.h"
#include "search_maxmin.h"
#include "cmd_check.h"
#include "csv.h"

// グローバル変数


// 関数プロトタイプ宣言
extern void vSpectrogram( CCmdCheck cmd );
extern void vSetWindowFunction( CCmdCheck cmd, double* plfWindowData );

int main( int argc, char* argv[] )
{
	long lRet = 0L;
	CCmdCheck cmd;
	CCmdCheckException ccmde;

	char acWindow[256];
	char acInFileName[256];
	char acOutFileName[256];
	char acInDataType[256];
	char acOutDataType[256];

	//初期化
	memset( acWindow, 0, sizeof( acWindow ) );
	memset( acInFileName, 0, sizeof( acInFileName ) );
	memset( acOutFileName, 0, sizeof( acOutFileName ) );
	memset( acInDataType, 0, sizeof( acInDataType ) );
	memset( acOutDataType, 0, sizeof( acOutDataType ) );

	try
	{
		// コマンドライン解析
		cmd.vCommandCheck( argc, argv );

		// コマンドラインから取得した値の設定
		cmd.vGetWindow( acWindow );
		cmd.vGetInFileName( acInFileName );
		cmd.vGetOutFileName( acOutFileName );
		cmd.vGetInDataType( acInDataType );
		cmd.vGetOutDataType( acOutDataType );
	
		// エラーチェック
		if( cmd.iGetFrame() <= 0 )
		{
			ccmde.SetErrorInfo( CCMD_ERROR_INVALID_FRAME, "CommandCheck", "CCmdCheck", "不正なフレーム数", __LINE__ );
			throw( ccmde );
		}
		if( cmd.iGetSampling() <= 0 )
		{
			ccmde.SetErrorInfo( CCMD_ERROR_INVALID_SAMPLING, "CommandCheck", "CCmdCheck", "不正なサンプリング周波数", __LINE__ );
			throw( ccmde );
		}
		if( cmd.lfGetTimeInter() <= 0 || cmd.lfGetTimeInter() > cmd.iGetFrame() )
		{
			ccmde.SetErrorInfo( CCMD_ERROR_INVALID_INTERVAL, "CommandCheck", "CCmdCheck", "不正な時間間隔値", __LINE__ );
			throw( ccmde );
		}
		if( cmd.iGetNormalizeFlag() != 0 && cmd.iGetNormalizeFlag() != 1 )
		{
			ccmde.SetErrorInfo( CCMD_ERROR_INVALID_NORMALIZE, "CommandCheck", "CCmdCheck", "不正なノーマライズ値", __LINE__ );
			throw( ccmde );
		}
		if( cmd.iGetWriteMode() < 0 && cmd.iGetWriteMode() > 5 )
		{
			ccmde.SetErrorInfo( CCMD_ERROR_INVALID_WRITEMODE, "CommandCheck", "CCmdCheck", "不正な書き出しモード値", __LINE__ );
			throw( ccmde );
		}
	
		// ペクトログラム実行
		vSpectrogram( cmd );
	}
	catch( CCmdCheckException cce )
	{
		printf("エラー番号：%d\nエラークラス名：%s\nエラーメソッド名：%s\nエラー行数：%d\nエラー詳細：%s\n",
			   cce.iGetErrCode(), 
			   cce.strGetClassName().c_str(), 
			   cce.strGetMethodName().c_str(), 
			   cce.iGetErrLine(), 
			   cce.strGetErrDetail().c_str() );
	}
	catch( CCsvException csve )
	{
		printf( "エラー番号：%d\nエラークラス名：%s\nエラーメソッド名：%s\nエラー行数：%d\nエラー詳細：%s\n",
			   csve.iGetErrCode(), 
			   csve.strGetClassName().c_str(), 
			   csve.strGetMethodName().c_str(), 
			   csve.iGetErrLine(), 
			   csve.strGetErrDetail().c_str() );
	}
	catch( std::bad_alloc ba )
	{
		printf("メモリ確保エラー：%s\n",ba.what() );
	}
	catch( std::bad_exception be )
	{
		printf("メモリ解法エラー：%s\n",be.what() );
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
	int *piBitTable;
	double lfData;
	double lfNoiseLevel = -100;
	char str[100];

	CCsv CsvRead, CsvWrite;
	CCmdCheckException ccmde;

	// ファイルオープン
	CsvRead.Open( cmd.pcGetInFileName(), "read" );
	CsvWrite.Open( cmd.pcGetOutFileName(), "write" );

	// 総解析区間を求める。
	lDataLen = CsvRead.GetColumn();
	lTotalLength = (double)lDataLen;

	if( cmd.iGetFrame() > lTotalLength )
	{
		ccmde.SetErrorInfo( CCMD_ERROR_INVALID_FRAME, "CommandCheck", "CCmdCheck", "不正なフレーム数", __LINE__ );
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

		for( i = 0 ;i < cmd.iGetFrame(); i++ )
		{
			plfFreqX[i] = 0.0;
			plfFreqY[i] = 0.0;
			plfSinTable[i] = 0.0;
			plfSinTable[i] = 0.0;
			piBitTable[i] = 0.0;
			plfOriginData[i] = 0.0;
			plfWindowTable[i] = 0.0;
		}
	}
	catch( std::bad_alloc ba )
	{
		throw( ba );
	}
	// ノイズレベルの取得。
	lfNoiseLevel = cmd.lfGetNoiseLevel();

	// sinテーブル作成
	make_fft_sintable( cmd.iGetFrame(), plfSinTable );
	// 参照テーブル作成
	make_bittable( cmd.iGetFrame(), piBitTable );
	// 窓関数用テーブルの作成
	vSetWindowFunction( cmd, plfWindowTable );

/* データ解析実行 */

	CsvRead.Read( plfOriginData, cmd.iGetFrame()-1, cmd.pcGetInDataType() );
	for( i = 0 ;i < lTotalLength; i += (int)cmd.lfGetTimeInter() )
	{
		// ファイルの読み込み
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
		
		// フーリエ変換実行
		fft( cmd.iGetFrame(), plfSinTable, piBitTable, plfFreqX, plfFreqY );
		
		// 絶対値の最大値を取得。
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
		
		// 結果出力
		if( cmd.iGetWriteMode() == 1 )
		{
			// 複素数出力
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
		// 複素振幅出力
		else if( cmd.iGetWriteMode() == 2 )
		{
			for( j = 0 ; j < cmd.iGetFrame() ; j++ )
			{
				lfData = sqrt(plfFreqX[j]*plfFreqX[j]+plfFreqY[j]*plfFreqY[j]);
				CsvWrite.Write( &lfData, 1, cmd.pcGetOutDataType() );
			}
		}
		// エネルギースペクトル密度出力
		else if( cmd.iGetWriteMode() == 3 )
		{
			for( j = 0 ; j < cmd.iGetFrame() ; j++ )
			{
				lfData = (plfFreqX[j]*plfFreqX[j]+plfFreqY[j]*plfFreqY[j]);
				CsvWrite.Write( &lfData, 1, cmd.pcGetOutDataType() );
			}
		}
		// パワースペクトル密度出力
		else if( cmd.iGetWriteMode() == 4 )
		{
			for( j = 0 ; j < cmd.iGetFrame() ; j++ )
			{
				lfData = (plfFreqX[j]*plfFreqX[j]+plfFreqY[j]*plfFreqY[j]);
				lfData /= ((double)cmd.iGetFrame()*cmd.iGetFrame());
				CsvWrite.Write( &lfData, 1, cmd.pcGetOutDataType() );
			}
		}
		// エネルギースペクトル密度出力(dB出力、基準dBは0とする。)
		else if (cmd.iGetWriteMode() == 5)
		{
			for (j = 0; j < cmd.iGetFrame(); j++)
			{
				lfData = (plfFreqX[j] * plfFreqX[j] + plfFreqY[j] * plfFreqY[j]);
				lfData = 10 * log10(lfData);
				CsvWrite.Write(&lfData, 1, cmd.pcGetOutDataType());
			}
		}
		// パワースペクトル密度出力(dB出力、基準dBは0とする。)
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
		// エネルギースペクトル密度出力(dB出力)
		else if (cmd.iGetWriteMode() == 7)
		{
			for (j = 0; j < cmd.iGetFrame(); j++)
			{
				lfData = (plfFreqX[j] * plfFreqX[j] + plfFreqY[j] * plfFreqY[j]);
				lfData = 10 * log10(lfData) + lfNoiseLevel;
				CsvWrite.Write(&lfData, 1, cmd.pcGetOutDataType());
			}
		}
		// パワースペクトル密度出力(dB出力)
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
		// 改行書き込み
		CsvWrite.WriteEndLine( "windows" );
		
		// 初期化
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