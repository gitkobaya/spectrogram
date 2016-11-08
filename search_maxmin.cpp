#include<cmath>
#include<cstdio>
#include"search_maxmin.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////極大,極小,最大,最小値を求める関数

typedef struct FreqInfo_t
{
	double spectrum;	//パワー情報
	int    freq;		//周波数情報
}FreqInfo_t;

/**
 * <PRE>
 * データ交換関数
 * </PRE>
 * @param swap1	交換元	
 * @param swap2	交換先	
 * @author kobayashi
 * @since 2010.02.08
 * @version 0.1
 */
void swap( FreqInfo_t *swap1, FreqInfo_t *swap2 )
{
	FreqInfo_t temp;
	temp = *swap1;
	*swap1 = *swap2;
	*swap2 = temp;
}

/**
 * <PRE>
 * クイックソートをおこなう関数
 * </PRE>
 * @param start		開始位置
 * @param end		終了位置
 * @param qsort1	ソートするデータ
 * @author kobayashi
 * @since 2010.02.08
 * @version 0.1
 */
void qsort( int start, int end, FreqInfo_t *qsort1 )
{
	int i = 0;
	int j = 0;
	int pibo = 0;
	double pibovalue = 0;
	i = start;
	j = end;
	pibo = (i + j) / 2;
	pibovalue = qsort1[pibo].spectrum;

	while(1)
	{
		while(qsort1[i].spectrum > pibovalue) i++;
		while(pibovalue > qsort1[j].spectrum) j--;
		if(i >= j) break;
		swap(&qsort1[i],&qsort1[j]);
		i++;
		j--;
	}
	if(start < i - 1) qsort(start,i - 1,qsort1);
	if(j + 1 < end)	qsort(j + 1,end,qsort1);
}

/**
 * <PRE>
 * パワー値の最大値を求める
 * </PRE>
 * @param frame		フレーム幅
 * @param freq_x	スペクトルの実部
 * @param freq_y	スペクトルの虚部
 * @author kobayashi
 * @since 2010.02.08
 * @version 0.1
 */
double search_max( const int &frame, double *freq_x, double *freq_y )
{
	int i;
	int half_frame = frame>>1;
	double comp1,comp2;

	comp1 = (freq_x[0]*freq_x[0]+freq_y[0]*freq_y[0]);
	for( i=1 ; i < half_frame ; i++ )
	{
		comp2 = freq_x[i]*freq_x[i]+freq_y[i]*freq_y[i];
		comp1 = comp1 < comp2 ? comp2 : comp1;
	}
	return comp1;
}

/**
 * <PRE>
 * パワー値の最小値を求める
 * </PRE>
 * @param frame		フレーム幅
 * @param freq_x	スペクトルの実部
 * @param freq_y	スペクトルの虚部
 * @author kobayashi
 * @since 2010.02.08
 * @version 0.1
 */
double search_min( const int &frame, double *freq_x, double *freq_y )
{
	int i;
	int half_frame = frame>>1;
	double comp1,comp2;

	comp1 = (freq_x[0]*freq_x[0]+freq_y[0]*freq_y[0]);
	for( i=1 ; i < half_frame ; i++ )
	{
		comp2 = freq_x[i]*freq_x[i]+freq_y[i]*freq_y[i];
		comp1 = comp1 > comp2 ? comp2 : comp1;
	}
	return comp1;
}

/**
 * <PRE>
 * パワー値の最大最小値を求める
 * </PRE>
 * @param frame		フレーム幅
 * @param freq_x	スペクトルの実部
 * @param freq_y	スペクトルの虚部
 * @param max_data	パワー最大値
 * @param min_data	パワー最小値
 * @author kobayashi
 * @since 2010.02.08
 * @version 0.1
 */
void search_maxmin( const int &frame, double *freq_x, double *freq_y, double *max_data, double *min_data )
{
	int i;
	int half_frame = frame>>1;
	double comp2;

	*max_data = (freq_x[0]*freq_x[0]+freq_y[0]*freq_y[0]);
	*min_data = *max_data;
	for( i=1 ; i < half_frame ; i++ )
	{
		comp2 = freq_x[i]*freq_x[i]+freq_y[i]*freq_y[i];
		*max_data = *max_data < comp2 ? comp2 : *max_data;
		*min_data = *min_data > comp2 ? comp2 : *min_data;
	}
	return ;
}

/**
 * <PRE>
 * 位相の最大値を求める
 * </PRE>
 * @param frame		フレーム幅
 * @param freq_x	スペクトルの実部
 * @param freq_y	スペクトルの虚部
 * @author kobayashi
 * @since 2010.02.08
 * @version 0.1
 */
double search_phase_max( const int &frame, double *freq_x, double *freq_y )
{
	int i;
	int half_frame = frame>>1;
	double comp1,comp2;

	comp1 = atan2( freq_y[0], freq_x[0] );
	for( i=1 ; i < half_frame ; i++ )
	{
		comp2 = atan2( freq_y[i], freq_x[i] );
		comp1 = comp1 < comp2 ? comp2 : comp1;
	}
	return comp1;
}

/**
 * <PRE>
 * 位相の最小値を求める
 * </PRE>
 * @param frame		フレーム幅
 * @param freq_x	スペクトルの実部
 * @param freq_y	スペクトルの虚部
 * @author kobayashi
 * @since 2010.02.08
 * @version 0.1
 */
double search_phase_min( const int &frame, double *freq_x, double *freq_y )
{
	int i;
	int half_frame = frame>>1;
	double comp1,comp2;

	comp1 = atan2( freq_y[0], freq_x[0] );
	for( i=1 ; i < half_frame ; i++ )
	{
		comp2 = atan2( freq_y[i], freq_x[i] );
		comp1 = comp1 > comp2 ? comp2 : comp1;
	}
	return comp1;
}

/**
 * <PRE>
 * 位相の最大最小値を求める
 * </PRE>
 * @param frame		フレーム幅
 * @param freq_x	スペクトルの実部
 * @param freq_y	スペクトルの虚部
 * @param max_data	位相最大値
 * @param min_data	位相最小値
 * @author kobayashi
 * @since 2010.02.08
 * @version 0.1
 */
void search_phase_maxmin( const int &frame, double *freq_x, double *freq_y, double *max_data, double *min_data )
{
	int i;
	int half_frame = frame>>1;
	double comp2;

	*max_data = atan2( freq_y[0], freq_x[0] );
	*min_data = *max_data;
	for( i=1 ; i < half_frame ; i++ )
	{
		comp2 = atan2( freq_y[i], freq_x[i] );
		*max_data = *max_data < comp2 ? comp2 : *max_data;
		*min_data = *min_data > comp2 ? comp2 : *min_data;
	}
	return ;
}

/**
 * <PRE>
 * パワーの極大値を求める
 * </PRE>
 * @param frame		フレーム幅
 * @param sampling	サンプリング周波数
 * @param peak		取得するピーク数
 * @param freq_x	スペクトルの実部
 * @param freq_y	スペクトルの虚部
 * @param peak_freq	ピークの周波数格納配列
 * @author kobayashi
 * @since 2010.02.08
 * @version 0.1
 */
void search_peak( const int &frame, const int &sampling, const int &peak, double *freq_x, double *freq_y, double *peak_freq )
{
	int i,j;
	FreqInfo_t	tFreqInfo[256];
	FreqInfo_t	temp;
	double	formant1,formant2,formant3;
	int		peak1;
	int		formant_num = 0;
	int		frame1 = frame-1;
	int		half_frame = frame>>1;

	//ピークの部分を抽出する.
	formant1 = (freq_x[0]*freq_x[0]+freq_y[0]*freq_y[0]);
	formant2 = (freq_x[1]*freq_x[1]+freq_y[1]*freq_y[1]);
	formant3 = (freq_x[2]*freq_x[2]+freq_y[2]*freq_y[2]);
	for(i = 3;i < frame;i++)
	{
		formant1 = formant2;
		formant2 = formant3;
		formant3 = (freq_x[i]*freq_x[i]+freq_y[i]*freq_y[i]);
		if(formant1 < formant2 && formant3 < formant2)
		{
			tFreqInfo[formant_num].spectrum = formant2;
			tFreqInfo[formant_num].freq     = i-1;
			formant_num++;
		}
	}
	//周波数順に並べなおす.
	peak1 = formant_num < peak ? formant_num : peak;
	for(j = 0;j < peak1;j++)
	{
		for(i = j+1;i < peak1;i++)
		{
			if(tFreqInfo[j].freq > tFreqInfo[i].freq)
			{
				temp = tFreqInfo[j];
				tFreqInfo[j] = tFreqInfo[i];
				tFreqInfo[i] = temp;
			}
		}
	}
	//周波数値を取り出す.
	if(peak_freq != NULL)
	{
		for(i = 0;i < peak1;i++)
			peak_freq[i] = tFreqInfo[i].freq*(double)sampling/(double)frame;
	}
	//ピークのみを出力(パワーデータをピーク以外0にする.)
	else
	{
		j = 0;
		for(i = 0;i < half_frame;i++)
		{
			if(tFreqInfo[j].freq == i) 
			{
				j++;
				if(j == peak) break;
			}
			else freq_x[i] = freq_y[i] = 0.0;
		}
		for(;i < frame;i++)
			freq_x[i] = freq_y[i] = 0.0;
	}
}

/**
 * <PRE>
 * パワースペクトルの極小値を求める
 * </PRE>
 * @param frame		フレーム幅
 * @param sampling	サンプリング周波数
 * @param peak		取得するピーク数
 * @param freq_x	スペクトルの実部
 * @param freq_y	スペクトルの虚部
 * @param peak_freq	ピークの周波数格納配列
 * @author kobayashi
 * @since 2010.02.08
 * @version 0.1
 */
void search_minimum( const int &frame, const int &sampling, const int &peak, double *freq_x, double *freq_y, double *peak_freq )
{
	int i,j;
	FreqInfo_t	tFreqInfo[256];
	FreqInfo_t	temp;
	double	formant1,formant2,formant3;
	int		peak1;
	int		formant_num = 0;
	int		frame1 = frame-1;
	int		half_frame = frame>>1;

	//極小値の部分を抽出する.
	formant1 = (freq_x[0]*freq_x[0]+freq_y[0]*freq_y[0]);
	formant2 = (freq_x[1]*freq_x[1]+freq_y[1]*freq_y[1]);
	formant3 = (freq_x[2]*freq_x[2]+freq_y[2]*freq_y[2]);
	for(i = 3;i < frame;i++)
	{
		formant1 = formant2;
		formant2 = formant3;
		formant3 = (freq_x[i]*freq_x[i]+freq_y[i]*freq_y[i]);
		if(formant1 < formant2 && formant3 < formant2)
		{
			tFreqInfo[formant_num].spectrum = formant2;
			tFreqInfo[formant_num].freq     = i-1;
			formant_num++;
		}
	}
	//周波数順に並べなおす.
	peak1 = formant_num < peak ? formant_num : peak;
	for(j = 0;j < peak1;j++)
	{
		for(i = j+1;i < peak1;i++)
		{
			if(tFreqInfo[j].freq > tFreqInfo[i].freq)
			{
				temp = tFreqInfo[j];
				tFreqInfo[j] = tFreqInfo[i];
				tFreqInfo[i] = temp;
			}
		}
	}
	//周波数値を取り出す.
	if(peak_freq != NULL)
	{
		for(i = 0;i < peak1;i++)
			peak_freq[i] = tFreqInfo[i].freq*(double)sampling/(double)frame;
	}
	//極小値のみを取り出す(パワーデータをピーク以外0にする.)
	else
	{
		j = 0;
		for(i = 0;i < half_frame;i++)
		{
			if(tFreqInfo[j].freq == i) 
			{
				j++;
				if(j == peak) break;
			}
			else freq_x[i] = freq_y[i] = 0.0;
		}
		for(;i < frame;i++)
			freq_x[i] = freq_y[i] = 0.0;
	}
}
