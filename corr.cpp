
#include<cstdio>
#include<cmath>
#include"./include/integral.hpp"
#include "corr.h"

/**
 * <PRE>
 * ���ȑ��֊֐��𗝘_�ʂ�Ɏ��s����B
 * </PRE>
 * @author kobayashi
 * @param frame		�t���[����
 * @param data		�g�`
 * @param corr		���ȑ��֊֐�
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
 * ���ȑ��֊֐��𗝘_�ʂ�Ɏ��s����B
 * </PRE>
 * @author kobayashi
 * @param frame		�t���[����
 * @param data1		�g�`1
 * @param data2		�g�`2
 * @param corr		���ݑ��֊֐�
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
 * ���`�\���c���ɂ�鑊�֊֐�
 * </PRE>
 * @author kobayashi
 * @param frame		�t���[����
 * @param data		�g�`
 * @param corr		���ݑ��֊֐�
 * @version 0.1
 */
void corr_diff_lpc( const int &frame, const double *data, double *corr )
{
	return ;
}

/**
 * <PRE>
 * Prony�@�ɂ�萄�������g�`�Ƃ��Ƃ̔g�`�Ƃ̎c���̑��֊֐�
 * </PRE>
 * @author kobayashi
 * @param frame		�t���[����
 * @param data		�g�`
 * @param corr		���ݑ��֊֐�
 * @version 0.1
 */
void corr_diff_prony( const int &frame, const double *data, double *corr )
{
	return ;
}
