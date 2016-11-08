#ifndef _INTEGRAL_HPP_
#define _INTEGRAL_HPP_

#include<cmath>
#include<complex>
#include<cfloat>

using namespace std;
#include"complex.hpp"
#include"constdata.h"


template<class T>
T romberg(int n);
template<class T>
T daikeisoku(T x_1,T x_2,int n,T (*pf)(T));
template<class T>
T gauss(T x_1,T x_2);
template<class T>
T nijushisuu(T a,T b,int n,T (*pf)(T));
template<class T>
T tyuutenn(T *fx,int start,int end,T delta_x);
template<class T>
void tyuutenn(T fx[][2],int start,int end,T delta_x[2],T result[2]);
template<class T>
std::complex<T> tyuutenn(std::complex<T> *fx,int start,int end,std::complex<T> delta_x);
template<class T>
cmp<T> tyuutenn(cmp<T> *fx,int start,int end,cmp<T> delta_x);
template<class T>
T daikeisoku(T *fx,int start,int end,T delta_x);
template<class T>
void daikeisoku(T *rfx,T *ifx,int start,int end,T delta_x[2],T result[2]);
template<class T>
void daikeisoku(T fx[][2],int start,int end,T delta_x[2],T result[2]);
template<class T>
std::complex<T> daikeisoku(std::complex<T> *fx,int start,int end,std::complex<T> delta_x);
template<class T>
cmp<T> daikeisoku(cmp<T> *fx,int start,int end,cmp<T> delta_x);
template<class T>
T sympson(T *fx,int start,int end,T delta_x);
template<class T>
void sympson(T fx[][2],int start,int end,T delta_x[2],T result[2]);
template<class T>
std::complex<T> sympson(std::complex<T> *fx,int start,int end,std::complex<T> delta_x);
template<class T>
cmp<T> sympson(cmp<T> *fx,int start,int end,cmp<T> delta_x);

extern double inte[100];


static double inte[100];

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/////関数既知の場合

//台形則
template<class T>
T daikeisoku(T x_1,T x_2,int n,T (*pf)(T))
{
	T sum = 0.0;
	T res = 0.0;
	T width = x_2-x_1;
	int i,j,k;
	int count = 0;

	for(i = n;i > 0;i >>= 1)
		count++;

	count--;
	inte[0] = res = (pf(x_1)+pf(x_2))/2.0*width;
	for(i = 1;i < count;i++)
	{
		k = 2 << (i-1);
		for(j = 1;j < k;j+= 2)
		{
			sum += pf((T)j/(T)k*width);
		}
		sum = sum*1.0/(T)k*width;
		res = res/2.0;
		res += sum;
		inte[i] = res;
		sum = 0.0;
	}
	return res;
}

//Romberg積分
template<class T>
T romberg(int n)
{
	int i,j;	
	int count = 0;

	for(i = n;i > 0;i >>= 1)
		count++;

	count--;
	for(i = 1;i < count;i++)
	{
		for(j = 0;j < count-i;j++)
		{
			inte[j] = (pow(4,i)*inte[j+1]-inte[j])/(pow(4,i)-1.0);
		}
	}
	return inte[0];
}

template<class T>
T gauss(T x_1,T x_2)
{
	return 0.0;
}

//2重指数形積分を行う関数
template<class T>
T nijushisuu(T a,T b,int n,T (*pf)(T))
{
	int flag = 0;
	T res = 0.0;
	T sum = 0.0;
	T h = (b-a);
	int i;

	if(n != 0)
		h=h/(T)n/2.0;
	if(b > DBL_MAX/2.0)
	{
		if(a < DBL_MAX/2.0)	   flag = 1;
		else if(a == 0.0)	   flag = 2;
		else				   flag = 3;
	}
	else					   flag = 4;

	for(i = -n;i < n+1;i++)
	{
		if(flag == 1)
		{
			res += pf(sinh(pi/2.0*sinh(i*h)))*pi/2.0*cosh(i*h)*cosh(pi/2.0*sinh(i*h));
		}
		else if(flag == 2)
		{
			res += pf(exp(pi*sinh(i*h)))*pi*cosh(i*h)*exp(pi*sinh(i*h));
		}
		else if(flag == 3)
		{
			res += pf(exp(pi*sinh(i*h))-a)*pi*cosh(i*h)*exp(pi*sinh(i*h));
		}
		else if(flag == 4)
		{
			T re = sinh(i*h);
			T re_1 = pi/2.0*sinh(i*h);
			T re_2 = (b-a)/2.0*tanh(re_1);
			T re_3 = re_2+(b+a)/2.0;
			T re_4 = pf(re_3);
			T re_5 = (pi/2.0*cosh(i*h));
			T re_6 = cosh(re_1);
			T re_7 = pow(re_6,2.0);
			T re_8 = re_6/re_7;
			T re_9 = (b-a)/2.0*re_8;
			T re_10= re_4*re_9;
			res += pf((b-a)/2.0*tanh(pi/2.0*sinh(i*h))+(b+a)/2.0)*(b-a)/2.0*
					(pi/2.0*cosh(i*h))/cosh(pi/2.0*sinh(i*h))/cosh(pi/2.0*sinh(i*h));
		}
	}
	res *= h;
	return res;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
////////データのみの場合

/*---------------------------------------------------------------------------------------*/
//中点則実数版
template<class T>
T tyuutenn(T *fx,int start,int end,T delta_x)
{
	T sum = 0.0;
	int i;
	for(i = start;i < end;i++)
		sum = sum + fx[i];
	return sum*delta_x;
}

//中点則複素数版(複素数の実部と虚部が2次元配列によって表現されている場合)
template<class T>
void tyuutenn(T fx[][2],int start,int end,T delta_x[2],T result[2])
{
	T sum[2] = {0.0,0.0};
	T dmult[2] = {0.0,0.0};
	int i;

	for(i = start;i < end;i++)
		cadd(sum,fx[i],sum);
	cmul(sum,delta_x,result);
}

//中点則複素数版
template<class T>
std::complex<T> tyuutenn(std::complex<T> *fx,int start,int end,std::complex<T> delta_x)
{
	complex<T> sum(0.0,0.0);
	complex<T> dmult(0.0,0.0);
	int i;

	for(i = start;i < end;i++)
		sum = sum+fx[i];
	return delta_x*sum;
}

//中点則複素数版
template<class T>
cmp<T> tyuutenn(cmp<T> *fx,int start,int end,cmp<T> delta_x)
{
	cmp<T> sum(0.0,0.0);
	cmp<T> dmult(0.0,0.0);
	int i;

	for(i = start;i < end;i++)
		sum = sum+fx[i];
	return delta_x*sum;
}

/*--------------------------------------------------------------------------------------*/
//台形則実数版
template<class T>
T daikeisoku(T *fx,int start,int end,T delta_x)
{
	T sum = 0.0;
	int i;
	for(i = start+1;i < end;i++)
		sum = sum + fx[i-1] + fx[i];
	return sum*delta_x*0.5;
}

//台形則複素数版(複素数の実部と虚部が分かれている場合)
template<class T>
void daikeisoku(T *rfx,T *ifx,int start,int end,T delta_x[2],T result[2])
{
	T fx[2]  = {0.0,0.0};
	T sum[2] = {0.0,0.0};
	T dmult[2] = {0.0,0.0};
	int i;

	for(i = start+1;i < end;i++)
	{
		fx[0]=rfx[i-1];	fx[1]=ifx[i-1];
		cadd(sum,fx,sum);
		fx[0]=rfx[i];	fx[1]=ifx[i];
		cadd(sum,fx,sum);
	}
	dmult[0] = delta_x[0]*0.5;
	dmult[1] = delta_x[1]*0.5;
	cmul(sum,dmult,result);
}

//台形則複素数版(複素数の実部と虚部が2次元配列によって表現されている場合)
template<class T>
void daikeisoku(T fx[][2],int start,int end,T delta_x[2],T result[2])
{
	T sum[2] = {0.0,0.0};
	T dmult[2] = {0.0,0.0};
	int i;

	for(i = start+1;i < end;i++)
	{
		cadd(sum,fx[i-1],sum);
		cadd(sum,fx[i],sum);
	}
	dmult[0] = delta_x[0]*0.5;
	dmult[1] = delta_x[1]*0.5;
	cmul(sum,dmult,result);
}

//台形則複素数版(C++のcomplexクラス)
template<class T>
std::complex<T> daikeisoku(std::complex<T> *fx,int start,int end,std::complex<T> delta_x)
{
	std::complex<T> sum(0.0,0.0);
	std::complex<T> dmult(0.0,0.0);
	int i;

	for(i = start+1;i < end;i++)
		sum = sum+fx[i-1]+fx[i];
	return delta_x*0.5*sum;
}

//台形則複素数版(自前複素数クラス)
template<class T>
cmp<T> daikeisoku(cmp<T> *fx,int start,int end,cmp<T> delta_x)
{
	cmp<T> sum(0.0,0.0);
	cmp<T> dmult(0.0,0.0);
	int i;

	for(i = start+1;i < end;i++)
		sum = sum+fx[i-1]+fx[i];
	return delta_x*0.5*sum;
}

/*----------------------------------------------------------------------------------------------*/
//シンプソン則実数版
template<class T>
T sympson(T *fx,int start,int end,T delta_x)
{
	T sum = 0.0;
	int i;
	for(i = start+1;i < end;i++)
		sum = sum + fx[i-1] + fx[i];
	return sum*delta_x*0.5;
}

//シンプソン則複素数版
template<class T>
void sympson(T fx[][2],int start,int end,T delta_x[2],T result[2])
{
	T sum[2] = {0.0,0.0};
	T dmult[2] = {0.0,0.0};
	int i;

	for(i = start+1;i < end;i++)
	{
		cadd(sum,fx[i-1],sum);
		cadd(sum,fx[i],sum);
	}
	dmult[0] = delta_x[0]*0.5;
	dmult[1] = delta_x[1]*0.5;
	cmul(sum,dmult,result);
}

//シンプソン則複素数版
template<class T>
std::complex<T> sympson(std::complex<T> *fx,int start,int end,std::complex<T> delta_x)
{
	std::complex<T> sum(0.0,0.0);
	std::complex<T> dmult(0.0,0.0);
	int i;

	for(i = start+1;i < end;i++)
		sum = sum+fx[i-1]+fx[i];
	return delta_x*0.5*sum;
}

//シンプソン則複素数版
template<class T>
cmp<T> sympson(cmp<T> *fx,int start,int end,cmp<T> delta_x)
{
	cmp<T> sum(0.0,0.0);
	cmp<T> dmult(0.0,0.0);
	int i;

	for(i = start+1;i < end;i++)
		sum = sum+fx[i-1]+fx[i];
	return delta_x*0.5*sum;
}
/*------------------------------------------------------------------------------------------------*/

#endif  _INTEGRAL_HPP_