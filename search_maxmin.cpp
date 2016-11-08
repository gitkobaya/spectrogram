#include<cmath>
#include<cstdio>
#include"search_maxmin.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////�ɑ�,�ɏ�,�ő�,�ŏ��l�����߂�֐�

typedef struct FreqInfo_t
{
	double spectrum;	//�p���[���
	int    freq;		//���g�����
}FreqInfo_t;

/**
 * <PRE>
 * �f�[�^�����֐�
 * </PRE>
 * @param swap1	������	
 * @param swap2	������	
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
 * �N�C�b�N�\�[�g�������Ȃ��֐�
 * </PRE>
 * @param start		�J�n�ʒu
 * @param end		�I���ʒu
 * @param qsort1	�\�[�g����f�[�^
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
 * �p���[�l�̍ő�l�����߂�
 * </PRE>
 * @param frame		�t���[����
 * @param freq_x	�X�y�N�g���̎���
 * @param freq_y	�X�y�N�g���̋���
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
 * �p���[�l�̍ŏ��l�����߂�
 * </PRE>
 * @param frame		�t���[����
 * @param freq_x	�X�y�N�g���̎���
 * @param freq_y	�X�y�N�g���̋���
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
 * �p���[�l�̍ő�ŏ��l�����߂�
 * </PRE>
 * @param frame		�t���[����
 * @param freq_x	�X�y�N�g���̎���
 * @param freq_y	�X�y�N�g���̋���
 * @param max_data	�p���[�ő�l
 * @param min_data	�p���[�ŏ��l
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
 * �ʑ��̍ő�l�����߂�
 * </PRE>
 * @param frame		�t���[����
 * @param freq_x	�X�y�N�g���̎���
 * @param freq_y	�X�y�N�g���̋���
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
 * �ʑ��̍ŏ��l�����߂�
 * </PRE>
 * @param frame		�t���[����
 * @param freq_x	�X�y�N�g���̎���
 * @param freq_y	�X�y�N�g���̋���
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
 * �ʑ��̍ő�ŏ��l�����߂�
 * </PRE>
 * @param frame		�t���[����
 * @param freq_x	�X�y�N�g���̎���
 * @param freq_y	�X�y�N�g���̋���
 * @param max_data	�ʑ��ő�l
 * @param min_data	�ʑ��ŏ��l
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
 * �p���[�̋ɑ�l�����߂�
 * </PRE>
 * @param frame		�t���[����
 * @param sampling	�T���v�����O���g��
 * @param peak		�擾����s�[�N��
 * @param freq_x	�X�y�N�g���̎���
 * @param freq_y	�X�y�N�g���̋���
 * @param peak_freq	�s�[�N�̎��g���i�[�z��
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

	//�s�[�N�̕����𒊏o����.
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
	//���g�����ɕ��ׂȂ���.
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
	//���g���l�����o��.
	if(peak_freq != NULL)
	{
		for(i = 0;i < peak1;i++)
			peak_freq[i] = tFreqInfo[i].freq*(double)sampling/(double)frame;
	}
	//�s�[�N�݂̂��o��(�p���[�f�[�^���s�[�N�ȊO0�ɂ���.)
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
 * �p���[�X�y�N�g���̋ɏ��l�����߂�
 * </PRE>
 * @param frame		�t���[����
 * @param sampling	�T���v�����O���g��
 * @param peak		�擾����s�[�N��
 * @param freq_x	�X�y�N�g���̎���
 * @param freq_y	�X�y�N�g���̋���
 * @param peak_freq	�s�[�N�̎��g���i�[�z��
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

	//�ɏ��l�̕����𒊏o����.
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
	//���g�����ɕ��ׂȂ���.
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
	//���g���l�����o��.
	if(peak_freq != NULL)
	{
		for(i = 0;i < peak1;i++)
			peak_freq[i] = tFreqInfo[i].freq*(double)sampling/(double)frame;
	}
	//�ɏ��l�݂̂����o��(�p���[�f�[�^���s�[�N�ȊO0�ɂ���.)
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
