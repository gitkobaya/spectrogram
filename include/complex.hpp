#ifndef _COMPLEX_HPP_
#define _COMPLEX_HPP_

#include<ostream>
#include<cmath>

template<class T>
class cmp
{
public:
	cmp();
	cmp(T);
	cmp(T,T);
	cmp<T>& operator = (const T&);
	cmp<T>& operator = (const cmp<T>&);
	cmp<T>& operator +=(const T&);
	cmp<T>& operator -=(const T&);
	cmp<T>& operator *=(const T&);
	cmp<T>& operator /=(const T&);
	cmp<T>& operator +=(const cmp<T>&);
	cmp<T>& operator -=(const cmp<T>&);
	cmp<T>& operator *=(const cmp<T>&);
	cmp<T>& operator /=(const cmp<T>&);
	cmp<T>  operator  +(const T&);
	cmp<T>  operator  -(const T&);
	cmp<T>  operator  *(const T&);
	cmp<T>  operator  /(const T&);
	cmp<T>  operator  +(const cmp<T>&);
	cmp<T>  operator  -(const cmp<T>&);
	cmp<T>  operator  *(const cmp<T>&);
	cmp<T>  operator  /(const cmp<T>&);
	bool    operator ==(const cmp<T>&);
	bool    operator !=(const cmp<T>&);
	bool    operator ==(const T&);
	bool    operator !=(const T&);
	T   real() const;
	T   imag() const;
	void	real(const T &_Tx);
	void	imag(const T &_Tx);		
	void    print_complex(int flag);
private:
	T		real_value;
	T		imag_value;
};


/////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////コンストラクタの定義
template<class T>
cmp<T>::cmp(){real_value = 0; imag_value = 0;}

template<class T>
cmp<T>::cmp(T _Tx){real_value = _Tx; imag_value = 0;}

template<class T>
cmp<T>::cmp(T _Tx,T _Ty){real_value = _Tx; imag_value = _Ty;}

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////
////////////////メンバ関数の定義
template<class T>
inline T  cmp<T>::real() const{ return real_value;}

template<class T>
inline T  cmp<T>::imag() const{ return imag_value;}

template<class T>
inline void  cmp<T>::real(const T &_Tx){real_value = _Tx;}

template<class T>
inline void  cmp<T>::imag(const T &_Tx){imag_value = _Tx;}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
////////////////演算子の定義
template<class T>
inline cmp<T>& cmp<T>::operator =(const cmp<T> &_Tx)
{
	real_value = _Tx.real();
	imag_value = _Tx.imag();
	return *this;
}

template<class T>
inline cmp<T>& cmp<T>::operator =(const T &_Tx)
{
	real_value = _Tx;
	imag_value = 0;
	return *this;
}

template<class T>
inline cmp<T>& cmp<T>::operator +=(const T &_Tx)
{
	real_value = real_value + _Tx;
	return *this;
}

template<class T>
inline cmp<T>& cmp<T>::operator -=(const T &_Tx)
{
	real_value = real_value - _Tx;
	return *this;
}

template<class T>
inline cmp<T>& cmp<T>::operator *=(const T &_Tx)
{
	real_value = real_value * _Tx;
	imag_value = imag_value * _Tx;
	return *this;
}

template<class T>
inline cmp<T>& cmp<T>::operator /=(const T &_Tx)
{
	real_value = real_value / _Tx;
	imag_value = imag_value / _Tx;
	return *this;
}

template<class T>
inline cmp<T>& cmp<T>::operator +=(const cmp<T> &_Tx)
{
	real_value = real_value + _Tx.real();
	imag_value = imag_value + _Tx.imag();
	return *this;
}

template<class T>
inline cmp<T>& cmp<T>::operator -=(const cmp<T> &_Tx)
{
	real_value = real_value - _Tx.real();
	real_value = imag_value - _Tx.imag();
	return *this;
}

template<class T>
inline cmp<T>& cmp<T>::operator *=(const cmp<T> &_Tx)
{
	T real1,imag1;
	real1 = real_value*_Tx.real() - imag_value*_Tx.imag();
	imag1 = real_value*_Tx.imag() + imag_value*_Tx.real();
	real_value = real1; imag_value = imag1;
	return *this;
}

template<class T>
cmp<T>& cmp<T>::operator /=(const cmp<T> &_Tx)
{
	T _S;
	T _Y2X2;
	T real1,imag1;

	if(_Tx.real() && fabs(_Tx.real()) >= fabs(_Tx.imag()) && fabs(_Tx.imag()) >= 0)
	{
		_Y2X2 = _Tx.imag()/_Tx.real();
		_S = _Tx.real() + _Tx.imag()*_Y2X2;
		real1 = (real_value + imag_value*_Y2X2) / _S;
		imag1 = (imag_value - real_value*_Y2X2) / _S;
	}
	else if(fabs(_Tx.imag()) > fabs(_Tx.real()) && fabs(_Tx.real()) >= 0)
	{
		_Y2X2 = _Tx.real()/_Tx.imag();
		_S = _Tx.real()*_Y2X2 + _Tx.imag();
		real1 = (real_value*_Y2X2 + imag_value) / _S;
		imag1 = (imag_value*_Y2X2 - real_value) / _S;
	}
	real_value = real1; imag_value = imag1;
	return *this;
}

template<class T>
inline cmp<T> cmp<T>::operator +(const T &_Tx)
{
	return cmp<T>(real_value + _Tx,imag_value);
}

template<class T>
inline cmp<T> cmp<T>::operator -(const T &_Tx)
{
	return cmp<T>(real_value - _Tx,imag_value);
}

template<class T>
inline cmp<T> cmp<T>::operator *(const T &_Tx)
{
	return cmp<T>(_Tx*real_value,_Tx*imag_value);
}

template<class T>
inline cmp<T> cmp<T>::operator /(const T &_Tx)
{
	return cmp<T>(real_value/_Tx,imag_value/_Tx);
}

template<class T>
inline cmp<T> cmp<T>::operator +(const cmp<T> &_Tx)
{
	cmp<T> _Ty;
	_Ty.real(real_value + _Tx.real());
	_Ty.imag(imag_value + _Tx.imag());
	return _Ty;
}

template<class T>
inline cmp<T> cmp<T>::operator -(const cmp<T> &_Tx)
{
	cmp<T> _Ty;
	_Ty.real(real_value - _Tx.real());
	_Ty.imag(imag_value - _Tx.imag());
	return _Ty;
}

template<class T>
inline cmp<T> cmp<T>::operator *(const cmp<T> &_Tx)
{
	cmp<T> _Ty;
	_Ty.real(real_value*_Tx.real() - imag_value*_Tx.imag());
	_Ty.imag(real_value*_Tx.imag() + imag_value*_Tx.real());
	return _Ty;
}

template<class T>
cmp<T> cmp<T>::operator /(const cmp<T> &_Tx)
{
	cmp<T> _Ty;
	T _S;
	T _Y2X2;

	if(_Tx.real() && fabs(_Tx.real()) >= fabs(_Tx.imag()) && fabs(_Tx.imag()) >= 0)
	{
		_Y2X2 = _Tx.imag()/_Tx.real();
		_S = _Tx.real() + _Tx.imag()*_Y2X2;
		_Ty.real((real_value + imag_value*_Y2X2) / _S);
		_Ty.imag((imag_value - real_value*_Y2X2) / _S);
	}
	else if(fabs(_Tx.imag()) > fabs(_Tx.real()) && fabs(_Tx.real()) >= 0)
	{
		_Y2X2 = _Tx.real()/_Tx.imag();
		_S = _Tx.real()*_Y2X2 + _Tx.imag();
		_Ty.real((real_value*_Y2X2 + imag_value) / _S);
		_Ty.imag((imag_value*_Y2X2 - real_value) / _S);
	}
	return _Ty;
}

template<class T>
inline bool cmp<T>::operator ==(const cmp<T> &_Tx)
{
	if(real_value == _Tx.real() && imag_value == _Tx.imag())
		return true;
	return false;
}

template<class T>
inline bool cmp<T>::operator !=(const cmp<T> &_Tx)
{
	if((const T)real_value != _Tx.real() && (const T)imag_value != _Tx.imag())
		return true;
	return false;
}

template<class T>
inline bool cmp<T>::operator ==(const T &_Tx)
{
	if(real_value == _Tx.real() && imag_value == 0)
		return true;
	return false;
}

template<class T>
inline bool cmp<T>::operator !=(const T &_Tx)
{
	if(real_value != _Tx && imag_value != 0)
		return true;
	return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
///////////演算子の定義フレンド関数
template<class T,class U>
inline cmp<U> operator +(const T& _Tx,const cmp<U> &_Ty)
{
	cmp<U> _Tz;
	_Tz.real(_Tx + _Ty.real());
	_Tz.imag(_Ty.imag());
	return _Tz;
}

template<class T,class U>
inline cmp<U> operator -(const T& _Tx,const cmp<U> &_Ty)
{
	cmp<U> _Tz;
	_Tz.real(_Tx - _Ty.real());
	_Tz.imag(0   - _Ty.imag());
	return _Tz;
}

template<class T,class U>
inline cmp<U> operator *(const T &_Tx,const cmp<U> &_Ty)
{
	cmp<U> _Tz;
	_Tz.real(_Tx * _Ty.real());
	_Tz.imag(_Tx * _Ty.imag());
	return _Tz;
}

template<class T,class U>
inline cmp<U> operator /(const T& _Tx,const cmp<U> &_Ty)
{
	cmp<U> _Tz;
	U _R;
	_R = _Ty.real()*_Ty.real() + _Ty.imag()*_Ty.imag();
	_Tz.real(    _Tx * _Ty.real() / _R);
	_Tz.imag(0 - _Tx * _Ty.imag() / _R);
	return _Tz;
}

template <class T>
std::ostream& operator <<(std::ostream& cout,const cmp<T> &_Tx)
{
	using namespace std;
	cout << "(" << _Tx.real() << ",";
	cout << _Tx.imag() << ")";
	return cout;
}

template<class T>
void cmp<T>::print_complex(int flag)
{
	using namespace std;
	if(flag == 1)
	{
		cout << "(";
	    mp_print(real_value,1);
		cout << ",";
		mp_print(imag_value,1);
		cout << ")\n";
	}
	else
	{
		cout << "(";
	    mp_print(real_value,0);
		cout << ",";
		mp_print(imag_value,0);
		cout << ")\n";
	}
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/////////////////複素数を扱うための関数
template<class T>
inline cmp<T> cconj(const cmp<T> &_Tx)
{
	cmp<T> z(_Tx.real(),0.0 - _Tx.imag());
	return z;
}

template<class T>
inline T cabs(const cmp<T> &_Tx)
{
	T t;
	T absolute1 = fabs(_Tx.real());
	T absolute2 = fabs(_Tx.imag());
	if(absolute1 == 0) return absolute2;
	else if(absolute2 == 0) return absolute1;
	if(absolute2 > absolute1)
	{
		t = _Tx.real()/_Tx.imag();
		return ( absolute2*sqrt( 1+t*t ) );
	}
	else
	{
		t = _Tx.imag()/_Tx.real();
		return ( absolute1*sqrt( 1+t*t ) );
	}
	return sqrt(_Tx.real()*_Tx.real()+_Tx.imag()*_Tx.imag());
}

template<class T>
inline T carg(const cmp<T> &_Tx)
{
	return atan2(_Tx.imag(),_Tx.real());
}

template<class T>
inline cmp<T> cexp(const cmp<T> &_Tx)
{
	T _R = exp(_Tx.real());
	cmp<T> z(_R*cos(_Tx.imag()),_R*sin(_Tx.imag()));
	return z;
}

template<class T>
cmp<T> cclog(const cmp<T> &_Tx)
{
	cmp<T> z(0.5*log(cabs(_Tx)),atan2(_Tx.imag(),_Tx.real()));
	return z;
}

template<class T,class U>
inline cmp<U> cpolar(const T &_R,const U &_Arg)
{
	cmp<U> z(_R*cos(_Arg),_R*sin(_Arg));
	return z;
}

template<class T>
inline cmp<T> cpow(const cmp<T> &_Tx,const cmp<T> &_Ty)
{
	return exp(_Ty*log(_Tx));
}

template<class T>
cmp<T> cpow(const cmp<T> &_Tx,const unsigned long &_Ty)
{
	cmp<T> _Tz(1,0);
	unsigned int i;
	for(i = 0;i < _Ty;i++) _Tz = _Tz * _Tx;
	return _Tz;
}

template<class T>
inline cmp<T> csin(const cmp<T> &_Tx)
{
	T e,f;
	e = exp(_Tx.imag());
	f = 1/e;
	return cmp<T>( 0.5*sin(_Tx.real())*( e+f ) , 0.5*cos(_Tx.real())*( e-f ) );
}

template<class T>
inline cmp<T> ccos(const cmp<T> &_Tx)
{
	T e,f;
	e = exp(_Tx.imag());
	f = 1/e;
	return cmp<T>( 0.5*cos(_Tx.real())*( f+e ) , 0.5*sin(_Tx.real())*( f-e ) );
}

template<class T>
inline cmp<T> ctan(const cmp<T> &_Tx)
{
	T e,f,d,h;
	h = _Tx.real()+_Tx.real();

	e = exp( _Tx.imag()+_Tx.imag() );
	f = 1/e;
	d = cos(h)+0.5*( e+f );
	return cmp<T>( sin(h)/d , 0.5*( e-f )/d );
}

template<class T>
inline cmp<T> csinh(const cmp<T> &_Tx)
{
	T e,f;
	e = exp(_Tx.real());
	f = 1/e;
	return cmp<T>(0.5*( e-f )*cos(_Tx.imag()) , 0.5*( e+f )*sin(_Tx.imag()) );
}

template<class T>
inline cmp<T> ccosh(const cmp<T> &_Tx)
{
	T e,f;
	e = exp(_Tx.real());
	f = 1/e;
	return cmp<T>( 0.5*( e+f )*cos(_Tx.imag()) , 0.5*( e-f )*sin(_Tx.imag()) );
}

template<class T>
inline cmp<T> ctanh(const cmp<T> &_Tx)
{
	T e,f,d,h;
	h = _Tx.imag()+_Tx.imag();
	e = exp( _Tx.real()+_Tx.real() );
	f = 1 / e;
	d = 0.5*( e+f )+cos(h);
	return cmp<T>( 0.5*( e-f )/d , sin(h)/d );
}

template<class T>
inline cmp<T> csqrt(const cmp<T> &_Tx)
{
	T r,w;
	r = cabs(_Tx);
	w = sqrt(r+_Tx.real());
	if(_Tx.real() >= 0)
		return cmp<T>(sqrt05*w,sqrt05*_Tx.imag()/w);
	else
		return cmp<T>(sqrt05*fabs(_Tx.imag())/w,(_Tx.imag() >= 0) ? sqrt05*w : -sqrt05*w);
	return _Tx;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////基本演算
template<class T>
inline void cadd(T cmp1[2],T cmp2[2],T cmp3[2])
{
	cmp3[0] = cmp1[0] + cmp2[0];
	cmp3[1] = cmp1[1] + cmp2[1];
}

template<class T>
inline void csub(T cmp1[2],T cmp2[2],T cmp3[2])
{
	cmp3[0] = cmp1[0] - cmp2[0];
	cmp3[1] = cmp1[1] - cmp2[1];
}

template<class T>
inline void cmul(T cmp1[2],T cmp2[2],T cmp3[2])
{
	T dum[2];
	dum[0] = cmp1[0]*cmp2[0] - cmp1[1]*cmp2[1];
	dum[1] = cmp1[0]*cmp2[1] + cmp1[1]*cmp2[0];
	cmp3[0] = dum[0];	cmp3[1] = dum[1];
}

template<class T>
inline void cdiv(T cmp1[2],T cmp2[2],T cmp3[2])
{
	T _S,_Y2X2,dum[2];

	if(cmp2[0] && fabs(cmp2[0]) >= fabs(cmp2[1]) && fabs(cmp2[1]) >= 0)
	{
		_Y2X2 = cmp2[1]/cmp2[0];
		_S = cmp2[0] + cmp2[1]*_Y2X2;
		dum[0] = (cmp1[0] + cmp1[1]*_Y2X2) / _S;
		dum[1] = (cmp1[1] - cmp1[0]*_Y2X2) / _S;
	}
	else if(fabs(cmp2[1]) > fabs(cmp2[0]) && fabs(cmp2[0]) >= 0)
	{
		_Y2X2 = cmp2[0]/cmp2[1];
		_S = cmp2[0]*_Y2X2 + cmp2[1];
		dum[0] = (cmp1[0]*_Y2X2 + cmp1[1]) / _S;
		dum[1] = (cmp1[1]*_Y2X2 - cmp1[0]) / _S;
	}
	cmp3[0] = dum[0];	cmp3[1] = dum[1];
}

template<class T>
inline void cconj(T cmp[2],T res[2])
{
	res[0] = cmp[0];
	res[1] = 0.0 - cmp[1];
}

template<class T>
inline T cabs(T cmp[2])
{
	T t;
	T absolute1 = fabs(cmp[0]);
	T absolute2 = fabs(cmp[1]);
	if(absolute1 == 0) return absolute2;
	else if(absolute2 == 0) return absolute1;
	if( absolute2 > absolute1 )
	{
		t = cmp[0]/cmp[1];
		return ( absolute2*sqrt( 1+t*t ) );
	}
	else
	{
		t = cmp[1]/cmp[0];
		return ( absolute1*sqrt( 1+t*t ) );
	}
	return sqrt(cmp[0]*cmp[0]+cmp[1]*cmp[1]);
}

template<class T>
inline T carg(T cmp[2])
{
	return atan2(cmp[1],cmp[0]);
}

template<class T>
inline void cexp(T cmp[2],T res[2])
{
	T _R = exp(cmp[0]);
	res[0] = _R*cos(cmp[1]);
	res[1] = _R*sin(cmp[1]);
}

template<class T>
inline void cclog(T cmp[2],T res[2])
{
	res[0] = 0.5*log(cabs(cmp));
	res[1] = carg(cmp);
}

template<class T,class U>
inline void cpolar(const T &_R,const U &_Arg,T res[2])
{
	res[0] = _R*cos(_Arg);
	res[1] = _R*sin(_Arg);
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////拡張演算

template<class T>
inline void cpow(T cmp1[2],T cmp2[2],T res[2])
{
	T x[2];
	clog(cmp2,x);
	mul(cmp1,x,res);
	cexp(res,res);
}

template<class T>
inline void cpow(T cmp[2],T res[2],unsigned long _Ty)
{
	for(int i = 0;i < _Ty;i++) _Tz = _Tz * _Tx;
	return _Tz;
}

template<class T>
inline void csin(T cmp[2],T res[2])
{
	T e,f;
	e = exp(cmp[1]);
	f = 1/e;
	res[0] = 0.5*sin(cmp[0])*( e+f );
	res[1] = 0.5*cos(cmp[0])*( e-f );
}

template<class T>
inline void ccos(T cmp[2],T res[2])
{
	T e,f;
	e = exp(cmp[1]);
	f = 1/e;
	res[0] = 0.5*cos(cmp[0])*( f+e );
	res[1] = 0.5*sin(cmp[0])*( f-e );
}

template<class T>
inline void ctan(T cmp[2],T res[2])
{
	T e,f,d,h;
	h = cmp[0]+cmp[0];
	e = exp( cmp[1]+cmp[1] );
	f = 1 / e;
	d = cos(h)+0.5*( e+f );
	res[0] = sin(h)/d;
	res[1] = 0.5*( e-f )/d;
}

template<class T>
inline void csinh(T cmp[2],T res[2])
{
	T e,f;
	e = exp(cmp[0]);
	f = 1/e;
	res[0] = 0.5*( e-f )*cos(cmp[1]);
	res[1] = 0.5*( e+f )*sin(cmp[1]);
}

template<class T>
inline void ccosh(T cmp[2],T res[2])
{
	T e,f;
	e = exp(cmp[0]);
	f = 1/e;
	res[0] = 0.5*( e+f )*cos(cmp[1]);
	res[1] = 0.5*( e-f )*sin(cmp[1]);
}

template<class T>
inline void ctanh(T cmp[2],T res[2])
{
	T e,f,d,h;
	h = cmp[1]+cmp[1];
	e = exp( cmp[0]+cmp[0] );
	f = 1/e;
	d = 0.5*( e+f )+cos(h);
	res[0] = 0.5*( e-f )/d;
	res[1] = sin(h)/d;
}

template<class T>
inline void csqrt(T cmp[2],T res[2])
{
	T r,w;
	r = cabs(cmp);
	w = sqrt(r + cmp[0]);
	if(cmp[0] >= 0)
	{
		res[0] = sqrt05*w;
		res[1] = sqrt05*cmp[1]/w;
	}
	else
	{
		res[0] = sqrt05*fabs(cmp[1])/w;
		res[1] = (cmp[1] >= 0) ? sqrt05*w : -sqrt05*w;
	}
}
#endif  _COMPLEX_HPP_