
#include<cstdio>
#include<cmath>
#include"./include/integral.hpp"
#include "corr.h"

/**
 * <PRE>
 * 自己相関関数を理論通りに実行する。
 * </PRE>
 * @author kobayashi
 * @param frame		フレーム幅
 * @param data		波形
 * @param corr		自己相関関数
 * @version 0.1
 */
void auto_correlation( const int &frame, const double *data, double *corr )
{
	int i,j;
	double result;
//	double inv_frame = 1.0/(double)frame;
	for(i = 0;i < frame;i++)
	{
		result = 0.0;
		for(j = i;j < frame;j++)
			result += data[j]*data[j-i];
		corr[i]	= result;
	}
}

/**
 * <PRE>
 * 自己相関関数を理論通りに実行する。
 * </PRE>
 * @author kobayashi
 * @param frame		フレーム幅
 * @param data1		波形1
 * @param data2		波形2
 * @param corr		相互相関関数
 * @version 0.1
 */
void cross_correlation( const int &frame, const double *data1, const double *data2, double *corr )
{
	int i,j;
	double result;
//	double inv_frame = 1.0/(double)frame;
	for(i = 0;i < frame;i++)
	{
		result = 0.0;
		for(j = i;j < frame;j++)
			result += data1[j]*data2[j-i];
		corr[i] = result;
	}
}

/**
 * <PRE>
 * 線形予測残差による相関関数
 * </PRE>
 * @author kobayashi
 * @param frame		フレーム幅
 * @param data		波形
 * @param corr		相互相関関数
 * @version 0.1
 */
void corr_diff_lpc( const int &frame, const double *data, double *corr )
{
	return ;
}

/**
 * <PRE>
 * Prony法により推測した波形ともとの波形との残差の相関関数
 * </PRE>
 * @author kobayashi
 * @param frame		フレーム幅
 * @param data		波形
 * @param corr		相互相関関数
 * @version 0.1
 */
void corr_diff_prony( const int &frame, const double *data, double *corr )
{
	return ;
}
