#ifndef _SERACH_MAX_MIN_H_
#define _SERACH_MAX_MIN_H_

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////極大,極小,最大,最小値を求める関数

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
extern double search_max( const int &frame, double *freq_x, double *freq_y );

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
extern double search_min( const int &frame, double *freq_x, double *freq_y );

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
extern void search_maxmin( const int &frame, double *freq_x, double *freq_y, double *max_data, double *min_data );

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
extern double search_phase_max( const int &frame, double *freq_x, double *freq_y );

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
extern double search_phase_min( const int &frame, double *freq_x, double *freq_y );

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
extern void search_phase_maxmin( const int &frame, double *freq_x, double *freq_y, double *max_data, double *min_data );

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
extern void search_peak( const int &frame, const int &sampling, const int &peak, double *freq_x, double *freq_y, double *peak_freq );

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
extern void search_minimum( const int &frame, const int &sampling, const int &peak, double *freq_x, double *freq_y, double *peak_freq );

#endif _SERACH_MAX_MIN_H_