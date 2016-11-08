#ifndef _CORRELATION_H_
#define _CORRELATION_H_

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
extern void auto_correlation( const int &frame, const double *data, double *corr );

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
extern void cross_correlation( const int &frame, const double *data1, const double *data2, double *corr );

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
extern void corr_diff_lpc( const int &frame, const double *data, double *corr );

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
extern void corr_diff_prony( const int &frame, const double *data, double *corr );

#endif _CORRELATION_H_