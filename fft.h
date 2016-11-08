#ifndef _FFT_H_
#define _FFT_H_

/**
 * <PRE>
 * fftのsinテーブルを作成する
 * </PRE>
 * @author kobayashi
 * @param n			フレーム幅
 * @param sintable	sinテーブル
 * @version 0.1
 */
extern void make_fft_sintable( const int &n, double *sintable );

/**
 * <PRE>
 * 分布関数のsinテーブルを作成する
 * </PRE>
 * @author kobayashi
 * @param n			フレーム幅
 * @param sintable	sinテーブル
 * @version 0.1
 */
extern void make_freq_table( const int &n, double lfPi, double *lfReal, double *lfImg, double *res_r, double *res_i );

/**
 * <PRE>
 * fftのsinテーブルを作成する
 * </PRE>
 * @author kobayashi
 * @param n			フレーム幅
 * @param bitprev	計算する順番
 * @version 0.1
 */
extern void make_bittable( const int &n, int *bitprev );

/**
 * <PRE>
 * fft高速フーリエ変換を実行する。
 * 
 * </PRE>
 * @author kobayashi
 * @param n			フレーム幅
 *					正：FFT
 *					負：IFFT
 * @param sintable	サインコサインテーブル
 * @param bitprev	ビット列
 * @param x			入力時：波形(FFT)、周波数(IFFT)
 *					出力時：波形(FFT)、周波数(IFFT)
 * @param y			入力時：周波数(IFFT)、波形(FFT)
 *					出力時：周波数(IFFT)、波形(FFT)
 * @version 0.1
 */
extern void fft( int n, const double *sintable, const int *bitprev, double *x, double *y );

/**
 * <PRE>
 * ftを純粋に実行する。(積分を実行する)
 * </PRE>
 * @author kobayashi
 * @param frame			フレーム幅
 * @param sampling		サンプリング周波数
 * @param data			波形
 * @param freq_data_r	フーリエ変換後の実部
 * @param freq_data_r	フーリエ変換後の虚部
 * @version 0.1
 */
extern void ft_spectrum_density(int frame,int sampling,double *data,double *freq_data_r,double *freq_data_i);

/**
 * <PRE>
 * Power Spectrum Densityを理論通りに実行する。
 * </PRE>
 * @author kobayashi
 * @param frame		フレーム幅
 * @param sampling	サンプリング周波数
 * @param data		波形
 * @param freq_x	フーリエ変換後の実部
 * @param freq_y	フーリエ変換後の虚部
 * @version 0.1
 */
extern void blackman_chuker( const int &frame, const int &sampling, const double *data, double *freq_x, double *freq_y );

/**
 * <PRE>
 * Cross Spectrum Densityを理論通りに実行する。
 * </PRE>
 * @author kobayashi
 * @param frame		フレーム幅
 * @param sampling	サンプリング周波数
 * @param data1		波形
 * @param data2		波形
 * @param freq_x	フーリエ変換後の実部
 * @param freq_y	フーリエ変換後の虚部
 * @version 0.1
 */
extern void cross_spectrum( const int &frame, const int &sampling, const double *data1, const double *data2, double *freq_x, double *freq_y );

#endif _FFT_H_
