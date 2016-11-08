#ifndef _FFT_H_
#define _FFT_H_

/**
 * <PRE>
 * fft��sin�e�[�u�����쐬����
 * </PRE>
 * @author kobayashi
 * @param n			�t���[����
 * @param sintable	sin�e�[�u��
 * @version 0.1
 */
extern void make_fft_sintable( const int &n, double *sintable );

/**
 * <PRE>
 * ���z�֐���sin�e�[�u�����쐬����
 * </PRE>
 * @author kobayashi
 * @param n			�t���[����
 * @param sintable	sin�e�[�u��
 * @version 0.1
 */
extern void make_freq_table( const int &n, double lfPi, double *lfReal, double *lfImg, double *res_r, double *res_i );

/**
 * <PRE>
 * fft��sin�e�[�u�����쐬����
 * </PRE>
 * @author kobayashi
 * @param n			�t���[����
 * @param bitprev	�v�Z���鏇��
 * @version 0.1
 */
extern void make_bittable( const int &n, int *bitprev );

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
 * @version 0.1
 */
extern void fft( int n, const double *sintable, const int *bitprev, double *x, double *y );

/**
 * <PRE>
 * ft�������Ɏ��s����B(�ϕ������s����)
 * </PRE>
 * @author kobayashi
 * @param frame			�t���[����
 * @param sampling		�T���v�����O���g��
 * @param data			�g�`
 * @param freq_data_r	�t�[���G�ϊ���̎���
 * @param freq_data_r	�t�[���G�ϊ���̋���
 * @version 0.1
 */
extern void ft_spectrum_density(int frame,int sampling,double *data,double *freq_data_r,double *freq_data_i);

/**
 * <PRE>
 * Power Spectrum Density�𗝘_�ʂ�Ɏ��s����B
 * </PRE>
 * @author kobayashi
 * @param frame		�t���[����
 * @param sampling	�T���v�����O���g��
 * @param data		�g�`
 * @param freq_x	�t�[���G�ϊ���̎���
 * @param freq_y	�t�[���G�ϊ���̋���
 * @version 0.1
 */
extern void blackman_chuker( const int &frame, const int &sampling, const double *data, double *freq_x, double *freq_y );

/**
 * <PRE>
 * Cross Spectrum Density�𗝘_�ʂ�Ɏ��s����B
 * </PRE>
 * @author kobayashi
 * @param frame		�t���[����
 * @param sampling	�T���v�����O���g��
 * @param data1		�g�`
 * @param data2		�g�`
 * @param freq_x	�t�[���G�ϊ���̎���
 * @param freq_y	�t�[���G�ϊ���̋���
 * @version 0.1
 */
extern void cross_spectrum( const int &frame, const int &sampling, const double *data1, const double *data2, double *freq_x, double *freq_y );

#endif _FFT_H_
