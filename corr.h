#ifndef _CORRELATION_H_
#define _CORRELATION_H_

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
extern void auto_correlation( const int &frame, const double *data, double *corr );

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
extern void cross_correlation( const int &frame, const double *data1, const double *data2, double *corr );

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
extern void corr_diff_lpc( const int &frame, const double *data, double *corr );

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
extern void corr_diff_prony( const int &frame, const double *data, double *corr );

#endif _CORRELATION_H_