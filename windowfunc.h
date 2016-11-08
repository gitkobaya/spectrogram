#ifndef _WINDOW_FUNC_H_
#define _WINDOW_FUNC_H_

#include <cmath>
#include "./include/constdata.h"

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
///////////窓関数

template<class T>
inline T bartlett(int n, int N)
{
	return 1.0-2.0*fabs((T)n/(T)(N-1)-0.5);
};

template<class T>
inline T hanning(int n, int N )
{
	return 0.5-0.5*cos(2.0*pi*(T)n/((T)N-1.0));
};

template<class T>
inline T hamming(int n, int N )
{
	return 0.54-0.46*cos(2.0*pi*(T)n/((T)N-1.0));
};

template<class T>
inline T blackman(int n,int N )
{
	return 0.42-0.5*cos(2.0*pi*(T)n/((T)N-1.0))+0.08*cos(4.0*pi*(T)n/((T)N-1.0));
};

template<class T>
inline T blackman_hann(int n,int N )
{
	return 0.62-0.48*fabs((T)n/((T)N-1)-0.5)-0.38*cos(2.0*pi*(T)n/((T)N-1));
};

template<class T>
inline T general_hamming(int n, int N,T alpha)
{
	return alpha-(1.0-alpha)*cos(2.0*pi*(T)n/((T)N-1.0));
};

template<class T>
inline T kaiser(int n, int N, T alpha)
{
	return _j0(alpha*sqrt(1.0-(2.0*(T)n/((T)N-1.0)-1.0)*(2.0*(T)n/((T)N-1.0)-1.0)))/_j0(alpha);
};

template<class T>
inline T gaussian(int n, int N, T sigma)
{
	T lfX = (T)n/(T)(N-1);
	return exp(-lfX*lfX/(sigma*sigma));
}; 

template<class T>
inline T exponential(int n, int N, T sigma)
{
	return exp(-(T)n/((T)(N-1))/sigma);
};

template<class T>
inline T flat_top( int n,int N )
{
	return 1-1.93*cos(2.0*pi*(T)n/((T)N-1.0))+1.29*cos(4.0*pi*(T)n/((T)N-1.0))-0.388*cos(6.0*pi*(T)n/((T)N-1.0))+0.032*cos(8.0*pi*(T)n/((T)N-1.0));
};

template<class T>
inline T blackman_nuttall( int n,int N )
{
	return 0.35875-0.48829*cos(2.0*pi*(T)n/((T)N-1.0))+0.14128*cos(4.0*pi*(T)n/((T)N-1.0))-0.01168*cos(6.0*pi*(T)n/((T)N-1.0));
};

template<class T>
inline T blackman_harris( int n,int N )
{
	return 0.355768-0.487396*cos(2.0*pi*(T)n/((T)N-1.0))+0.144232*cos(4.0*pi*(T)n/((T)N-1.0))-0.012604*cos(6.0*pi*(T)n/((T)N-1.0));
};

template<class T>
inline T akaike( int n,int N )
{
	return 0.625-0.5*cos(2.0*pi*(T)n/((T)N-1.0))-0.125*cos(4.0*pi*(T)n/((T)N-1.0));
};

template<class T>
inline T parzen( int n,int N )
{
	T lfData = (T)n/((T)N-1.0);
	return n <= N-1 ? 1 - 1.5*lfData*lfData+0.75*lfData*lfData*lfData : 0.25*(2.0-lfData*lfData*lfData);
};

template<class T>
inline T welch( int n,int N )
{
	T lfData = (T)n/((T)N-1.0);
	return 4.0*lfData*(1.0-lfData);
};

/**
 *
 *<PRE>
 *  MDCT窓関数(データ参照方法が変化する。)
 *</PRE>
 *
 **/
template<class T>
inline T wsin( int n,int N )
{
	T lfData = ((T)n+0.5)/(T)(N+N);
	return sin*(pi*lfData);
};

/**
 *
 *<PRE>
 *  MDCT窓関数(データ参照方法が変化する。)
 *</PRE>
 *
 **/
template<class T>
inline T wvorbis( int n,int N )
{
	T lfData = ((T)n+0.5)/(T)(N+N);
	T lfData1 = sin(pi*lfData);
	return sin*(pi/2*lfData1*lfData1);
};

/**
  * 周波数サンプリング法
  *@param frame		フレーム数
  *@param sampling	サンプリング周波数
  *@param data      周波数領域のフィルタデータ
  *@param wave      逆フーリエ変換後のデータ
  **/
template<class T>
T freq_sampling(int frame,int sampling,T* data,T* wave)
{
	fft(data,wave,-frame);
	return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
///////////聴覚フィルタ( fir )
template<class T>
inline T erb( const T &f )
{
	return 24.7+0.108*f;
}

template<class T>
inline T gamma_tone_filter( const T &A, const T &n, const T &b, const T &fai, const T &t, const T &fc)
{
	T omega = -2.0*pi*t;
	T res = A*pow( t, n-1 )*exp( omega*b*erb( fc ) )*cos( omega+fai );
	return res;
}

template<class T>
inline T gamma_chirp_filter(const T &A,const T &n,const T &b,const T &c,const T &fai,const T &t,const T &fc)
{
	T omega = -2.0*pi*t;
	T res = A*pow( t, n-1 )*exp( b*erb( fc ) )*cos( omega*fc+c*log(t)+fai );
	return res;
}

template<class T>
inline T auditory_filter( int flag )
{
	T res;
	return res;
}

#endif  _WINDOW_FUNC_H_