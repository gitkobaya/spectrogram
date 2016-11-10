#include <cstdio>
#include <string>
#include <iostream>
#include "fft.h"
#include "windowfunc.h"
#include "search_maxmin.h"
#include "cmd_check.h"
#include "csv.h"
#include "iir.hpp"

// グローバル変数


// 関数プロトタイプ宣言
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

	//初期化
	memset( acWindow, 0, sizeof( acWindow ) );
	memset( acFilter, 0, sizeof(acFilter));
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
		cmd.vGetFilter( acFilter );
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
	double *plfFilterRe;
	double *plfFilterImg;
	int *piBitTable;
	double lfData;
	double lfNoiseLevel = -100;
	double lfRe, lfImg;
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
	// ノイズレベルの取得。
	lfNoiseLevel = cmd.lfGetNoiseLevel();

	// sinテーブル作成
	make_fft_sintable( cmd.iGetFrame(), plfSinTable );
	// 参照テーブル作成
	make_bittable( cmd.iGetFrame(), piBitTable );
	// 窓関数用テーブルの作成
	vSetWindowFunction( cmd, plfWindowTable );
	// フィルタ用のテーブル作成
	vSetFilterFunction(cmd, plfFilterRe, plfFilterImg );

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

		// フィルタをかける（LPF,HPF,BPF,BEF,APF,LowShelf,HighShelf,PeakingEQ,PEQ,GEQ）
		// 実部と虚部の積を算出する。
		for (j = 0; j < cmd.iGetFrame(); j++)
		{
			lfRe = plfFreqX[j]*plfFreqY[j] - plfFilterRe[j]*plfFilterImg[j];
			lfImg = plfFreqX[j]*plfFilterImg[j] + plfFilterRe[j]*plfFreqY[j];
			plfFreqX[j] = lfRe;
			plfFreqY[j] = lfImg;
		}
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
		// フィルタをかけない。
		if (iFilterFlag == 0)
		{
			plfFilterReData[i] = 1.0;
			plfFilterImgData[i] = 0.0;
		}
		// ローパスフィルタ
		else if (iFilterFlag == 1)
		{
			lfSf = cmd.iGetSampling();	// サンプリング周波数
			lfFp = cmd.lfGetParam()[0];	// 通過域周波数（LPFの周波数）
			lfFs = lfFp*1.5;			// 遮断域周波数
			lfAp = 0.1;					// 通過域振幅値（1-の形式）
			lfAs = 0.3;					// 遮断域振幅値 (そのまま)
			lpf( eChebyshev, lfFp, lfFs, lfAp, lfAs, iFrame, lfSf, plfFilterReData, plfFilterImgData );
		}
		// ハイパスフィルタ
		else if (iFilterFlag == 2)
		{
			lfSf = cmd.iGetSampling();	// サンプリング周波数
			lfFs = cmd.lfGetParam()[0];	// 遮断域周波数（HPFの周波数）
			lfFp = lfFs*1.5;			// 通過域周波数
			lfAp = 0.2;					// 通過域振幅値（1-の形式）
			lfAs = 0.1;					// 遮断域振幅値 (そのまま)
			hpf(eChebyshev, lfFp, lfFs, lfAp, lfAs, iFrame, lfSf, plfFilterReData, plfFilterImgData);
		}
		// バンドパスフィルタ
		else if (iFilterFlag == 3)
		{
			lfSf = cmd.iGetSampling();	// サンプリング周波数
			lfFpl = cmd.lfGetParam()[0];// 通過域周波数（BPFの始め周波数）
			lfFph = cmd.lfGetParam()[1];// 通過域周波数（BPFの終わり周波数）
			lfFsl = lfFsl*0.5;			// 遮断域周波数 
			lfFsh = lfFsh*1.5;			// 遮断域周波数
			lfAp = 0.2;					// 通過域振幅値（1-の形式）
			lfAs = 0.1;					// 遮断域振幅値 (そのまま)
			bpf(eChebyshev, lfFpl, lfFsl, lfFph, lfFsh, lfAp, lfAs, iFrame, lfSf, plfFilterReData, plfFilterImgData);
		}
		// バンドエリミネイトフィルタ(ノッチフィルタ)
		else if (iFilterFlag == 4)
		{
			lfSf = cmd.iGetSampling();	// サンプリング周波数
			lfFsl = cmd.lfGetParam()[0];// 遮断域周波数（BEFの始め周波数）
			lfFsh = cmd.lfGetParam()[1];// 遮断域周波数（BEFの終わり周波数）
			lfFpl = lfFsl*0.5;			// 通過域周波数 
			lfFph = lfFsh*1.5;			// 通過域周波数
			lfAp = 0.2;					// 通過域振幅値（1-の形式）
			lfAs = 0.1;					// 遮断域振幅値 (そのまま)
			bef(eChebyshev, lfFpl, lfFsl, lfFph, lfFsh, lfAp, lfAs, iFrame, lfSf, plfFilterReData, plfFilterImgData);
		}
		// オールパスフィルタ
		else if (iFilterFlag == 5)
		{
//			plfFilterData[i] = <double>(i, iFrame);
		}
#if 0
		// ローシェルフ
		else if (iFilterFlag == 6)
		{
			plfFilterData[i] = lowshelf<double>(i, iFrame, lfParam);
		}
		// ハイシェルフ
		else if (iFilterFlag == 7)
		{
			plfFilterData[i] = kaiser<double>(i, iFrame, lfParam);
		}
		// ピーキングEQ
		else if (iFilterFlag == 8)
		{
			plfFilterData[i] = gaussian<double>(i, iFrame, lfParam);
		}
		// パラメトリックイコライザ
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