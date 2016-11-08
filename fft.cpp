
#include <cstdio>
#include <cmath>
#include <memory>
#include <exception>
#include "./include/constdata.h"
#include "./include/integral.hpp"
#include "corr.h"
#include "fft.h"

/**
 * <PRE>
 * fft��sin�e�[�u�����쐬����
 * </PRE>
 * @author kobayashi
 * @param n			�t���[����
 * @param sintable	sin�e�[�u��
 * @since 2009/10/04
 * @version 0.1
 */
void make_fft_sintable( const int &n, double *sintable )
{
	int i,n2,n4,n8;
	double c, s, dc, ds, t;

	n2 = n>>1;
	n4 = n>>2;
	n8 = n>>3;
	t = sin(pi/n);
	dc = 2 * t * t;
	ds = sqrt(dc * (2 - dc));
	t = 2 * dc;
	c = sintable[n4] = 1;
	s = sintable[0] = 0;
	for(i = 1;i < n8;i++)
	{
		c -= dc;
		dc += t * c;
		s += ds;
		ds -= t*s;
		sintable[i] = s;
		sintable[n4-i] = c;
	}
	if(n8 != 0) sintable[n8] = sqrt(0.5);
	for( i=0 ; i<n4 ; i++ )
	{
		sintable[n2-i] = sintable[i];
		sintable[i+n2] = -sintable[i];
	}
}

/**
 * <PRE>
 * ���z�֐���sin�e�[�u�����쐬����
 * </PRE>
 * @author kobayashi
 * @param n			�t���[����
 * @param sintable	sin�e�[�u��
 * @since 2009/10/04
 * @version 0.1
 */
void make_freq_table( const int &n, double lfPi, double *lfReal, double *lfImg, double *res_r, double *res_i )
{
	int i,j;
	for( j=0 ; j<n ; j++ )
	{
		for( i=0 ; i<n ; i++ )
		{
			res_r[j] = res_r[j]+lfReal[i]*cos(lfPi/(double)n*i)+lfImg[i]*sin(lfPi/(double)n*i);
			res_i[j] = res_i[j]-lfReal[i]*sin(lfPi/(double)n*i)+lfImg[i]*cos(lfPi/(double)n*i);
		}
	}
}


/**
 * <PRE>
 * fft��sin�e�[�u�����쐬����
 * </PRE>
 * @author kobayashi
 * @param n			�t���[����
 * @param bitprev	�v�Z���鏇��
 * @since 2009/10/04
 * @version 0.1
 */
void make_bittable( const int &n, int *bitprev )
{
	int i,j,k,n2;

	n2 = n>>1;
	i = j = 0;
	for(;;)
	{
		bitprev[i] = j;
		if(++i >= n) break;
		k = n2;
		while(k <= j)
		{
			j -= k;
			k /= 2;
		}
		j += k;
	}
}

/**
 * <PRE>
 * fft�����t�[���G�ϊ������s����B
 * 
 * </PRE>
 * @author kobayashi
 * @param n			�t���[����
 *					���FFFT
 *					���FIFFT
 * @param sintable	�T�C���R�T�C���e�[�u��
 * @param bitprev	�r�b�g��
 * @param x			���͎��F�g�`(FFT)�A���g��(IFFT)
 *					�o�͎��F�g�`(FFT)�A���g��(IFFT)
 * @param y			���͎��F���g��(IFFT)�A�g�`(FFT)
 *					�o�͎��F���g��(IFFT)�A�g�`(FFT)
 * @since 2009/10/04
 * @version 0.1
 */
void fft( int n, const double *sintable, const int *bitprev, double *x, double *y )
{
	int	i, j, k, ik, h, d, k2, n4, inverse;
	int n_1 = n-1;
	double	t, s, c, dx, dy;
	double	frame_rate;

	if(n < 0)
	{
		n =-n;
		inverse = -1;
	}
	else inverse = 1;
	n4 = n>>2;

	for(i = 0;i < n;i++)
	{
		j = bitprev[i];
		if(i < j)
		{
			t = x[i];
			x[i] = x[j];
			x[j] = t;
			t = y[i];
			y[i] = y[j];
			y[j] = t;
		}
	}
	for(k = 1;k < n;k = k2)
	{
		h = 0;
		k2 = k+k;
		d = n / k2;
		for( j=0 ; j<k ; j++ )
		{
			c = sintable[h+n4];
			s = inverse*sintable[h];

			for( i=j ; i<n ; i+=k2 )
			{
				ik = i+k;
				if( ik < n_1 )
				{
					dx = s * y[ik] + c * x[ik];
					dy = c * y[ik] - s * x[ik];
					x[ik] = x[i] - dx;
					x[i] += dx;
					y[ik] = y[i] - dy;
					y[i] += dy;
				}
			}
			h += d;
		}
	}
	/*
	if(inverse==1)
	{
		frame_rate = 1.0/(double)n;
		for(i = 0;i < n;i++)
		{
			x[i] *= frame_rate;
			y[i] *= frame_rate;
		}
	}*/
}

/**
 * <PRE>
 * ft�������Ɏ��s����B(�ϕ������s����)
 * ver0.1 �V�K�쐬
 * ver0.2 ��O�����̕��������܂����삵�Ă��Ȃ��̂œ��삷��悤�ɏC���B
 * </PRE>
 * @author kobayashi
 * @param frame			�t���[����
 * @param sampling		�T���v�����O���g��
 * @param data			�g�`
 * @param freq_data_r	�t�[���G�ϊ���̎���
 * @param freq_data_r	�t�[���G�ϊ���̋���
 * @since 2009/10/04
 * @version 0.1
 */
void ft_spectrum_density(int frame,int sampling,double *data,double *freq_data_r,double *freq_data_i)
{
	int i,j;
	double loc;
	double con = pi2/(double)frame;
	double delta_x[2];
	double result[2];
	double *data_r = NULL;
	double *data_i = NULL;

	try
	{
		data_r = new double[frame];
		data_i = new double[frame];
	}
	catch( std::bad_alloc ba )
	{
		// bad_alloc���O�Ƃ��ĕԂ��B
		throw( ba );
	}
	delta_x[0] = delta_x[1] = 1.0/(double)sampling;

	for( i=0 ; i<frame ; i++ )
	{
		for( j=0 ; j<frame ; j++ )
		{
			loc = i*j*con;
			data_i[j] = data[j]*sin(loc);
			data_r[j] = data[j]*cos(loc);
		}
		daikeisoku(data_r,data_i,0,frame,delta_x,result);
		freq_data_r[i] = result[0];
		freq_data_i[i] = result[1];
	}
	try
	{
		if( data_r != NULL )
		{
			delete[] data_r;
			data_r = NULL;
		}
		if( data_i != NULL )
		{
			delete[] data_i;
			data_i = NULL;
		}
	}
	catch( ... )
	{
		// �T�C�Y������Ȃ���Ԃŉ�������Ȃ�
		std::bad_exception bae;
		throw( bae );
	}
}

/**
 * <PRE>
 * Power Spectrum Density�𗝘_�ʂ�Ɏ��s����B
 * ver0.1 �V�K�쐬
 * ver0.2 ��O�����̕��������܂����삵�Ă��Ȃ��̂œ��삷��悤�ɏC���B
 * </PRE>
 * @author kobayashi
 * @param frame		�t���[����
 * @param sampling	�T���v�����O���g��
 * @param data		�g�`
 * @param freq_x	�t�[���G�ϊ���̎���
 * @param freq_y	�t�[���G�ϊ���̋���
 * @since 2009/10/04
 * @version 0.2
 */
void blackman_chuker( const int &frame, const int &sampling, const double *data, double *freq_x, double *freq_y )
{
	double *data_r = NULL;
	try
	{
		data_r = new double[frame];
		auto_correlation( frame, data, data_r );
		ft_spectrum_density( frame, sampling, data_r, freq_x, freq_y);
		delete[] data_r;
	}
	catch( std::bad_alloc ba )
	{
		// bad_alloc���O�Ƃ��ĕԂ��B
		throw( ba );
	}
	catch( ... )
	{
		// �T�C�Y������Ȃ���Ԃŉ�������Ȃ�
		std::bad_exception bae;
		throw( bae );
	}
}

/**
 * <PRE>
 * Cross Spectrum Density�𗝘_�ʂ�Ɏ��s����B
 * ver0.1 �V�K�쐬
 * ver0.2 ��O�����̕��������܂����삵�Ă��Ȃ��̂œ��삷��悤�ɏC���B
 * </PRE>
 * @author kobayashi
 * @param frame		�t���[����
 * @param sampling	�T���v�����O���g��
 * @param data1		�g�`
 * @param data2		�g�`
 * @param freq_x	�t�[���G�ϊ���̎���
 * @param freq_y	�t�[���G�ϊ���̋���
 * @since 2009/10/04
 * @version 0.2
 */
void cross_spectrum( const int &frame, const int &sampling, const double *data1, const double *data2, double *freq_x, double *freq_y )
{
	double *data_r = NULL;
	try
	{
		data_r = new double[frame];
		cross_correlation( frame, data1, data2, data_r );
		ft_spectrum_density( frame, sampling, data_r, freq_x, freq_y);
		delete[] data_r;
	}
	catch( std::bad_alloc ba )
	{
		// bad_alloc���O�Ƃ��ĕԂ��B
		throw( ba );
	}
	catch( ... )
	{
		// �T�C�Y������Ȃ���Ԃŉ�������Ȃ�
		std::bad_exception bae;
		throw( bae );
	}
}
