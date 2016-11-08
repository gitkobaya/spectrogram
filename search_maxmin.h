#ifndef _SERACH_MAX_MIN_H_
#define _SERACH_MAX_MIN_H_

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////�ɑ�,�ɏ�,�ő�,�ŏ��l�����߂�֐�

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
extern double search_max( const int &frame, double *freq_x, double *freq_y );

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
extern double search_min( const int &frame, double *freq_x, double *freq_y );

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
extern void search_maxmin( const int &frame, double *freq_x, double *freq_y, double *max_data, double *min_data );

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
extern double search_phase_max( const int &frame, double *freq_x, double *freq_y );

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
extern double search_phase_min( const int &frame, double *freq_x, double *freq_y );

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
extern void search_phase_maxmin( const int &frame, double *freq_x, double *freq_y, double *max_data, double *min_data );

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
extern void search_peak( const int &frame, const int &sampling, const int &peak, double *freq_x, double *freq_y, double *peak_freq );

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
extern void search_minimum( const int &frame, const int &sampling, const int &peak, double *freq_x, double *freq_y, double *peak_freq );

#endif _SERACH_MAX_MIN_H_